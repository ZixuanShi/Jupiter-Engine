# Copyright Jupiter Technologies, Inc. All Rights Reserved.
# Deletes all generated temp folders in Jupiter Engine

import os
import shutil

TO_DELETE = ["_Output", "_ProjectFiles", "_Saved", "_Baked"]

def clean(root: str) -> None:
    root += "/"
    print("Cleaning " + root)

    for folder in TO_DELETE:
        if os.path.exists(root + folder):
            print("Removing " + root + folder)
            shutil.rmtree(root + folder)

if __name__ == "__main__":
    jupiterRoot = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")

    clean(jupiterRoot)

    # Projects
    projects = os.listdir(jupiterRoot + "/Projects")
    for project in projects:
        projectPath = jupiterRoot + "/Projects/" + project
        clean(projectPath)

    print("Finished cleaning Jupiter Engine")
