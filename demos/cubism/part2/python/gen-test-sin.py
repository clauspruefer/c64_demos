import math

SinMinValue = 1
SinMaxValue = 32
ValueCount = 256
StepCount = int(ValueCount/2)

bank_bytes = [ 32,  48, 64,  80, 96,  112, 128,  144, 160, 176, 192, 208, 224, 240 ]

with open('sinus-d016-data.i', 'w') as fh:
    with open('sinus-d018-data.i', 'w') as fh2:

        fh.write('!byte ')
        fh2.write('!byte ')

        for i in range(0, StepCount):
            preval = math.sin(i/64)
            sinint = round((preval*100)+SinMinValue)
            sinintmod = sinint % 8
            #print('i:'+str(i)+' Sin(i):'+str(preval))
            #print('i:'+str(i)+' Sin(i):'+str(sinint)
            fh.write('$'+format(sinintmod, 'x')+',')
            fh2.write('$'+format(bank_bytes[round(sinint/8)], 'x')+',')

        fh.write('\n!byte ')
        fh2.write('\n!byte ')

        for i in range(StepCount, 0, -1):
            preval = math.sin(i/64)
            sinint = round((preval*100)+SinMinValue)
            sinintmod = sinint % 8
            #print('i:'+str(i)+' Sin(i):'+str(preval))
            #print('i:'+str(i)+' Sin(i):'+str(sinint)
            fh.write('$'+format(sinintmod, 'x')+',')
            fh2.write('$'+format(bank_bytes[round(sinint/8)], 'x')+',')
