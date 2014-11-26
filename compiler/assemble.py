#!/usr/bin/python

import ctypes
import opcodes
import sys
import math
import collections

def read(fname):
    with open(fname, 'r') as f:
        return f.readlines()

class ParseError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return '%s' % (self.value)

class Parser:
    __MAGIC_JUMP = 'FIXME'

    def __init__(self, data):
        self.data = data
        self.code = ''
        self.output = collections.OrderedDict({})
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
        """
        >>> p = Parser('')
        >>> p.hexstr('1234')
        '0x31 0x32 0x33 0x34'
        >>> p.hexstr('a')
        '0x61'
        >>> p.hexstr('\\0')
        '0x0'
        >>> p.hexstr('\\1')
        '0x1'
        >>> p.hexstr(' ')
        '0x20'
        >>> p.hexstr(' a  g')
        '0x20 0x61 0x20 0x20 0x67'
        """
        res = ''
        for c in s:
            res += hex(ord(c)) + ' '
        return res.strip()

    def __str__(self):
        r = ''
        for l in self.output:
            r += '%s: %s\n' % (l, self.hexstr(self.output[l]))
        return r

    def generate(self):
        """
        >>> p = Parser('')
        >>> p.generate()
        ''
        >>> p.output[1] = 'a'
        >>> p.output[2] = 'b'
        >>> p.output[6] = 'c'
        >>> p.generate()
        'abc'
        >>> p.output[10] = 'f'
        >>> p.generate()
        'abcf'
        >>> p.output[5] = 'm'
        >>> p.generate()
        'abmcf'
        """
        res = ''
        for l in collections.OrderedDict(sorted(self.output.items())):
            res += self.output[l]
        return res

    def parse_label(self, data):
        """
        >>> p = Parser('')
        >>> p.debug = False
        >>> p.parse_label('a')
        True
        >>> 'a' in p.labels
        True
        >>> p.labels['a']
        0
        >>> p.parse_label('a12')
        True
        >>> 'a12' in p.labels
        True
        >>> p.labels['a12']
        0
        >>> p.parse_label('a12-,')
        False
        >>> 'a12-,' in p.labels
        False
        >>> p.parse_label('daud8912eh8921he')
        True
        >>> p.parse_label('test_label')
        False
        """
        if not data.isalnum():
            return False
        if self.debug and data not in self.labels:
            print ('LABEL: %s' % (data))
        self.labels[data] = self.line
        return True

    def raw_number_neg(self, num):
        """
        >>> p = Parser('')
        >>> p.raw_number_neg(-1)
        255
        >>> p.raw_number_neg(-12)
        244
        >>> p.raw_number_neg(-126)
        130
        >>> p.raw_number_neg(-127)
        65409
        >>> p.raw_number_neg(-200)
        65336
        >>> p.raw_number_neg(-40000)
        4294927296L
        >>> p.raw_number_neg(1) # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ValueError: Invalid logic, number should not be positive
        """
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
            raise ParseError('Invalid number %s @%s' % (num, self.line))

        while len(num) < num_bytes:
            num = '\x00' + num

        return (num_bytes, num)

    def is_float(self, val):
        """
        >>> p = Parser('')
        >>> p.is_float('')
        False
        >>> p.is_float('.')
        False
        >>> p.is_float('.0')
        True
        >>> p.is_float('1.0')
        True
        >>> p.is_float('1.00005')
        True
        >>> p.is_float('1.000.05')
        False
        """
        if not val:
            return False
        got_digit = False
        got_dot = False
        for c in val:
            if not (c.isdigit() or c == '.'):
                return False
            if not got_digit and c.isdigit():
                got_digit = True
            elif c == '.':
                if got_dot:
                    return False
                got_dot = True
        return got_digit

    def parse_reg(self, regdata):
        """
        >>> p = Parser('')
        >>> p.parse_reg('a') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Invalid register: A @0
        >>> p.parse_reg('r') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ValueError: invalid literal for int() with base 10: ''
        >>> p.parse_reg('r0')
        0
        >>> p.parse_reg('r1')
        1
        >>> p.parse_reg('r112')
        112
        >>> p.parse_reg('R999')
        999
        >>> p.parse_reg('PC')
        -1
        >>> p.parse_reg('pc')
        -1
        """
        regdata = regdata.upper()
        if regdata != 'PC' and regdata[0] != 'R':
            raise ParseError('Invalid register: %s @%s' % (regdata, self.line))

        if regdata == 'PC':
            return -1

        reg = int(regdata[1:])

        return reg

    def output_num(self, val, full=True):
        """
        >>> p = Parser('')
        >>> p.output_num(12)
        (1, '\\x0c')
        >>> p.output_num(1234)
        (2, '\\x04\\xd2')
        >>> p.output_num(65)
        (1, 'A')
        >>> p.output_num(1234567890)
        (4, 'I\\x96\\x02\\xd2')
        >>> p.output_num(1234567890, False)
        'I\\x96\\x02\\xd2'
        """
        res = self.fill_to_bytes(self.raw_number(val))
        if full:
            return res
        return res[1]

    def format_string(self, s):
        """
        >>> p = Parser('')
        >>> p.format_string('a')
        'a'
        >>> p.format_string('aa\\\\taa')
        'aa\\taa'
        >>> p.format_string('aa\\\\naa')
        'aa\\naa'
        >>> p.format_string('aa\\\\"naa')
        'aa"naa'
        >>> p.format_string('aa\\\\fbb') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Invalid escape: \\f @0
        """
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
                raise ParseError('Invalid escape: \\%s @%s' % (c, self.line))
            else:
                res += c
        return res

    def parse_store(self, opts):
        """
        >>> p = Parser('')
        >>> p.parse_store('R0, 1')
        '\\x02\\x00\\x01'
        >>> p.code = ''
        >>> p.parse_store('R1, 1')
        '\\x02\\x01\\x01'
        >>> p.code = ''
        >>> p.parse_store('R11, 28')
        '\\x02\\x0b\\x1c'
        >>> p.code = ''
        >>> p.parse_store('R11, 12345')
        '\\x03\\x0b09'
        >>> p.code = ''
        >>> p.parse_store('R11, 123456789')
        '\\x04\\x0b\\x07[\\xcd\\x15'
        >>> p.parse_store('') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Invalid number argument for STORE: 1 () @0
        >>> p.parse_store('R1') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Invalid number argument for STORE: 1 (R1) @0
        >>> p.parse_store('R1, R2, 3') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Invalid number argument for STORE: 3 (R1, R2, 3) @0
        >>> p.code = ''
        >>> p.parse_store('R1, "abc"')
        '\\x07\\x01abc\\x00'
        >>> p.code = ''
        >>> p.parse_store('R1, 1.234') # FIXME: Unimplemented
        ''
        """
        data = [x.strip() for x in opts.split(',')]
        if len(data) != 2:
            raise ParseError('Invalid number argument for STORE: %s (%s) @%s' % (len(data), opts, self.line))

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
            # TODO
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
            raise ParseError('Invalid argument for STORE: %s @%s' % (value, self.line))

        return self.code

    def parse_print(self, opts):
        """
        >>> p = Parser('')
        >>> p.parse_print('') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: No mandatory parameter given for PRINT
        >>> p.parse_print('a') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Unsupported PRINT: a @0
        >>> p.parse_print('"a"') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Unsupported PRINT: "a" @0
        >>> p.parse_print('R0') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Using unused register for PRINT: R0 @0
        >>> p.regmap[1] = "int"
        >>> p.regmap[2] = "str"
        >>> p.parse_print('R1')
        '\\x0f\\x01'
        >>> p.code = ''
        >>> p.parse_print('R2')
        '\\x11\\x02'
        """
        opts = opts.strip()
        if not opts:
            raise ParseError('No mandatory parameter given for PRINT')
        if opts[0] == 'R':
            reg = self.parse_reg(opts)
            if not reg in self.regmap:
                raise ParseError('Using unused register for PRINT: %s @%s' % (opts, self.line))
            if self.regmap[reg] == 'int':
                self.code += chr(opcodes.PRINT_INT)
            elif self.regmap[reg] == 'str':
                self.code += chr(opcodes.PRINT_STR)
            self.code += self.output_num(reg, False)
        else:
            raise ParseError('Unsupported PRINT: %s @%s' % (opts, self.line))
        """
        elif opts in self.labels:
            self.code += 'FIXME'
            self.code += chr(opcodes.PRINT_STR)
            print ('print %s' % (opts))
        elif opts[0] == '"':
            self.code += chr(opcodes.PRINT_STR)
            self.code += opts
            self.code += '\x00'
            print ('print %s' % (opts))
        """
        return self.code

    def parse_inc(self, opts):
        """
        >>> p = Parser('')
        >>> p.parse_inc('') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: No mandatory parameter given for INC
        >>> p.parse_inc('R0')
        '\\x08\\x00'
        >>> p.code = ''
        >>> p.parse_inc('R18')
        '\\x08\\x12'
        >>> p.parse_inc('a') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Unsupported INC: a @0
        >>> p.parse_inc('PC') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Unsupported INC: PC @0
        """
        opts = opts.strip()
        if not opts:
            raise ParseError('No mandatory parameter given for INC')
        if opts[0] == 'R':
            reg = self.parse_reg(opts)
            self.code += chr(opcodes.INC_INT)
            self.code += self.output_num(reg, False)
        else:
            raise ParseError('Unsupported INC: %s @%s' % (opts, self.line))

        return self.code

    def parse_dec(self, opts):
        """
        >>> p = Parser('')
        >>> p.parse_dec('') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: No mandatory parameter given for DEC
        >>> p.parse_dec('R0')
        '\\t\\x00'
        >>> p.code = ''
        >>> p.parse_dec('R18')
        '\\t\\x12'
        >>> p.parse_dec('PC') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Unsupported DEC: PC @0
        """
        opts = opts.strip()
        if not opts:
            raise ParseError('No mandatory parameter given for DEC')
        if opts[0] == 'R':
            reg = self.parse_reg(opts)
            self.code += chr(opcodes.DEC_INT)
            self.code += self.output_num(reg, False)
        else:
            raise ParseError('Unsupported DEC: %s @%s' % (opts, self.line))

        return self.code

    def parse_target(self, target):
        """
        >>> p = Parser('')
        >>> p.parse_target('') # doctest: +ELLIPSIS
        Traceback (most recent call last):
        ...
        ParseError: Invalid target
        >>> p.parse_target('R1')
        ('reg', 1)
        >>> p.parse_target('R99')
        ('reg', 99)
        >>> p.parse_target('123')
        ('imm', 123)
        >>> p.parse_target('0')
        ('imm', 0)
        >>> p.parse_target('label') is None
        True
        >>> p.labels['label'] = 17
        >>> p.labels['label2'] = 42
        >>> p.parse_target('label')
        ('label', 17)
        """
        if target in self.labels:
            return ('label', self.labels[target])

        if not target:
            raise ParseError('Invalid target')
        if target[0] == 'R':
            return ('reg', self.parse_reg(target))
        if target.isdigit():
            return ('imm', int(target))

        return None

    def find_output(self, pos):
        """
        >>> p = Parser('')
        >>> p.find_output(1) is None
        True
        >>> p.find_output(-1) is None
        True
        >>> p.output[1] = 'a'
        >>> p.output[2] = 'b'
        >>> p.output[6] = 'c'
        >>> p.find_output(1)
        'a'
        >>> p.find_output(6)
        'c'
        """
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
        """
        >>> p = Parser('')
        >>> p.estimate_jump_len(10)
        (True, 100)
        >>> p.line = 5
        >>> p.estimate_jump_len(1)
        (False, 0)
        >>> p.output[1] = 'a'
        >>> p.output[2] = 'bb42'
        >>> p.output[6] = 'c'
        >>> p.output[8] = 'eue'
        >>> p.output[10] = 'ff'
        >>> p.output[11] = 'ff12334567789'
        >>> p.estimate_jump_len(1)
        (False, -5)
        >>> p.estimate_jump_len(3)
        (False, 0)
        >>> p.estimate_jump_len(2)
        (False, -4)
        >>> p.estimate_jump_len(8)
        (True, 110)
        >>> p.estimate_jump_len(100)
        (True, 290)
        """
        diff = target_line - self.line

        fuzzy = False

        s = min(target_line, self.line)
        s_end = max(target_line, self.line)
        jlen = 0
        while s < s_end:
            outp = self.find_output(s)
            s += 1
            if outp is None:
                continue
            jlen += len(outp)
            if Parser.__MAGIC_JUMP in outp:
                jlen += 10
                fuzzy = True

        if diff < 0:
            return (fuzzy, -jlen)

        # Just estimate roughly upwards.
        # Does not result optimal code, but it should work in most cases
        return (True, (jlen + 10) * 10)

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
                raise ParseError('Unsupported JMP: %s @%s' % (opts, self.line))

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
                    self.code = '%s %s,%s:' % (Parser.__MAGIC_JUMP, bits, target) + self.code
            else:
                raise ParseError('Unsupported JMP target: %s @%s' % (data[1], self.line))

        elif len(data) == 1:
            target = self.parse_target(data[1].strip())
        else:
            raise ParseError('Invalid JMP: %s @%s' % (opts, self.line))

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
            raise ParseError('Unsupported %s: %s @%s' % (name, opts, self.line))

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
            raise ParseError('Unsupported %s: %s @%s' % (name, opts, self.line))

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
            print (cmd, opts)

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

    def try_fix(self, line):
        data = self.output[line].split(':')
        (bits, target) = [int(x) for x in data[0][6:].split(',')]

        tmp = line+1
        size = 0
        while tmp < target:
            if tmp in self.output:
                if self.output[tmp][:5] == Parser.__MAGIC_JUMP:
                    return (False, 0)
                size += len(self.output[tmp])
            tmp += 1
        return (True, size)

    def fix_line(self, line, size):
        data = self.output[line].split(':')
        (bits, target) = [int(x) for x in data[0][6:].split(',')]
        data = ':'.join(data[1:])

        num = self.output_num(size + bits, False)
        while len(num) < bits/8:
            num = '\x00' + num
        self.output[line] = data + num

    def try_fixes(self, lines):
        for l in lines:
            (res, size) = self.try_fix(l)
            if res:
                self.fix_line(l, size)

    def fixme_lines(self):
        flines = []
        for c in self.output:
            if self.output[c][:5] == Parser.__MAGIC_JUMP:
                flines.append(c)
        return flines

    def fix_fixmes(self):
        flines = self.fixme_lines()
        tries = 0
        while flines:
            self.try_fixes(flines)
            flines = self.fixme_lines()
            tries += 1
            if tries >= 100:
                break

    def parse(self):
        self.pre_parse()
        for self.line, line in enumerate(self.data):
            self.code = ''

            line = line.strip()
            if not line or line[0] == ';':
                continue

            self.parse_line(line)

            self.output[self.line] = self.code

        self.fix_fixmes()

if __name__ == '__main__':
    if len(sys.argv) <= 2:
        print ('Usage: %s input.asm output.bin' % (sys.argv[0]))
        sys.exit(1)

    data = read(sys.argv[1])
    p = Parser(data)
    p.parse()

    print ('Code:\n%s' % (p))
    with open(sys.argv[2], 'w') as f:
        f.write(p.generate())
