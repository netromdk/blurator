# -*- coding: utf-8 -*-
import sys, os
from util import *

DEP_LIST = []

def usage():
    print("usage: {} <binary> <dest>".format(sys.argv[0]))

def handleDep(dep, dst):
    newfile = copy(dep, dst)

    if newfile in DEP_LIST:
        return
    DEP_LIST.append(newfile)

    # Handle its deps
    deps = getDeps(newfile)
    for dep_ in deps:
        handleDep(dep_, dst)

    # Localize the dep's shared library paths to the executable path.
    localizeDep(newfile)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        usage()
        exit(-1)

    dep = sys.argv[1]
    dst = sys.argv[2]
    handleDep(dep, dst)

    for dep in DEP_LIST:
        localizeDep(dep)
