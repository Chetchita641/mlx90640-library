import sys
import colorsys
import time
from PIL import Image
sys.path.insert(0, "./build/lib.linux-armv7l-2.7")

import MLX90640 as mlx

left_img = Image.new( 'RGB', (24,32), "black")
right_img = Image.new( 'RGB', (24,32), "black")

def temp_to_col(val):
    hue = (180 - (val * 6)) / 360.0
    return tuple([int(c*255) for c in colorsys.hsv_to_rgb(hue % 1, 1.0, 1.0)])

mlx.setup(0x33, 1)
f = mlx.get_frame()

v_min = min(f)
v_max = max(f)

print(min(f))
print(max(f))

print(",".join(["{:05b}".format(x) for x in range(24)]))
for x in range(24):
    row = []
    for y in range(32):
        val = f[32 * (23-x) + y]
        row.append(val)
        left_img.putpixel((x, y), temp_to_col(val))
    print(",".join(["{:05.2f}".format(v) for v in row]))

left_img = left_img.resize((480,640), Image.BICUBIC)

left_img.save("/home/pi/Desktop/left.png")

#/////////////////////////////////////////////////////////////////////////////// 

mlx.setup(0x77, 1)
f = mlx.get_frame()

v_min = min(f)
v_max = max(f)

print(min(f))
print(max(f))

print(",".join(["{:05b}".format(x) for x in range(24)]))
for x in range(24):
    row = []
    for y in range(32):
        val = f[32 * (23-x) + y]
        row.append(val)
        right_img.putpixel((x, y), temp_to_col(val))
    print(",".join(["{:05.2f}".format(v) for v in row]))

right_img = right_img.resize((480,640), Image.BICUBIC)

right_img.save("/home/pi/Desktop/right.png")

