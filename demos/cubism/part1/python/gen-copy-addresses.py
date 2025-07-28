StartVal = (9*8)-2
EndVal = 0

with open('copy-data.asm', 'w') as fh:
    for i in range(StartVal, EndVal, -1):
        CmdLoad = 'lda charset_map_offset{}+1'.format(i)
        CmdStore = 'sta charset_map_offset{}+1'.format(i+1)
        fh.write(CmdLoad+'\n'+CmdStore+'\n')
        CmdLoad = 'lda scroll_offset{}+1'.format(i)
        CmdStore = 'sta scroll_offset{}+1'.format(i+1)
        fh.write(CmdLoad+'\n'+CmdStore+'\n')
        CmdLoad = 'lda background_color{}+1'.format(i)
        CmdStore = 'sta background_color{}+1'.format(i+1)
        fh.write(CmdLoad+'\n'+CmdStore+'\n')
