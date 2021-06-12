import sensor, image, time, math, pyb

GRAYSCALE_THRESHOLD = [(200,255)]



ROIS = [ # [ROI, weight]
        (0, 60, 160, 20, 0.1), 
        (0, 40, 160, 20, 0.3), 。
        (0, 0, 160, 20, 0.7)
       ]



weight_sum = 0 
for r in ROIS: weight_sum += r[4] 


sensor.reset() 
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA) 
sensor.skip_frames(30)
sensor.set_auto_gain(False) 
sensor.set_auto_whitebal(False) 
clock = time.clock()

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

status = -1  
turn = -1

def change_status(blocks):
    global status
    global turn
    n = len(blocks)
    x = []
    y = []
    for b in blocks:
        x.append(b[0])
        y.append(b[1])
    if(n >= 2 and (status == 0 or status == -1)):
        if(x[0] - x[1]) >= 20:
            status = 0
            turn = 2
        elif(x[1] - x[0] >= 20):
            status = 0
            turn = 1
        elif(x[0] <= 40 and x[1] <= 40):
            status = 1
            turn = 1
        elif(x[0] >= 120 and x[1] >= 120):
            status = 2
            turn = 2
    elif(n == 0 and status == 0):
        time.sleep(1)
        status = turn
    elif(n == 2 and ((status == 1) or (status == 2))):
        dx = abs(x[0] - x[1])
        dy = abs(y[0] - y[1])
        print(dx)
        if(dx != 0 and (dy/dx >= 1 and abs(x[0] - 80) <= 15)):
            status = 0

while(True):
    clock.tick() 
    img = sensor.snapshot() 

    centroid_sum = 0
    most_pixels = 0
    blocks = []

    for r in ROIS:
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True)
        if blobs:
            largest_blob = 0
            for i in range(len(blobs)):
                if blobs[i].pixels() > most_pixels:
                    most_pixels = blobs[i].pixels()
                    largest_blob = i

            if(blobs[largest_blob].cy() <= 80):
                img.draw_rectangle(blobs[largest_blob].rect())
                img.draw_cross(blobs[largest_blob].cx(),
                               blobs[largest_blob].cy())
                blocks.append((blobs[largest_blob].cx(), blobs[largest_blob].cy()))
                centroid_sum += blobs[largest_blob].cx() * r[4] 

    change_status(blocks)
    s = str(status)
    print(s)
    uart.write(s.encode())


