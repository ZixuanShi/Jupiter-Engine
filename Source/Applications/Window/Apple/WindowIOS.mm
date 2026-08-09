// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_IOS

#include "WindowIOS.h"
#include "AppleCallbacks.h"
#include "Graphics/ImGui/ImGuiLayer.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>

@interface JupiterMetalView : UIView
@end

@implementation JupiterMetalView

// Makes the view's own backing layer the CAMetalLayer, rather than a sublayer to keep in sync.
+ (Class)layerClass
{
    return [CAMetalLayer class];
}

// UIKit has no ImGui backend, so the primary touch is mapped onto ImGui's mouse. Every touch
// also goes to jpt::Input, which recognizes the gestures.
//
// UITouchPhase rather than an engine enum: this is a .mm, so the platform's own type is the one
// it can name, and the four jpt::OnTouch* functions are what cross the import-free boundary.
- (void)reportTouches:(NSSet<UITouch*>*)touches phase:(UITouchPhase)phase
{
    const CGFloat scale = self.contentScaleFactor;

    for (UITouch* touch in touches)
    {
        const CGPoint point = [touch locationInView:self];
        const std::uint64_t touchId = reinterpret_cast<std::uintptr_t>(touch);

        // Pixels for the engine, which measures its viewport in pixels.
        const float x = static_cast<float>(point.x * scale);
        const float y = static_cast<float>(point.y * scale);

        switch (phase)
        {
        case UITouchPhaseBegan:
            if (!jpt::ImGuiWantsMouse())
            {
                jpt::OnTouchBegan(touchId, x, y, touch.timestamp);
            }
            break;

        case UITouchPhaseMoved:     jpt::OnTouchMoved(touchId, x, y, touch.timestamp);     break;
        case UITouchPhaseEnded:     jpt::OnTouchEnded(touchId, x, y, touch.timestamp);     break;
        case UITouchPhaseCancelled: jpt::OnTouchCancelled(touchId, x, y, touch.timestamp); break;
        default: break;
        }
    }

    // ImGui gets one finger, in points -- it scales by DisplayFramebufferScale itself.
    UITouch* primary = touches.anyObject;
    if (primary != nil)
    {
        const CGPoint point = [primary locationInView:self];
        jpt::ImGuiOnPointerMoved(static_cast<float>(point.x), static_cast<float>(point.y));
        jpt::ImGuiOnPointerButton(phase == UITouchPhaseBegan || phase == UITouchPhaseMoved);
    }
}

- (void)touchesBegan:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouches:touches phase:UITouchPhaseBegan];
}

- (void)touchesMoved:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouches:touches phase:UITouchPhaseMoved];
}

- (void)touchesEnded:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouches:touches phase:UITouchPhaseEnded];
}

// A touch dragged off-screen or interrupted by a call never gets touchesEnded.
- (void)touchesCancelled:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouches:touches phase:UITouchPhaseCancelled];
}

- (void)layoutSubviews
{
    [super layoutSubviews];

    // bounds is in points, the drawable in pixels.
    const CGSize points = self.bounds.size;
    const CGFloat scale = self.contentScaleFactor;

    jpt::OnResize(static_cast<std::uint32_t>(points.width * scale),
                  static_cast<std::uint32_t>(points.height * scale));
}

@end

@interface JupiterAppDelegate : NSObject <UIApplicationDelegate>
@property (nonatomic, strong) UIWindow* window;
@property (nonatomic, strong) CADisplayLink* displayLink;
@end

@implementation JupiterAppDelegate

- (BOOL)application:(UIApplication*)application
        didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    (void)application;
    (void)launchOptions;

    // Deprecated since the iOS 26 target. The real fix is adopting the UIScene lifecycle -- its
    // own change, so the deprecation is scoped here rather than silenced project-wide.
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    const CGRect screenBounds = UIScreen.mainScreen.bounds;
    self.window = [[UIWindow alloc] initWithFrame:screenBounds];
    #pragma clang diagnostic pop

    // A UIWindow with no rootViewController renders black.
    UIViewController* viewController = [[UIViewController alloc] init];
    JupiterMetalView* metalView = [[JupiterMetalView alloc] initWithFrame:screenBounds];

    // UIView defaults this to NO, and without it touchesBegan: never carries a second finger.
    metalView.multipleTouchEnabled = YES;
    viewController.view = metalView;
    self.window.rootViewController = viewController;

    [self.window makeKeyAndVisible];

    // metal-cpp types are typed views over the same ObjC object, so the bridge to void* and
    // back is how a CAMetalLayer* becomes a CA::MetalLayer*.
    if (!jpt::OnSurfaceReady(reinterpret_cast<CA::MetalLayer*>((__bridge void*)metalView.layer)))
    {
        return YES;
    }

    // After OnSurfaceReady, which creates the ImGui context. On iOS this only records the view
    // and sizes the fonts for touch; the events come from the handlers above.
    jpt::ImGuiInitPlatform((__bridge void*)metalView);

    // Silently never fires unless added to a run loop, and deallocs unless strongly held.
    self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(OnFrame:)];
    [self.displayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];

    return YES;
}

- (void)OnFrame:(CADisplayLink*)sender
{
    (void)sender;
    jpt::OnFrame();
}

// Presenting a drawable while backgrounded gets the app killed by the GPU watchdog.
- (void)applicationDidEnterBackground:(UIApplication*)application
{
    (void)application;
    self.displayLink.paused = YES;
}

- (void)applicationWillEnterForeground:(UIApplication*)application
{
    (void)application;
    self.displayLink.paused = NO;
}

// Not delivered once the app is suspended, so pausing above is what actually keeps this safe.
- (void)applicationWillTerminate:(UIApplication*)application
{
    (void)application;
    [self.displayLink invalidate];
    self.displayLink = nil;
    jpt::OnTerminate();
}

@end

namespace jpt
{
    struct WindowIOS::Impl
    {
        std::int32_t argc = 0;
        char** ppArgv = nullptr;
    };

    bool WindowIOS::PreInit(std::int32_t argc, char* ppArgv[])
    {
        m_pImpl = new Impl();
        m_pImpl->argc = argc;
        m_pImpl->ppArgv = ppArgv;
        return true;
    }

    // UIKit owns window creation and has not started yet -- the delegate above does the work
    // once Run() hands control over.
    bool WindowIOS::Init()
    {
        return true;
    }

    void WindowIOS::Run()
    {
        @autoreleasepool
        {
            UIApplicationMain(m_pImpl->argc, m_pImpl->ppArgv, nil,
                              NSStringFromClass([JupiterAppDelegate class]));
        }
    }

    void WindowIOS::Terminate()
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

#endif // IS_PLATFORM_IOS
