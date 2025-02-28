# Deletes all generated temp folders in Jupiter Engine

import os
import shutil

to_delete = ["_Output", "_ProjectFiles", "_Saved", "_Baked"]

def clean(root):
    print("Cleaning " + root)
    for folder in to_delete:
        if os.path.exists(root + folder):
            print("Removing " + root + folder)
            shutil.rmtree(root + folder)

if __name__ == "__main__":
    # Jupiter Root
    jupiter_root = os.getenv("JUPITER_ENGINE_DIR")
    if jupiter_root is None:
        jupiter_root = "C:/Program Files/Jupiter Technologies/Jupiter-Engine/"

    clean(jupiter_root)

    # Projects
    projects = os.listdir(jupiter_root + "Projects")
    for project in projects:
        project_path = jupiter_root + "Projects/" + project + "/"
        clean(project_path)

    print("Finished cleaning Jupiter Engine")