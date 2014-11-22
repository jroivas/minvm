#!/usr/bin/python

import sys

def parse(f):
    got_list = False
    code = 0
    while True:
        line = f.readline()
        if 'Opcode' in line and 'class' in line and 'enum' in line:
            got_list = True
            continue
        if '{' in line:
            continue
        if '}' in line:
            return

        line = line.replace(',','').strip()
        if '=' in line:
            tmp = line.split('=')
            line = tmp[0].strip()
            code = int(tmp[1])

        if line and got_list:
            print ("%s = %s" % (line, code))
            code += 1

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
