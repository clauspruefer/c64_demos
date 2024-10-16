StartVal = 1
EndVal = 40

with open('sin-processing.asm', 'w') as fh:
    for i in range(StartVal, EndVal):
        CmdLoad = 'lda lbl_sprite{}_2+3'.format(i+1)
        CmdStore = 'sta lbl_sprite{}_2+3'.format(i)
        fh.write(CmdLoad+'\n'+CmdStore+'\n')

        CmdLoad = 'lda lbl_sprite{}_2+1'.format(i+1)
        CmdStore = 'sta lbl_sprite{}_2+1'.format(i)
        fh.write(CmdLoad+'\n'+CmdStore+'\n')

        CmdLoad = 'lda lbl_sprite{}_1+3'.format(i+1)
        CmdStore = 'sta lbl_sprite{}_1+3'.format(i)
        fh.write(CmdLoad+'\n'+CmdStore+'\n')

        CmdLoad = 'lda lbl_sprite{}_1+1'.format(i+1)
        CmdStore = 'sta lbl_sprite{}_1+1'.format(i)
        fh.write(CmdLoad+'\n'+CmdStore+'\n')
