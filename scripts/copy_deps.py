# -*- coding: utf-8 -*-
import sys, os, re, shutil

CLEAN_REGEX = re.compile("(.+)\s*\(.+\)", re.IGNORECASE)
IGNORE = ["libobjc.A.dylib", "libSystem.B.dylib", "/System/Library/Frameworks",
          "/usr/lib/system/"]
DEP_LIST = []

def usage():
    print("usage: {} <binary> <dest>".format(sys.argv[0]))

def checkIgnore(path):
    for elm in IGNORE:
        if path.find(elm) != -1:
            return True
    return False
    
def clean(path):
    res = CLEAN_REGEX.findall(path.strip())
    if len(res) == -1:
        return None
    res = res[0].strip()
    if checkIgnore(res):
        return None
    return res

def getDeps(bin):
    lines = os.popen("otool -L %s" % bin).readlines()
    deps = []
    for dep in lines[1:]:
        dep = clean(dep)
        if dep: deps.append(dep)
    return deps

def copy(dep, dst):
    #print("Copying {}".format(dep))
    newfile = dst + os.path.sep + os.path.basename(dep)
    if os.path.exists(newfile):
        return newfile
    shutil.copyfile(dep, newfile)
    return newfile

def getIdLibs(lib, hasId = True):
    lines = os.popen("otool -L %s" % lib).readlines()
    id = clean(lines[1])
    libs = []
    if not hasId:
        libs.append(id)
        id = None
    for dep in lines[2:]:
        dep = clean(dep)
        if dep: libs.append(dep)
    return [id, libs]

def otoolID(path, id):
    #print("otool id {} on {}".format(id, path))
    os.popen("install_name_tool -id %s %s" % (id, path))

def otoolChange(path, old, new):
    #print("otool change {} -> {} on {}".format(old, new, path))
    os.popen("install_name_tool -change %s %s %s" % (old, new, path))

def fixEntry(entry):
    return "@executable_path/{}".format(os.path.basename(entry))

def localizeDep(dep, dst, hasId):
    [id, libs] = getIdLibs(dep, hasId)
    if id:
        otoolID(dep, fixEntry(id))
    for lib in libs:
        otoolChange(dep, lib, fixEntry(lib))

def handleDep(dep, dst, hasId = True):
    bname = os.path.basename(dep)
    if bname in DEP_LIST:
        return
    DEP_LIST.append(bname)
        
    newfile = copy(dep, dst)

    # Handle its deps
    deps = getDeps(newfile)
    for dep_ in deps:
        handleDep(dep_, dst)

    # Localize the dep's shared library paths to the destination
    # folder.
    localizeDep(newfile, dst, hasId)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        usage()
        exit(-1)

    dep = sys.argv[1]
    dst = sys.argv[2]
    handleDep(dep, dst, False)
