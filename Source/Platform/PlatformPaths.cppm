// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.PlatformPaths;

import std;

export namespace jpt
{
    /** Directories only the operating system can name. Everything here is a root that jpt::Path
        composes against, which is why Path itself needs no platform knowledge. */

    /** The one root Jupiter resolves relative paths against: the repo directory in a source
        tree, and the app bundle's resource directory once installed. */
    [[nodiscard]] const std::filesystem::path& GetRoot();

    /** The repository root, and the active project's directory under Projects/. Source-tree
        locations, so both are empty in a Release or mobile build, which ships to a machine that
        has no repo -- test what you get before composing against it. */
    [[nodiscard]] const std::filesystem::path& GetEngineDir();
    [[nodiscard]] const std::filesystem::path& GetProjectDir();

    /** Writable per-user storage: editor preferences, logs. Deliberately not under GetRoot(),
        which is read-only in an installed app and, once signed, must not change at all. The
        directory is created on first call. */
    [[nodiscard]] const std::filesystem::path& GetSavedDir();
}
