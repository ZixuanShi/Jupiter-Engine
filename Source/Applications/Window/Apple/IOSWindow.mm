// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_IOS

#include "IOSWindow.h"
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

// UIKit has no ImGui backend -- imgui_impl_osx is AppKit-only -- so a single touch is mapped
// onto ImGui's mouse here. This is also where a real input system will tap in; until then the
// UI is the only consumer, so the events go straight to it.
- (void)reportTouch:(NSSet<UITouch*>*)touches isDown:(BOOL)isDown
{
    const CGPoint point = [touches.anyObject locationInView:self];

    // Points, not pixels: ImGui works in points and scales by DisplayFramebufferScale.
    jpt::ImGuiOnPointerMoved(static_cast<float>(point.x), static_cast<float>(point.y));
    jpt::ImGuiOnPointerButton(isDown);
}

- (void)touchesBegan:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouch:touches isDown:YES];
}

- (void)touchesMoved:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouch:touches isDown:YES];
}

- (void)touchesEnded:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouch:touches isDown:NO];
}

// A touch dragged off-screen or interrupted by a call never gets touchesEnded, and ImGui would
// be left holding the button down forever.
- (void)touchesCancelled:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event
{
    (void)event;
    [self reportTouch:touches isDown:NO];
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

    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];

    // A UIWindow with no rootViewController renders black.
    UIViewController* viewController = [[UIViewController alloc] init];
    JupiterMetalView* metalView = [[JupiterMetalView alloc] initWithFrame:UIScreen.mainScreen.bounds];
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
    struct IOSWindow::Impl
    {
        std::int32_t argc = 0;
        char** ppArgv = nullptr;
    };

    bool IOSWindow::PreInit(std::int32_t argc, char* ppArgv[])
    {
        m_pImpl = new Impl();
        m_pImpl->argc = argc;
        m_pImpl->ppArgv = ppArgv;
        return true;
    }

    // UIKit owns window creation and has not started yet -- the delegate above does the work
    // once Run() hands control over.
    bool IOSWindow::Init()
    {
        return true;
    }

    void IOSWindow::Run()
    {
        @autoreleasepool
        {
            UIApplicationMain(m_pImpl->argc, m_pImpl->ppArgv, nil,
                              NSStringFromClass([JupiterAppDelegate class]));
        }
    }

    void IOSWindow::Terminate()
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

#endif // IS_PLATFORM_IOS
