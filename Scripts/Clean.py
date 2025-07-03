# Copyright Jupiter Technologies, Inc. All Rights Reserved.
# Deletes all generated temp folders in Jupiter Engine

import os
import shutil

kToDelete = ["_Output", "_ProjectFiles", "_Saved", "_Baked"]

def Clean(root: str) -> None:
    root += "/"
    print("Cleaning " + root)

    for folder in kToDelete:
        if os.path.exists(root + folder):
            print("Removing " + root + folder)
            shutil.rmtree(root + folder)

if __name__ == "__main__":
    jupiterRoot = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")

    Clean(jupiterRoot)

    # Projects
    projects = os.listdir(jupiterRoot + "/Projects")
    for project in projects:
        projectPath = jupiterRoot + "/Projects/" + project
        Clean(projectPath)

    print("Finished cleaning Jupiter Engine")