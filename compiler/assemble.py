#!/usr/bin/python

import sys


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
        self.code = []
        self.debug = True
        self.labels = {}
        self.pos = 0
        self.line = 0

    def parse_label(self, data):
        if not data.isalnum():
            return False
        if self.debug:
            print ("LABEL: %s" % (data))
        self.labels[data] = self.pos
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

    def parse_store(self, opts):
        data = [x.strip() for x in opts.split(',')]
        print data
        if len(data) != 2:
            raise ParseError("Invalid number argument for STORE: %s (%s) @%s" % (len(data), opts, self.line))

        if data[0] != 'PC' and data[0].upper()[0] != 'R':
            raise ParseError("Invalid register: %s @%s" % (data[0], self.line))

        value = data[1]
        if value.isdigit():
            # Int
            val = int(value)
            (_, val) = self.fill_to_bytes(self.raw_number(val))
            print ("VV", val, value)
        #if value[0] == '"' and value[-1] == '"':
        #    # String
        #    pass

    def parse_command(self, cmd, opts):
        cmd = cmd.upper()
        if cmd == 'STORE':
            self.parse_store(opts)
        else:
            print cmd, opts

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

    def parse(self):
        for self.line, line in enumerate(self.data):
            line = line.strip()
            if not line or line[0] == ';':
                continue
            self.parse_line(line)

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print ('Usage: %s input.asm' % (sys.argv[0]));
        sys.exit(1)

    data = read(sys.argv[1])
    p = Parser(data)
    p.parse()

