# Deletes all generated folders in Jupiter Engine

import os
import shutil

to_remove = ["_Output", "_ProjectFiles", "_Saved", "_Baked"]

def Clean(root):
    print("Cleaning " + root)
    for folder in to_remove:
        if os.path.exists(root + folder):
            print("Removing " + root + folder)
            shutil.rmtree(root + folder)

if __name__ == "__main__":
    # Jupiter Root
    jupiter_root = os.getenv("JUPITER_ENGINE_DIR")
    if jupiter_root is None:
        jupiter_root = "C:/Program Files/Jupiter Technologies/Jupiter-Engine/"

    Clean(jupiter_root)

    # Projects
    projects = os.listdir(jupiter_root + "Projects")
    for project in projects:
        project_path = jupiter_root + "Projects/" + project + "/"
        Clean(project_path)

    print("Finished cleaning Jupiter Engine")