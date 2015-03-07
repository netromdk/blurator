# -*- coding: utf-8 -*-
import sys
from util import *

def usage():
    print("usage: {} <binary>".format(sys.argv[0]))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage()
        exit(-1)

    binary = sys.argv[1]
    hasId = hasSuffix(binary)
    localizeDep(binary, hasId)
