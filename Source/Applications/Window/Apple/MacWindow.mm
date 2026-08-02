// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS

#include "AppleWindow.h"
#include "Graphics/Metal/MetalRenderer.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <QuartzCore/CADisplayLink.h>

@interface JupiterMetalView : NSView
@property (nonatomic, strong) CADisplayLink* displayLink;
@property (nonatomic, assign) CFTimeInterval startTime;
@end

@implementation JupiterMetalView

// AppKit's counterpart to iOS's +layerClass, called once wantsLayer becomes YES.
- (CALayer*)makeBackingLayer
{
    return [CAMetalLayer layer];
}

- (instancetype)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        // NSViews are not layer-backed by default. This is what triggers makeBackingLayer.
        self.wantsLayer = YES;
        self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;
    }
    return self;
}

- (BOOL)wantsUpdateLayer
{
    return YES;
}

// A window can move to a display of different scale without changing size, so this is a
// separate event from setFrameSize:.
- (void)viewDidChangeBackingProperties
{
    [super viewDidChangeBackingProperties];
    [self updateDrawableSize];
}

- (void)setFrameSize:(NSSize)newSize
{
    [super setFrameSize:newSize];
    [self updateDrawableSize];
}

- (void)updateDrawableSize
{
    const CGFloat scale = self.window ? self.window.backingScaleFactor : 1.0;
    self.layer.contentsScale = scale;

    // bounds is in points, the drawable in pixels.
    const NSSize points = self.bounds.size;
    jpt::ResizeRenderer(static_cast<std::uint32_t>(points.width * scale),
                        static_cast<std::uint32_t>(points.height * scale));
}

- (void)startRenderLoop
{
    // Silently never fires unless added to a run loop, and deallocs unless strongly held.
    self.startTime = CACurrentMediaTime();
    self.displayLink = [self displayLinkWithTarget:self selector:@selector(onFrame:)];
    [self.displayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];
}

- (void)stopRenderLoop
{
    [self.displayLink invalidate];
    self.displayLink = nil;
}

- (void)onFrame:(CADisplayLink*)sender
{
    (void)sender;
    jpt::DrawFrame(CACurrentMediaTime() - self.startTime);
}

@end

@interface JupiterAppDelegate : NSObject <NSApplicationDelegate>
@end

namespace
{
    NSWindow*           g_window    = nil;
    JupiterAppDelegate* g_delegate  = nil;
    JupiterMetalView*   g_metalView = nil;

    void BuildMenuBar()
    {
        NSMenu* menuBar = [[NSMenu alloc] init];

        NSMenuItem* appMenuItem = [[NSMenuItem alloc] init];
        [menuBar addItem:appMenuItem];

        NSMenu* appMenu = [[NSMenu alloc] init];

        NSString* appName = [[NSProcessInfo processInfo] processName];
        NSString* quitTitle = [@"Quit " stringByAppendingString:appName];

        NSMenuItem* quitItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                          action:@selector(terminate:)
                                                   keyEquivalent:@"q"];
        [appMenu addItem:quitItem];
        [appMenuItem setSubmenu:appMenu];

        // Test top menu
        NSMenuItem* testMenuItem = [[NSMenuItem alloc] init];
        [menuBar addItem:testMenuItem];

        NSMenu* testMenu = [[NSMenu alloc] initWithTitle:@"Test"];
        [testMenuItem setSubmenu:testMenu];

        [NSApp setMainMenu:menuBar];
    }
}

@implementation JupiterAppDelegate

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)app
{
    (void)app;
    return YES;
}

// Unlike the code after Run() in main, this does run: AppKit posts it before exit().
- (void)applicationWillTerminate:(NSNotification*)notification
{
    (void)notification;
    [g_metalView stopRenderLoop];
    jpt::ShutdownRenderer();
}

@end

namespace jpt
{
    bool CreateAppWindow(std::int32_t width, std::int32_t height, const char* title)
    {
        [NSApplication sharedApplication];

        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        g_delegate = [[JupiterAppDelegate alloc] init];
        [NSApp setDelegate:g_delegate];

        BuildMenuBar();

        const NSRect contentRect = NSMakeRect(0.0, 0.0, width, height);
        const NSWindowStyleMask style = NSWindowStyleMaskTitled
                                      | NSWindowStyleMaskClosable
                                      | NSWindowStyleMaskMiniaturizable
                                      | NSWindowStyleMaskResizable;

        g_window = [[NSWindow alloc] initWithContentRect:contentRect
                                               styleMask:style
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
        if (g_window == nil)
        {
            return false;
        }

        [g_window setReleasedWhenClosed:NO];

        [g_window setTitle:[NSString stringWithUTF8String:title]];
        [g_window center];

        g_metalView = [[JupiterMetalView alloc] initWithFrame:contentRect];
        [g_window setContentView:g_metalView];

        [g_window makeKeyAndOrderFront:nil];

        [NSApp activate];

        if (!jpt::InitRenderer((__bridge void*)g_metalView.layer))
        {
            NSLog(@"Jupiter: Metal renderer failed to initialise");
            return false;
        }

        [g_metalView updateDrawableSize];
        [g_metalView startRenderLoop];

        return true;
    }

    void RunAppLoop([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
    {
        [NSApp run];
    }
}

#endif // IS_PLATFORM_MACOS
