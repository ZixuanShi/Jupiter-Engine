// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_IOS

#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>

#include "AppleWindow.h"
#include "Graphics/Metal/MetalRenderer.h"

@interface JupiterMetalView : UIView
@end

@implementation JupiterMetalView

// Makes the view's own backing layer the CAMetalLayer, rather than a sublayer to keep in sync.
+ (Class)layerClass
{
    return [CAMetalLayer class];
}

- (void)layoutSubviews
{
    [super layoutSubviews];

    // bounds is in points, the drawable in pixels.
    const CGSize points = self.bounds.size;
    const CGFloat scale = self.contentScaleFactor;

    jpt::ResizeRenderer(static_cast<std::uint32_t>(points.width * scale), 
                        static_cast<std::uint32_t>(points.height * scale));
}

@end

@interface JupiterAppDelegate : NSObject <UIApplicationDelegate>
@property (nonatomic, strong) UIWindow* window;
@property (nonatomic, strong) CADisplayLink* displayLink;
@property (nonatomic, assign) CFTimeInterval startTime;
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

    if (!jpt::InitRenderer((__bridge void*)metalView.layer))
    {
        NSLog(@"Jupiter: Metal renderer failed to initialise");
        return YES;
    }

    // Silently never fires unless added to a run loop, and deallocs unless strongly held.
    self.startTime = CACurrentMediaTime();
    self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(onFrame:)];
    [self.displayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];

    return YES;
}

- (void)onFrame:(CADisplayLink*)sender
{
    (void)sender;
    jpt::DrawFrame(CACurrentMediaTime() - self.startTime);
}

- (void)applicationWillTerminate:(UIApplication*)application
{
    (void)application;
    [self.displayLink invalidate];
    self.displayLink = nil;
    jpt::ShutdownRenderer();
}

@end

namespace jpt
{
    // UIKit owns window creation and has not started yet -- the delegate above does the work.
    bool CreateAppWindow([[maybe_unused]] std::int32_t width,
                         [[maybe_unused]] std::int32_t height,
                         [[maybe_unused]] const char* title)
    {
        return true;
    }

    void RunAppLoop(int argc, char* argv[])
    {
        @autoreleasepool
        {
            // argv must be non-null: passing nullptr trips -Wnonnull, fatal under -Werror.
            // Never returns.
            UIApplicationMain(argc, argv, nil, NSStringFromClass([JupiterAppDelegate class]));
        }
    }
}

#endif // IS_PLATFORM_IOS
