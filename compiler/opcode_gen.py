#!/usr/bin/python

import sys

def parse(f):
    got_list = False
    code = 0
    while True:
        line = f.readline()
        if not line:
            break
        if 'core::Opcode' in line and 'class' in line:
            got_list = True
            continue
        elif got_list:
            if 'core::Opcode' not in line:
                continue
            if '};' in line:
                return

            line = line.replace('static','')
            line = line.replace('core::Opcode','')
            line = line.replace('{','')
            line = line.replace('}','')
            line = line.replace('(','')
            line = line.replace(')','')
            line = line.replace(';','')
            line = line.replace('return','')
            while True:
                a = line.replace('  ', ' ')
                if a == line:
                    break
                line = a
            line = [x.strip() for x in line.strip().split(' ')]
            print ' = '.join(line)

if __name__ == '__main__':
    print ("# Generated opcodes")
    print ("# Do not update manually")
    print ("# Use: %s" % (' '.join(sys.argv)))
    print ("")

    f = sys.stdin
    if len(sys.argv) > 1:
        f = open(sys.argv[1], 'r')
    parse(f)
    if len(sys.argv) > 0:
        f.close()
