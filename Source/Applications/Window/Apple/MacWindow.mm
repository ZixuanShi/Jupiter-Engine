// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS

#import <Cocoa/Cocoa.h>

#include "AppleWindow.h"

@interface JupiterAppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation JupiterAppDelegate

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)app
{
    return YES;
}

@end

namespace
{
    NSWindow*           g_window   = nil;
    JupiterAppDelegate* g_delegate = nil;

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
        [g_window makeKeyAndOrderFront:nil];

        [NSApp activate];

        return true;
    }

    void RunAppLoop()
    {
        [NSApp run];
    }
}

#endif // IS_PLATFORM_MACOS
