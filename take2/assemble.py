#!/usr/bin/env python

import sys

INSTRUCTION_MAPPINGS = {
    'noop': 0,
    'halt': 1,
    'push10': 10,
    'push20': 20,
    'print': 9,
}

with open(sys.argv[1], 'r') as f:
    instruction_codes = [INSTRUCTION_MAPPINGS[l.strip()] for l in f.readlines()]

with open(sys.argv[1] + '.cb', 'wb') as f:
    f.write(bytes(instruction_codes))
