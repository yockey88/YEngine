PROJECT_NAME = "MachineY"
ENGINE_NAME = "Engine"

TOOLS_DIR = "tools"

VMAJOR = 1
VMINOR = 0
VPATCH = 1

CONFIG = "debug"
CONFIGEXE = "Debug"

import sys , platform
PLATFORM = sys.platform
for x in platform.uname():
    if "microsoft" in x.lower():
        PLATFORM = "windows"
        break

def IsWindows():
    return PLATFORM == "windows"

def IsLinux():
    return PLATFORM == "linux"

def IsMac():
    return PLATFORM == "darwin"
