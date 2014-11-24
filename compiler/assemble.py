#!/usr/bin/python

import ctypes
import opcodes
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
        self.code = ''
        self.output = {}
        self.debug = True
        self.labels = {}
        self.line = 0
        self.regmap = {}
        self.opers = {
            '==': 0,
            '<': 1,
            '>': 2,
            '<=': 3,
            '>=': 4,
            '!=': 5,
            '&&': 8,
            '||': 9,
            '^': 14,
            '%': 15
            }

    def hexstr(self, s):
        res = ''
        for c in s:
            res += hex(ord(c)) + ' '
        return res

    def __str__(self):
        r = ''
        for l in self.output:
            r += '%s: %s\n' % (l, self.hexstr(self.output[l]))
        return r
        #return self.code

    def generate(self):
        res = ''
        for l in self.output:
            res += self.output[l]
        return res

    def parse_label(self, data):
        if not data.isalnum():
            return False
        if self.debug and data not in self.labels:
            print ("LABEL: %s" % (data))
        self.labels[data] = self.line
        return True

    def raw_number_neg(self, num):
        if num >= 0:
            raise ValueError('Invalid logic, number should not be positive')

        if ctypes.c_uint8(-num).value == -num and -num < 0xff/2:
            return ctypes.c_uint8(num).value
        if ctypes.c_uint16(-num).value == -num and -num < 0xffff/2:
            return ctypes.c_uint16(num).value
        if ctypes.c_uint32(-num).value == -num and -num < 0xffffffff/2:
            return ctypes.c_uint32(num).value

        return ctypes.c_uint64(num).value

    def raw_number(self, num):
        """
        >>> p = Parser('')
        >>> p.raw_number(-1)
        '\\xff'
        >>> p.raw_number(-2)
        '\\xfe'
        >>> p.raw_number(-3)
        '\\xfd'
        >>> p.raw_number(-200)
        '\\xff8'
        >>> p.raw_number(48)
        '0'
        >>> p.raw_number(0x5048)
        'PH'
        >>> p.raw_number(0x434241)
        'CBA'
        >>> p.raw_number(0x00434241)
        'CBA'
        >>> p.raw_number(0x4400434241)
        'D\\x00CBA'
        >>> p.raw_number(2147483647)
        '\\x7f\\xff\\xff\\xff'
        >>> p.raw_number(2147483649)
        '\\x80\\x00\\x00\\x01'
        """
        res = ''
        if num < 0:
            #if not signed:
            #    raise ParseError('Negative number in invalid place: %s @%s' % (num, self.line))
            num = self.raw_number_neg(num)

        while num > 0:
            val = num & 0xff
            res += chr(val)
            num = num >> 8
        return res[::-1]

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

        return reg

    def output_num(self, val, full=True):
        res = self.fill_to_bytes(self.raw_number(val))
        if full:
            return res
        return res[1]

    def format_string(self, s):
        res = ''
        escape = False
        for c in s:
            if not escape and c == '\\':
                escape = True
            elif escape and c == 'n':
                res += '\n'
                escape = False
            elif escape and c == 't':
                res += '\t'
                escape = False
            elif escape and c == '"':
                res += '"'
                escape = False
            elif escape:
                raise ParseError("Invalid escape: \\%s @%s" % (c, self.line))
            else:
                res += c
        return res

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
        elif self.is_float(value):
            # FIXME
            # Float
            self.regmap[reg] = 'float'
            pass
        elif value[0] == '"' and value[-1] == '"':
            # String
            self.code += chr(opcodes.STORE_STR)
            self.code += self.output_num(reg, False)
            self.regmap[reg] = 'str'
            self.code += self.format_string(value[1:-1]) + '\x00'
        else:
            raise ParseError("Invalid argument for STORE: %s @%s" % (value, self.line))

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
        else:
            raise ParseError("Unsupported PRINT: %s @%s" % (opts, self.line))
        """
        elif opts in self.labels:
            self.code += 'FIXME'
            self.code += chr(opcodes.PRINT_STR)
            print ("print %s" % (opts))
        elif opts[0] == '"':
            self.code += chr(opcodes.PRINT_STR)
            self.code += opts
            self.code += '\x00'
            print ("print %s" % (opts))
        """

    def parse_inc(self, opts):
        opts = opts.strip()
        if opts[0] == 'R':
            reg = self.parse_reg(opts)
            self.code += chr(opcodes.INC_INT)
            self.code += self.output_num(reg, False)
        else:
            raise ParseError("Unsupported INC: %s @%s" % (opts, self.line))

    def parse_dec(self, opts):
        opts = opts.strip()
        if opts[0] == 'R':
            reg = self.parse_reg(opts)
            self.code += chr(opcodes.DEC_INT)
            self.code += self.output_num(reg, False)
        else:
            raise ParseError("Unsupported DEC: %s @%s" % (opts, self.line))

    def parse_target(self, target):
        if target in self.labels:
            return ('label', self.labels[target])

        if target[0] == 'R':
            return ('reg', self.parse_reg(target))
        if target.isnum():
            return ('imm', int(target))

        return None

    def find_output(self, pos):
        if pos in self.output:
            return self.output[pos]
        return None
        """
        while pos < max(self.output):
            pos += 1
            if pos in self.output:
                return self.output[pos]

        raise ParseError("No code for line: %s" % (pos))
        """

    def estimate_jump_len(self, target_line):
        diff = target_line - self.line

        fuzzy = False

        s = target_line
        jlen = 0
        while s < self.line:
            outp = self.find_output(s)
            s += 1
            if outp is None:
                continue
            jlen += len(outp)
            if 'FIXME' in outp:
                print 'estimate_jump_len: FIXME'
                jlen += 10 # FIXME
                fuzzy = True

        if diff < 0:
            return (fuzzy, -jlen)
        if diff > 0 and diff < 50:
            return (True, (jlen + 10) * 10)

        raise ParseError('Can\'t estimate jump length')

    def parse_jmp(self, opts):
        data = [x.strip() for x in opts.split(',')]
        if len(data) == 2:
            cmp_ops = [x.strip() for x in data[0].split(' ')]
            cmp_op = 0
            if len(cmp_ops) == 3:
                cmp_op = self.opers[cmp_ops[1]]
                reg1 = self.parse_reg(cmp_ops[0])
                reg2 = self.parse_reg(cmp_ops[2])
            else:
                raise ParseError("Unsupported JMP: %s @%s" % (opts, self.line))

            (ttype, target) = self.parse_target(data[1].strip())
            if ttype == 'imm':
                (cnt, val) = self.output_num(target)
                if cnt == 1:
                    self.code += chr(opcodes.JMP8)
                elif cnt == 2:
                    self.code += chr(opcodes.JMP16)
                elif cnt == 4:
                    self.code += chr(opcodes.JMP32)
                elif cnt == 4:
                    self.code += chr(opcodes.JMP64)
                self.code += val
            elif ttype == 'label':
                (est, est_size) = self.estimate_jump_len(target)
                if est_size < 0xff:
                    bits = 1
                    self.code += chr(opcodes.JMP_LE8)
                elif est_size < 0xffff:
                    bits = 2
                    self.code += chr(opcodes.JMP_LE16)
                elif est_size < 0xffffffff:
                    bits = 4
                    self.code += chr(opcodes.JMP_LE32)
                else:
                    bits = 8
                    self.code += chr(opcodes.JMP_LE64)

                self.code += self.output_num(cmp_op, False)
                self.code += self.output_num(reg1, False)
                self.code += self.output_num(reg2, False)

                if not est:
                    if est_size < 0:
                        est_size -= 4
                    else:
                        (extra_bytes, _) = self.output_num(est_size + 8)
                        est_size += extra_bytes
                    self.code += self.output_num(est_size, False)
                else:
                    print 'parse_jmp: FIXME'
                    self.code = 'FIXME' + self.code
            else:
                raise ParseError("Unsupported JMP target: %s @%s" % (data[1], self.line))

        elif len(data) == 1:
            target = self.parse_target(data[1].strip())
        else:
            raise ParseError("Invalid JMP: %s @%s" % (opts, self.line))

    def parse_db(self, opts):
        data = [x.strip() for x in opts.split(',')]
        for val in data:
            if not val:
                continue
            if val[0] == '\'':
                self.code += self.format_string(val[1:-1])
            elif val[0] == '\"':
                self.code += self.format_string(val[1:-1])
            elif val[0] == '0' and val[1] == 'x':
                self.code += self.output_num(int(val, 16), False)
            elif val.isdigit():
                self.code += self.output_num(int(val), False)

    def stub_2regs(self, opcode, name, opts):
        data = [x.strip() for x in opts.split(',')]
        if len(data) == 2:
            reg1 = self.parse_reg(data[0])
            reg2 = self.parse_reg(data[1])

            self.code += chr(opcode)
            self.code += self.output_num(reg1, False)
            self.code += self.output_num(reg2, False)
        else:
            raise ParseError("Unsupported %s: %s @%s" % (name, opts, self.line))

    def stub_3regs(self, opcode, name, opts):
        data = [x.strip() for x in opts.split(',')]
        if len(data) == 3:
            reg1 = self.parse_reg(data[0])
            reg2 = self.parse_reg(data[1])
            reg3 = self.parse_reg(data[2])

            self.code += chr(opcode)
            self.code += self.output_num(reg1, False)
            self.code += self.output_num(reg2, False)
            self.code += self.output_num(reg3, False)
        else:
            raise ParseError("Unsupported %s: %s @%s" % (name, opts, self.line))

    def parse_mov(self, opts):
        self.stub_2regs(opcodes.MOV, 'MOV', opts)

    def parse_add(self, opts):
        self.stub_3regs(opcodes.ADD_INT, 'ADD', opts)

    def parse_sub(self, opts):
        self.stub_3regs(opcodes.SUB_INT, 'SUB', opts)

    def parse_mul(self, opts):
        self.stub_3regs(opcodes.MUL_INT, 'MUL', opts)

    def parse_div(self, opts):
        self.stub_3regs(opcodes.DIV_INT, 'DIV', opts)

    def parse_mod(self, opts):
        self.stub_3regs(opcodes.MOD_INT, 'MOD', opts)

    def parse_command(self, cmd, opts):
        cmd = cmd.upper()
        if cmd == 'STORE':
            self.parse_store(opts)
        elif cmd == 'PRINT':
            self.parse_print(opts)
        elif cmd == 'INC':
            self.parse_inc(opts)
        elif cmd == 'DEC':
            self.parse_dec(opts)
        elif cmd == 'JMP':
            self.parse_jmp(opts)
        elif cmd == 'MOV':
            self.parse_mov(opts)
        elif cmd == 'ADD':
            self.parse_add(opts)
        elif cmd == 'SUB':
            self.parse_sub(opts)
        elif cmd == 'MUL':
            self.parse_mul(opts)
        elif cmd == 'DIV':
            self.parse_div(opts)
        elif cmd == 'MOD':
            self.parse_mod(opts)
        elif cmd == 'DB':
            self.parse_db(opts)
        elif cmd == 'STOP':
            self.code += chr(opcodes.STOP)
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
    if len(sys.argv) <= 2:
        print ('Usage: %s input.asm output.bin' % (sys.argv[0]))
        sys.exit(1)

    data = read(sys.argv[1])
    p = Parser(data)
    p.parse()

    print "Code:\n%s" % (p)
    with open(sys.argv[2], 'w') as f:
        f.write(p.generate())
