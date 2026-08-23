#!/usr/bin/env python3
import os
import shutil
import subprocess
import sys

from utils import ROOT, active_preset, artifact_path, project_dir


def package_apk(preset) -> int:
    """Run gradle over the prebuilt jniLibs and land the APK in _Output.

    Gradle never sees native code -- CMake already wrote libmain.so into the gradle project's
    jniLibs -- so this is javac, the manifest, debug signing and 16 KB alignment. The debug-signed
    APK is the artifact for every preset: the native config is baked into libmain.so, and a
    release APK would only add a signing config nobody has yet.
    """
    android_dir = project_dir() / "_ProjectFiles" / "android"
    wrapper = android_dir / ("gradlew.bat" if os.name == "nt" else "gradlew")

    environment = {**os.environ}
    environment.setdefault("JAVA_HOME", r"C:\Program Files\Android\Android Studio\jbr")

    result = subprocess.run([str(wrapper), "assembleDebug"], cwd=android_dir, env=environment)
    if result.returncode != 0:
        return result.returncode

    apk = android_dir / "app" / "build" / "outputs" / "apk" / "debug" / "app-debug.apk"
    destination = artifact_path(preset)
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(apk, destination)
    print(f"Packaged {destination}")
    return 0


def main():
    # A build preset resolves its binaryDir through the configure preset, which reads this.
    preset = active_preset()
    environment = {**os.environ, "JUPITER_PROJECT_DIR": str(project_dir())}
    result = subprocess.run(["cmake", "--build", "--preset", preset], cwd=ROOT, env=environment)
    if result.returncode != 0:
        sys.exit(result.returncode)

    if preset.startswith("android"):
        sys.exit(package_apk(preset))


if __name__ == "__main__":
    main()
