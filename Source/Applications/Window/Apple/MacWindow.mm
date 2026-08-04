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

// Defined with the rest of the cursor state below; the delegate above the namespace needs it.
namespace
{
    void ApplyCursorCapture(bool captured);
}

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

// Cmd-Tabbing away mid-look would otherwise leave the pointer hidden and pinned system-wide,
// with the button-up that would release it delivered to somebody else.
- (void)applicationWillResignActive:(NSNotification*)notification
{
    (void)notification;
    ApplyCursorCapture(false);
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
    NSMutableDictionary<id<NSObject, NSCopying>, NSNumber*>* g_pTouchIds = nil;
    std::uint64_t g_nextTouchId = 1;

    // While captured the cursor is hidden and frozen in place, so locationInWindow stops moving
    // and only the event deltas carry the motion. This is the position reported to the engine:
    // real while free, accumulated while captured, so the engine keeps differencing positions and
    // never has to know which mode it is in.
    bool  g_cursorCaptured = false;
    float g_reportedX = 0.0f;
    float g_reportedY = 0.0f;

    /** Process-global, because NSCursor's hide count and the CG association both are. Idempotent:
        AppKit counts hide against unhide, so an unmatched call strands the pointer system-wide. */
    void ApplyCursorCapture(bool captured)
    {
        if (captured == g_cursorCaptured)
        {
            return;
        }

        g_cursorCaptured = captured;

        // Disassociating pins the cursor where it is, so a long look cannot walk it into a screen
        // edge and stall. Deltas keep arriving regardless.
        CGAssociateMouseAndMouseCursorPosition(!captured);

        if (captured)
        {
            [NSCursor hide];
        }
        else
        {
            [NSCursor unhide];
        }
    }

    void ForwardTrackpadTouches(NSEvent* event, NSView* pView)
    {
        NSSet<NSTouch*>* touches = [event touchesMatchingPhase:NSTouchPhaseAny inView:nil];

        NSUInteger touching = 0;
        for (NSTouch* touch in touches)
        {
            if ((touch.phase & NSTouchPhaseTouching) != 0)
            {
                ++touching;
            }
        }

        if (touching < 2)
        {
            for (NSNumber* id in g_pTouchIds.allValues)
            {
                jpt::OnTouchEnded(id.unsignedLongLongValue, 0.0f, 0.0f, event.timestamp);
            }
            [g_pTouchIds removeAllObjects];
            return;
        }

        const CGFloat backing = pView.window ? pView.window.backingScaleFactor : 1.0;
        const CGFloat viewHeight = pView.bounds.size.height * backing;

        for (NSTouch* touch in touches)
        {
            const NSSize device = touch.deviceSize;
            if (device.height < 1.0)
            {
                continue;
            }

            const CGFloat perPoint = viewHeight / device.height;
            const float x = static_cast<float>(touch.normalizedPosition.x * device.width * perPoint);
            const float y = static_cast<float>((1.0 - touch.normalizedPosition.y) * viewHeight);

            NSNumber* existing = [g_pTouchIds objectForKey:touch.identity];

            if ((touch.phase & (NSTouchPhaseEnded | NSTouchPhaseCancelled)) != 0)
            {
                if (existing)
                {
                    jpt::OnTouchEnded(existing.unsignedLongLongValue, x, y, event.timestamp);
                    [g_pTouchIds removeObjectForKey:touch.identity];
                }
            }
            else if (existing)
            {
                jpt::OnTouchMoved(existing.unsignedLongLongValue, x, y, event.timestamp);
            }
            else
            {
                const std::uint64_t touchId = g_nextTouchId++;
                [g_pTouchIds setObject:@(touchId) forKey:touch.identity];
                jpt::OnTouchBegan(touchId, x, y, event.timestamp);
            }
        }
    }

    id InstallEventMonitor(NSView* pView)
    {
        // Raw trackpad touches ride on NSEventTypeGesture, which a monitor can see -- the responder
        // methods would not fire, since imgui_impl_osx holds first responder.
        pView.allowedTouchTypes = NSTouchTypeMaskIndirect;
        g_pTouchIds = [NSMutableDictionary dictionary];

        const NSEventMask mask = NSEventMaskGesture
                               | NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged
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
            case NSEventTypeGesture:
                ForwardTrackpadTouches(event, pView);
                break;

            case NSEventTypeKeyDown:
                jpt::OnKeyDown(event.keyCode, event.isARepeat);
                break;

            case NSEventTypeKeyUp:
                jpt::OnKeyUp(event.keyCode);
                break;

            case NSEventTypeFlagsChanged:
                jpt::OnModifierChanged(event.keyCode, static_cast<std::uint32_t>(event.modifierFlags));
                break;

            case NSEventTypeLeftMouseDown:
            case NSEventTypeRightMouseDown:
            case NSEventTypeOtherMouseDown:
                jpt::OnMouseButton(static_cast<std::int32_t>(event.buttonNumber), true, x, y);
                break;

            case NSEventTypeLeftMouseUp:
            case NSEventTypeRightMouseUp:
            case NSEventTypeOtherMouseUp:
                jpt::OnMouseButton(static_cast<std::int32_t>(event.buttonNumber), false, x, y);
                break;

            case NSEventTypeMouseMoved:
            case NSEventTypeLeftMouseDragged:
            case NSEventTypeRightMouseDragged:
            case NSEventTypeOtherMouseDragged:
                if (g_cursorCaptured)
                {
                    // Measured, not assumed: a warp of +80 CG-x and +80 CG-y reports deltaX +80
                    // and deltaY +80, so both already run the way this engine's pixels do.
                    g_reportedX += static_cast<float>(event.deltaX * scale);
                    g_reportedY += static_cast<float>(event.deltaY * scale);
                }
                else
                {
                    g_reportedX = x;
                    g_reportedY = y;
                }
                jpt::OnMouseMove(g_reportedX, g_reportedY);
                break;

            case NSEventTypeScrollWheel:
                jpt::OnMouseScroll(static_cast<float>(event.scrollingDeltaX), 
                                   static_cast<float>(event.scrollingDeltaY), 
                                   event.hasPreciseScrollingDeltas);
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

    void MacWindow::SetCursorCaptured(bool captured)
    {
        ApplyCursorCapture(captured);
    }

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
