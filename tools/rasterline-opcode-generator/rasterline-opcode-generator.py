lines = 9
bad_line = 1

register = [
    {
        "opcode-read": "lda",
        "r-bytes": 1,
        "value": "#%00100000",
        "set_label": "charset_map_offset",
        "set_label_counter": 0,
        "opcode-write": "sta",
        "w-bytes": 2,
        "address": "$d018"
    },
    {
        "opcode-read": "lda",
        "r-bytes": 1,
        "value": "#0",
        "set_label": "scroll_offset",
        "set_label_counter": 0,
        "opcode-write": "sta",
        "w-bytes": 2,
        "address": "$d016"
    },
    {
        "opcode-read": "lda",
        "r-bytes": 1,
        "value": "#$0b",
        "set_label": "background_color",
        "set_label_counter": 0,
        "opcode-write": "sta",
        "w-bytes": 2,
        "address": "$d021"
    }
]

timings = {
    "lda": {
        1: 2,
        2: 4
    },
    "sta": {
        1: 3,
        2: 4
    },
    "stx": {
        1: 3,
        2: 4
    },
    "sty": {
        1: 3,
        2: 4
    },
    "nop": {
        1: 2
    }
}

cycles_line = 63
cycles_badline = 20

with open("rasterline-opcodes-out.asm", "w") as fh:
    for l in range(0, lines):
        for i in range(0, 8):
            cycles = 0
            for regitem in register:
                fh.write('{}{}\n'.format(regitem["set_label"], regitem["set_label_counter"]))
                fh.write('  {} {}\n'.format(regitem["opcode-read"], regitem["value"]))
                fh.write('  {} {}\n'.format(regitem["opcode-write"], regitem["address"]))
                cycles += timings[regitem["opcode-read"]][regitem["r-bytes"]]
                cycles += timings[regitem["opcode-write"]][regitem["w-bytes"]]
                regitem["set_label_counter"] += 1
            print("sum-cycles-line:{}".format(cycles))
            print(i)
            if i == (bad_line-1):
                nop_count = ((cycles_badline - cycles)/2)+2
            else:
                nop_count = ((cycles_line - cycles)/2)
            print("sum-nops-line:{}".format(round(nop_count)))
            for n in range (0, round(nop_count)):
                fh.write('  nop\n')
