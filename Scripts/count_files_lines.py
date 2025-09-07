# Copyright Jupiter Technologies, Inc. All Rights Reserved.
# Counts how many files and lines of code are in Jupiter Engine

import os

EXTENSIONS = {"h", "cpp", "cppm", "py", "lua", "bat", "hlsl", "glsl"}

g_files_count = 0
g_lines_count = 0

def walk(directory: str) -> None:
    global g_files_count
    global g_lines_count

    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(tuple(EXTENSIONS)):
                g_files_count += 1
                file_path = os.path.join(root, file)  # Use os.path.join for cross-platform compatibility
                try:
                    # Try UTF-8 first
                    with open(file_path, "r", encoding="utf-8") as f:
                        line_count = len(f.readlines())
                        g_lines_count += line_count
                except UnicodeDecodeError:
                    try:
                        # Fallback to cp1252
                        with open(file_path, "r", encoding="cp1252") as f:
                            line_count = len(f.readlines())
                            g_lines_count += line_count
                    except UnicodeDecodeError:
                        print(f"Warning: Could not read {file_path} - encoding issues")

if __name__ == "__main__":
    directory = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")

    walk(directory + "/Source")
    walk(directory + "/Scripts")
    walk(directory + "/Projects")

    print("Files: " + str(g_files_count))
    print("Lines: " + str(g_lines_count))
