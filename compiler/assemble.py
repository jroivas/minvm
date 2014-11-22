#!/usr/bin/python

import sys
import opcodes


def read(fname):
    with open(fname, 'r') as f:
        return f.readlines()

class ParseError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return '%s' % (self.value)

class Parser:
    def __init__(self, data):
        self.data = data
        self.code = ''
        self.output = {}
        self.debug = True
        self.labels = {}
        self.line = 0
        self.regmap = {}

    def __str__(self):
        r = ''
        for l in self.output:
            r += '%s: %s\n' % (l, self.output[l])
        return r
        #return self.code

    def parse_label(self, data):
        if not data.isalnum():
            return False
        if self.debug:
            print ("LABEL: %s" % (data))
        #self.labels[data] = len(self.code)
        self.labels[data] = self.line
        return True

    def raw_number(self, num):
        """
        >>> p = Parser('')
        >>> p.raw_number(48)
        '0'
        >>> p.raw_number(0x5048)
        'HP'
        >>> p.raw_number(0x434241)
        'ABC'
        >>> p.raw_number(0x00434241)
        'ABC'
        >>> p.raw_number(0x4400434241)
        'ABC\\x00D'
        """
        res = ''
        while num > 0:
            val = num & 0xff
            res += chr(val)
            num = num >> 8
        return res

    def fill_to_bytes(self, num):
        """

        >>> p = Parser('')
        >>> p.fill_to_bytes('')
        (1, '\\x00')
        >>> p.fill_to_bytes('0')
        (1, '0')
        >>> p.fill_to_bytes('AB')
        (2, 'AB')
        >>> p.fill_to_bytes('ABC')
        (4, '\\x00ABC')
        >>> p.fill_to_bytes('ABCD')
        (4, 'ABCD')
        >>> p.fill_to_bytes('ABCDE')
        (8, '\\x00\\x00\\x00ABCDE')
        >>> p.fill_to_bytes('ABCDEFGHI') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Invalid number ABCDEFGHI @0
        >>> p.fill_to_bytes('ABCDEFGH')
        (8, 'ABCDEFGH')
        """
        num_bytes = len(num)

        if num_bytes <= 1:
            num_bytes = 1
        elif num_bytes <= 2:
            num_bytes = 2
        elif num_bytes <= 4:
            num_bytes = 4
        elif num_bytes <= 8:
            num_bytes = 8
        else:
            raise ParseError("Invalid number %s @%s" % (num, self.line))

        while len(num) < num_bytes:
            num = '\x00' + num

        return (num_bytes, num)

    def is_float(self, val):
        for c in val:
            if not (c.isdigit() or c == '.'):
                return False
        return True

    def parse_reg(self, regdata):
        if regdata != 'PC' and regdata.upper()[0] != 'R':
            raise ParseError("Invalid register: %s @%s" % (regdata, self.line))

        if regdata == 'PC':
            return -1

        reg = int(regdata[1:])
        if reg == 0:
            raise ParseError("Invalid register: %s @%s" % (regdata, self.line))
        reg -= 1

        return reg

    def output_num(self, val, full=True):
        res = self.fill_to_bytes(self.raw_number(val))
        if full:
            return res
        return res[1]

    def parse_store(self, opts):
        data = [x.strip() for x in opts.split(',')]
        if len(data) != 2:
            raise ParseError("Invalid number argument for STORE: %s (%s) @%s" % (len(data), opts, self.line))

        reg = self.parse_reg(data[0])

        value = data[1]
        if value.isdigit():
            # Int
            val = int(value)
            (cnt, val) = self.output_num(val)
            if cnt == 1:
                self.code += chr(opcodes.STORE_INT8)
            elif cnt == 2:
                self.code += chr(opcodes.STORE_INT16)
            elif cnt == 4:
                self.code += chr(opcodes.STORE_INT32)
            elif cnt == 8:
                self.code += chr(opcodes.STORE_INT64)

            self.regmap[reg] = 'int'
            self.code += self.output_num(reg, False)
            self.code += val
        elif value.isfloat():
            # FIXME
            # Float
            self.regmap[reg] = 'float'
            pass
        else:
            # String
            self.code += chr(opcodes.STORE_STR)
            self.code += self.output_num(reg, False)
            self.regmap[reg] = 'str'
            self.code += value + '\x00'

    def parse_print(self, opts):
        opts = opts.strip()
        if opts[0] == 'R':
            reg = self.parse_reg(opts)
            if not reg in self.regmap:
                raise ParseError("Using unused register for PRINT: %s @%s" % (opts, self.line))
            if self.regmap[reg] == 'int':
                self.code += chr(opcodes.PRINT_INT)
            elif self.regmap[reg] == 'str':
                self.code += chr(opcodes.PRINT_STR)
            self.code += self.output_num(reg, False)
            print ("print %s %s" % (self.regmap[reg], reg))
        elif opts in self.labels:
            self.code += 'FIXME'
            self.code += chr(opcodes.PRINT_STR)
            print ("print %s" % (opts))

    def parse_command(self, cmd, opts):
        cmd = cmd.upper()
        if cmd == 'STORE':
            self.parse_store(opts)
        elif cmd == 'PRINT':
            self.parse_print(opts)
        else:
            print cmd, opts

    def pre_parse_line(self, line):
        datas = line.split(':')
        if len(datas) >= 2 and self.parse_label(datas[0]):
            return

    def parse_line(self, line):
        datas = line.split(':')
        if len(datas) >= 2 and self.parse_label(datas[0]):
            line = ':'.join(datas[1:]).strip()
        if not line:
            return

        cmds = line.split(' ')
        if not cmds:
            return

        self.parse_command(cmds[0], ' '.join(cmds[1:]))

    def pre_parse(self):
        for self.line, line in enumerate(self.data):
            line = line.strip()
            if not line or line[0] == ';':
                continue
            self.pre_parse_line(line)

    def parse(self):
        self.pre_parse()
        for self.line, line in enumerate(self.data):
            self.code = ''

            line = line.strip()
            if not line or line[0] == ';':
                continue

            self.parse_line(line)

            self.output[self.line] = self.code

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print ('Usage: %s input.asm' % (sys.argv[0]));
        sys.exit(1)

    data = read(sys.argv[1])
    p = Parser(data)
    p.parse()

    print "res: %s" % (p)
