#!/usr/bin/env python3
# macOS ships Python 3.9, where `str | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path

# Scripts/ is the import root for utils, and it is no longer this file's own directory.
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from utils import BUNDLE_ID, ROOT

# /usr/bin/python3 is an xcrun shim, so it exports SDKROOT pointing at the macOS SDK to every
# child. CMake honours SDKROOT over CMAKE_SYSTEM_NAME, configuring this iOS project against
# the host sysroot and then failing with "No CMAKE_C_COMPILER could be found".
IOS_ENV = {key: value for key, value in os.environ.items() if key != "SDKROOT"}

# Ninja cannot ask Apple for a profile, and CMake's Xcode generator cannot build C++ modules.
# So a stub project exists purely to let xcodebuild's automatic signing mint one, which the
# real Ninja build then reuses through sign.py.
PROJECT_DIR = ROOT / "_ProjectFiles" / "provision"

CMAKELISTS = """cmake_minimum_required(VERSION 3.30)
project(JupiterProvision LANGUAGES C)

add_executable(JupiterProvision MACOSX_BUNDLE stub.c)

set_target_properties(JupiterProvision PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "{bundle}"
    MACOSX_BUNDLE_BUNDLE_NAME "JupiterProvision"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_BUNDLE_VERSION "1"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "{bundle}"
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "{team}"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development")
"""

STUB = "int main(int argc, char* argv[]) { (void)argc; (void)argv; return 0; }\n"


def team_id() -> str:
    """Return the Team ID from the signing certificate, or exit if there is none."""
    pem = subprocess.run(["security", "find-certificate", "-c", "Apple Development", "-p"],
                         capture_output=True, text=True).stdout
    subject = subprocess.run(["openssl", "x509", "-noout", "-subject"],
                             input=pem, capture_output=True, text=True).stdout

    # The Organizational Unit of an Apple Development certificate is the Team ID.
    match = re.search(r"OU\s*=\s*([A-Z0-9]+)", subject)
    if match is None:
        print("No 'Apple Development' certificate in the keychain.")
        print("Sign in to Xcode > Settings > Accounts, then try again.")
        sys.exit(1)

    return match.group(1)


def main():
    if len(sys.argv) > 1:
        print("Usage: py Scripts/Build/provision.py")
        sys.exit(1)

    team = team_id()
    PROJECT_DIR.mkdir(parents=True, exist_ok=True)
    (PROJECT_DIR / "stub.c").write_text(STUB)
    (PROJECT_DIR / "CMakeLists.txt").write_text(
        CMAKELISTS.format(bundle=BUNDLE_ID, team=team))

    print(f"Minting a profile for {BUNDLE_ID} (team {team})")

    configure = subprocess.run(
        ["cmake", "-G", "Xcode", "-DCMAKE_SYSTEM_NAME=iOS",
         "-DCMAKE_OSX_DEPLOYMENT_TARGET=17.0", "-B", "build", "."],
        cwd=PROJECT_DIR, env=IOS_ENV, capture_output=True, text=True)
    if configure.returncode != 0:
        print(configure.stderr)
        sys.exit(configure.returncode)

    # -allowProvisioningUpdates is what contacts Apple to register the App ID and device.
    build = subprocess.run(
        ["xcodebuild", "-project", "JupiterProvision.xcodeproj",
         "-target", "JupiterProvision", "-sdk", "iphoneos",
         "-configuration", "Release", "-allowProvisioningUpdates"],
        cwd=PROJECT_DIR / "build", env=IOS_ENV, capture_output=True, text=True)

    for line in build.stdout.splitlines():
        if "Provisioning Profile:" in line:
            print(line.strip())

    if build.returncode != 0:
        print(build.stdout[-2000:])
        print("\nIf this failed to authenticate, sign in at Xcode > Settings > Accounts.")

    sys.exit(build.returncode)


if __name__ == "__main__":
    main()
