// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS

#include "AppleWindow.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <QuartzCore/CADisplayLink.h>

@interface JupiterMetalView : NSView
@property (nonatomic, strong) CADisplayLink* displayLink;
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
    jpt::OnResize(static_cast<std::uint32_t>(points.width * scale),
                  static_cast<std::uint32_t>(points.height * scale));
}

- (void)startRenderLoop
{
    // Silently never fires unless added to a run loop, and deallocs unless strongly held.
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
    jpt::OnFrameDraw();
}

@end

@interface JupiterAppDelegate : NSObject <NSApplicationDelegate>
@end

namespace
{
    NSWindow*           g_pWindow    = nil;
    JupiterAppDelegate* g_pDelegate  = nil;
    JupiterMetalView*   g_pMetalView = nil;

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
    [g_pMetalView stopRenderLoop];
    jpt::OnTerminate();
}

@end

namespace jpt
{
    bool CreateAppWindow(std::int32_t width, std::int32_t height, const char* title)
    {
        [NSApplication sharedApplication];

        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        g_pDelegate = [[JupiterAppDelegate alloc] init];
        [NSApp setDelegate:g_pDelegate];

        BuildMenuBar();

        const NSRect contentRect = NSMakeRect(0.0, 0.0, width, height);
        const NSWindowStyleMask style = NSWindowStyleMaskTitled
                                      | NSWindowStyleMaskClosable
                                      | NSWindowStyleMaskMiniaturizable
                                      | NSWindowStyleMaskResizable;

        g_pWindow = [[NSWindow alloc] initWithContentRect:contentRect
                                               styleMask:style
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
        if (g_pWindow == nil)
        {
            return false;
        }

        [g_pWindow setReleasedWhenClosed:NO];

        [g_pWindow setTitle:[NSString stringWithUTF8String:title]];
        [g_pWindow center];

        g_pMetalView = [[JupiterMetalView alloc] initWithFrame:contentRect];
        [g_pWindow setContentView:g_pMetalView];

        [g_pWindow makeKeyAndOrderFront:nil];

        [NSApp activate];

        if (!jpt::OnSurfaceReady((__bridge void*)g_pMetalView.layer))
        {
            return false;
        }

        [g_pMetalView updateDrawableSize];
        [g_pMetalView startRenderLoop];

        return true;
    }

    void RunAppLoop([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
    {
        [NSApp run];
    }
}

#endif // IS_PLATFORM_MACOS
