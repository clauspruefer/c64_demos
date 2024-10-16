import math

SinMinValue = 30
SinMaxValue = 240
ValueCount = 256
StepCount = int(ValueCount/2)

with open('sinus-sprite-pixel-data.i', 'w') as fh:
    fh.write('!byte ')
    for i in range(0, StepCount):
        preval = math.sin(i/64)
        sinint = round((preval*100)+SinMinValue)
        #print('i:'+str(i)+' Sin(i):'+str(preval))
        #print('i:'+str(i)+' Sin(i):'+str(sinint)
        fh.write('$'+format(sinint, 'x')+',')

    fh.write('\n!byte ')
    for i in range(StepCount, 0, -1):
        preval = math.sin(i/64)
        sinint = round((preval*100)+SinMinValue)
        #print('i:'+str(i)+' Sin(i):'+str(preval))
        #print('i:'+str(i)+' Sin(i):'+str(sinint)
        fh.write('$'+format(sinint, 'x')+',')
