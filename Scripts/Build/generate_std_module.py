"""Generate Vendor/LibCxxModules from a libcxx/modules checkout.

Replicates what libcxx/modules/CMakeLists.txt does at install time: fills the
@LIBCXX_MODULE_*_INCLUDE_SOURCES@ token in std.cppm.in/std.compat.cppm.in with one #include per
.inc in the CMakeLists.txt list order, copies the .inc trees, and writes a libc++.modules.json
with paths relative to the json. See Vendor/LibCxxModules/README.md for when to run this.
"""
import re
import shutil
import sys
from pathlib import Path


def parse_list(cmake_text: str, name: str) -> list:
    """Return the entries of a set(<name> ...) list in declaration order."""
    block = re.search(rf"set\({name}\n(.*?)\n\)", cmake_text, re.DOTALL).group(1)
    return [line.strip() for line in block.splitlines() if line.strip()]


def main() -> int:
    if len(sys.argv) != 3:
        print("Usage: py Scripts/Build/generate_std_module.py <libcxx-modules-dir> <output-dir>")
        return 1

    src = Path(sys.argv[1])
    dst = Path(sys.argv[2])
    cmake_text = (src / "CMakeLists.txt").read_text()

    v1 = dst / "v1"
    v1.mkdir(parents=True, exist_ok=True)

    for module, list_name, token in (
        ("std", "LIBCXX_MODULE_STD_SOURCES", "LIBCXX_MODULE_STD_INCLUDE_SOURCES"),
        ("std.compat", "LIBCXX_MODULE_STD_COMPAT_SOURCES",
         "LIBCXX_MODULE_STD_COMPAT_INCLUDE_SOURCES"),
    ):
        includes = "".join(f'#include "{inc}"\n' for inc in parse_list(cmake_text, list_name))
        text = (src / f"{module}.cppm.in").read_text().replace(f"@{token}@", includes)
        (v1 / f"{module}.cppm").write_text(text)
        shutil.copytree(src / module, v1 / module, dirs_exist_ok=True)

    entries = ",\n".join(f"""    {{
      "logical-name": "{module}",
      "source-path": "v1/{module}.cppm",
      "is-std-library": true,
      "local-arguments": {{
        "system-include-directories": [
          "v1"
        ]
      }}
    }}""" for module in ("std", "std.compat"))

    (dst / "libc++.modules.json").write_text(
        f'{{\n  "version": 1,\n  "revision": 1,\n  "modules": [\n{entries}\n  ]\n}}\n')
    print(f"Generated {dst}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
