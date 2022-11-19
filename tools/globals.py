VMAJOR = 1
VMINOR = 0
VPATCH = 1


import sys , platform

for x in platform.name():
    if "microsoft" in x.lower():
        PLATFORM = "windows"
        break

def IsWindows():
    return PLATFORM == "windows"

def IsLinux():
    return PLATFORM == "linux"

def IsMac():
    return PLATFORM == "darwin"
