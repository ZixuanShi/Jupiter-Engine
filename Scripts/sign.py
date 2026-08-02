#!/usr/bin/env python3
# macOS ships Python 3.9, where `tuple | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import plistlib
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

from utils import BUNDLE_ID, ROOT, active_preset, artifact_path

PROFILE_DIR = Path.home() / "Library/Developer/Xcode/UserData/Provisioning Profiles"
ENTITLEMENTS = ROOT / "_ProjectFiles" / "JupiterEngine.entitlements"

# Substring match. With one certificate in the keychain it is unambiguous, and it keeps the
# developer's name and email out of the repository.
IDENTITY = "Apple Development"

REMINT = """No valid provisioning profile for {bundle}.

Free personal-team profiles expire after 7 days. To mint a fresh one, open the throwaway
Xcode project whose bundle identifier is {bundle} and run it once on the device."""


def decode(profile) -> dict:
    """Return the plist embedded in a CMS-signed .mobileprovision."""
    result = subprocess.run(["security", "cms", "-D", "-i", str(profile)],
                            capture_output=True)
    return plistlib.loads(result.stdout)


def find_profile() -> tuple | None:
    """Return the newest unexpired (path, plist) provisioning BUNDLE_ID, or None."""
    # Profile dates are naive UTC, so compare against a naive UTC now.
    now = datetime.now(timezone.utc).replace(tzinfo=None)
    best = None

    for path in PROFILE_DIR.glob("*.mobileprovision"):
        info = decode(path)
        app_id = info.get("Entitlements", {}).get("application-identifier", "")

        if not app_id.endswith(f".{BUNDLE_ID}") or info["ExpirationDate"] < now:
            continue
        if best is None or info["CreationDate"] > best[1]["CreationDate"]:
            best = (path, info)

    return best


def sign(app) -> int:
    """Embed the provisioning profile in the bundle and code-sign it."""
    found = find_profile()
    if found is None:
        print(REMINT.format(bundle=BUNDLE_ID))
        return 1

    profile, info = found
    shutil.copy(profile, app / "embedded.mobileprovision")

    # The profile already carries exactly the entitlements the signature must claim, so
    # deriving them here means the two can never drift out of sync.
    ENTITLEMENTS.write_bytes(plistlib.dumps(info["Entitlements"]))

    print(f"Signing {app.name} with {info['Name']} (expires {info['ExpirationDate']:%Y-%m-%d})")
    return subprocess.run(["codesign", "--force", "--timestamp=none",
                           "--sign", IDENTITY,
                           "--entitlements", str(ENTITLEMENTS),
                           str(app)]).returncode


def main():
    if len(sys.argv) > 2:
        print("Usage: py Scripts/sign.py [path/to/App.app]")
        sys.exit(1)

    # Defaults to the configured preset so the bundle can be re-signed after a profile
    # expires, when nothing has changed and the build itself is a no-op.
    app = Path(sys.argv[1]) if len(sys.argv) == 2 else artifact_path(active_preset())

    if not app.is_dir():
        print(f"Not built: {app}")
        print("Run: py Scripts/build.py")
        sys.exit(1)

    sys.exit(sign(app))


if __name__ == "__main__":
    main()
