# -*- coding: utf-8 -*-
import sys, os
from util import *

def usage():
    print("usage: {} <binary>".format(sys.argv[0]))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage()
        exit(-1)

    binary = sys.argv[1]
    localizeDep(binary)
