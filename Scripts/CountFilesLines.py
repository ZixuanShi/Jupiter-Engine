# Copyright Jupiter Technologies, Inc. All Rights Reserved.
# Counts how many files and lines of code are in Jupiter Engine

import os

extensions = {"h", "cpp", "ixx", "py", "lua", "bat", "hlsl", "glsl"}

g_filesCount = 0
g_linesCount = 0

def walk(directory: str) -> None:
    global g_filesCount
    global g_linesCount

    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(tuple(extensions)):
                g_filesCount += 1
                filePath = os.path.join(root, file)  # Use os.path.join for cross-platform compatibility
                try:
                    # Try UTF-8 first
                    with open(filePath, "r", encoding="utf-8") as f:
                        lineCount = len(f.readlines())
                        g_linesCount += lineCount
                except UnicodeDecodeError:
                    try:
                        # Fallback to cp1252
                        with open(filePath, "r", encoding="cp1252") as f:
                            lineCount = len(f.readlines())
                            g_linesCount += lineCount
                    except UnicodeDecodeError:
                        print(f"Warning: Could not read {filePath} - encoding issues")

if __name__ == "__main__":
    directory = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")

    walk(directory + "/Source")
    walk(directory + "/Scripts")
    walk(directory + "/Projects")

    print("Files: " + str(g_filesCount))
    print("Lines: " + str(g_linesCount))
    os.system("pause")
