from string import Template

CountCharLines = 4
CountSpritesPerLine = 4
LineNr = CountSpritesPerLine + 1

DataReplaced = ''

with open('rasterbar-4line-tpl.asm', 'r') as fh:
    DataSrc = Template(fh.read())

for i in range(0, CountCharLines):
    ReplaceValues = {}
    for n in range(0, CountSpritesPerLine):
        ReplaceValues['lbl_sprite'+str(n+1)+'_1'] = 'lbl_sprite'+str(LineNr)+'_1'
        ReplaceValues['lbl_sprite'+str(n+1)+'_2'] = 'lbl_sprite'+str(LineNr)+'_2'
        ReplaceValues['lbl_barcol'+str(n+1)] = 'lbl_barcol'+str(LineNr)
        LineNr += 1

    #print(ReplaceValues)
    DataReplaced += DataSrc.substitute(ReplaceValues)

with open('rbars-gen-code-spritestop.asm', 'w') as fh:
    fh.write(DataReplaced)

CountCharLines = 5
LineNr = 21

DataReplaced = ''

for i in range(0, CountCharLines):
    ReplaceValues = {}
    for n in range(0, CountSpritesPerLine):
        ReplaceValues['lbl_sprite'+str(n+1)+'_1'] = 'lbl_sprite'+str(LineNr)+'_1'
        ReplaceValues['lbl_sprite'+str(n+1)+'_2'] = 'lbl_sprite'+str(LineNr)+'_2'
        ReplaceValues['lbl_barcol'+str(n+1)] = 'lbl_barcol'+str(LineNr)
        LineNr += 1

    #print(ReplaceValues)
    DataReplaced += DataSrc.substitute(ReplaceValues)

with open('rbars-gen-code-spritesbottom.asm', 'w') as fh:
    fh.write(DataReplaced)
