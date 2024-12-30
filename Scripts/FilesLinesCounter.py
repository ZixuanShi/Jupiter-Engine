# Counts how many files and lines of code are in Jupiter Engine

import os

extensions = {"h", "cpp", "ixx", "py", "lua", "bat", "hlsl", "glsl"}

files_count = 0
lines_count = 0

def walk(directory):
    global files_count
    global lines_count

    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(tuple(extensions)):
                files_count += 1
                file_path = os.path.join(root, file)  # Use os.path.join for cross-platform compatibility
                try:
                    # Try UTF-8 first
                    with open(file_path, "r", encoding="utf-8") as f:
                        line_count = len(f.readlines())
                        lines_count += line_count
                except UnicodeDecodeError:
                    try:
                        # Fallback to cp1252
                        with open(file_path, "r", encoding="cp1252") as f:
                            line_count = len(f.readlines())
                            lines_count += line_count
                    except UnicodeDecodeError:
                        print(f"Warning: Could not read {file_path} - encoding issues")

if __name__ == "__main__":
    directory = os.environ["JUPITER_ENGINE_DIR"]

    walk(directory + "Source")
    walk(directory + "Scripts")
    walk(directory + "Projects")

    print("Files: " + str(files_count))
    print("Lines: " + str(lines_count))
