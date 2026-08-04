// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS

#include "MacWindow.h"
#include "AppleCallbacks.h"
#include "Graphics/ImGui/ImGuiLayer.h"

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
    self.displayLink = [self displayLinkWithTarget:self selector:@selector(OnFrame:)];
    [self.displayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];
}

- (void)stopRenderLoop
{
    [self.displayLink invalidate];
    self.displayLink = nil;
}

- (void)OnFrame:(CADisplayLink*)sender
{
    (void)sender;
    jpt::OnFrame();
}

@end

// weak, not strong: the view is owned by the window's content-view hierarchy.
@interface JupiterAppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, weak) JupiterMetalView* metalView;
@end

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
    [self.metalView stopRenderLoop];
    jpt::OnTerminate();
}

@end

namespace
{
    /** A monitor, not responder overrides: imgui_impl_osx makes its own KeyEventResponder the
        window's first responder, so a keyDown: override here would never fire. Returns the event,
        so ImGui still sees it -- hence the ImGuiWants* gate. */
    id InstallEventMonitor(NSView* pView)
    {
        const NSEventMask mask = NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged
                               | NSEventMaskLeftMouseDown  | NSEventMaskLeftMouseUp
                               | NSEventMaskRightMouseDown | NSEventMaskRightMouseUp
                               | NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp
                               | NSEventMaskMouseMoved
                               | NSEventMaskLeftMouseDragged | NSEventMaskRightMouseDragged
                               | NSEventMaskOtherMouseDragged
                               | NSEventMaskScrollWheel;

        return [NSEvent addLocalMonitorForEventsMatchingMask:mask
                                                     handler:^NSEvent* _Nullable(NSEvent* event)
        {
            // locationInWindow is points, bottom-left. Flip to top-left and scale to pixels
            // once, here: the engine measures the viewport in pixels, and a drag delta is only
            // meaningful against it in the same unit.
            const NSPoint inView = [pView convertPoint:event.locationInWindow fromView:nil];
            const CGFloat scale = pView.window ? pView.window.backingScaleFactor : 1.0;
            const float x = static_cast<float>(inView.x * scale);
            const float y = static_cast<float>((pView.bounds.size.height - inView.y) * scale);

            switch (event.type)
            {
            case NSEventTypeKeyDown:
                if (!jpt::ImGuiWantsKeyboard())
                {
                    jpt::OnKeyDown(event.keyCode, event.isARepeat);
                }
                break;

            // Releases are never gated. A release is bookkeeping, not input to compete over: drop
            // one because ImGui took focus mid-press and the key stays down forever, which a
            // polled IsKeyDown reads as held. Presses stay gated, so typing still cannot drive
            // the game.
            case NSEventTypeKeyUp:
                jpt::OnKeyUp(event.keyCode);
                break;

            case NSEventTypeFlagsChanged:
                jpt::OnModifierChanged(event.keyCode, static_cast<std::uint32_t>(event.modifierFlags));
                break;

            case NSEventTypeLeftMouseDown:
            case NSEventTypeRightMouseDown:
            case NSEventTypeOtherMouseDown:
                if (!jpt::ImGuiWantsMouse())
                {
                    jpt::OnMouseButton(static_cast<std::int32_t>(event.buttonNumber), true, x, y);
                }
                break;

            case NSEventTypeLeftMouseUp:
            case NSEventTypeRightMouseUp:
            case NSEventTypeOtherMouseUp:
                if (!jpt::ImGuiWantsMouse())
                {
                    jpt::OnMouseButton(static_cast<std::int32_t>(event.buttonNumber), false, x, y);
                }
                break;

            case NSEventTypeMouseMoved:
            case NSEventTypeLeftMouseDragged:
            case NSEventTypeRightMouseDragged:
            case NSEventTypeOtherMouseDragged:
                if (!jpt::ImGuiWantsMouse())
                {
                    jpt::OnMouseMove(x, y);
                }
                break;

            case NSEventTypeScrollWheel:
                if (!jpt::ImGuiWantsMouse())
                {
                    jpt::OnMouseScroll(static_cast<float>(event.scrollingDeltaX),
                                       static_cast<float>(event.scrollingDeltaY),
                                       event.hasPreciseScrollingDeltas);
                }
                break;

            default:
                break;
            }

            return event;
        }];
    }

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

namespace jpt
{
    struct MacWindow::Impl
    {
        NSWindow*           pWindow   = nil;
        JupiterAppDelegate* pDelegate = nil;
        JupiterMetalView*   pView     = nil;
        id                  pMonitor  = nil;
    };

    bool MacWindow::PreInit([[maybe_unused]] std::int32_t argc, [[maybe_unused]] char* ppArgv[])
    {
        m_pImpl = new Impl();

        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        m_pImpl->pDelegate = [[JupiterAppDelegate alloc] init];
        [NSApp setDelegate:m_pImpl->pDelegate];

        BuildMenuBar();

        return true;
    }

    bool MacWindow::Init()
    {
        const NSRect contentRect = NSMakeRect(0.0, 0.0, 1280.0, 720.0);
        const NSWindowStyleMask style = NSWindowStyleMaskTitled
                                      | NSWindowStyleMaskClosable
                                      | NSWindowStyleMaskMiniaturizable
                                      | NSWindowStyleMaskResizable;

        m_pImpl->pWindow = [[NSWindow alloc] initWithContentRect:contentRect
                                                       styleMask:style
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        if (m_pImpl->pWindow == nil)
        {
            return false;
        }

        [m_pImpl->pWindow setReleasedWhenClosed:NO];
        [m_pImpl->pWindow setTitle:@"Jupiter Engine"];
        [m_pImpl->pWindow center];

        m_pImpl->pView = [[JupiterMetalView alloc] initWithFrame:contentRect];
        m_pImpl->pDelegate.metalView = m_pImpl->pView;

        // Resizes the view to fill the window, so OnResize fires before the layer exists.
        [m_pImpl->pWindow setContentView:m_pImpl->pView];

        // Without this, NSEventMaskMouseMoved is never generated at all.
        [m_pImpl->pWindow setAcceptsMouseMovedEvents:YES];

        [m_pImpl->pWindow makeKeyAndOrderFront:nil];
        [NSApp activate];

        // metal-cpp types are typed views over the same ObjC object, so the bridge to void*
        // and back is how a CAMetalLayer* becomes a CA::MetalLayer*.
        if (!jpt::OnSurfaceReady(reinterpret_cast<CA::MetalLayer*>((__bridge void*)m_pImpl->pView.layer)))
        {
            return false;
        }

        // After OnSurfaceReady, which is where the ImGui context is created. The AppKit backend
        // installs its own NSEvent monitors on this view, so no engine input path is involved.
        jpt::ImGuiInitPlatform((__bridge void*)m_pImpl->pView);

        // After ImGuiInitPlatform: the ImGuiWants* gate needs a context.
        m_pImpl->pMonitor = InstallEventMonitor(m_pImpl->pView);

        // No resize event fires for the initial size.
        [m_pImpl->pView updateDrawableSize];
        [m_pImpl->pView startRenderLoop];

        return true;
    }

    void MacWindow::Run()
    {
        // Never returns: AppKit owns the loop and exits the process on terminate:.
        [NSApp run];
    }

    void MacWindow::Terminate()
    {
        if (m_pImpl->pMonitor != nil)
        {
            [NSEvent removeMonitor:m_pImpl->pMonitor];
            m_pImpl->pMonitor = nil;
        }

        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

#endif // IS_PLATFORM_MACOS
