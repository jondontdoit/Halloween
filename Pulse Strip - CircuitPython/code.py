# Using a Trinket M0

import time
import board
import neopixel
import random
from adafruit_simplemath import map_range

pixel_pin = board.D4
num_pixels = 150

comet_speed = 10
comet_size = 3

BLACK = (0,0,0)
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.1, auto_write=False, pixel_order='RGB')


comet_colour = [(255,255,255), (96,96,96), (16,16,16)]
def get_comet_colour():
    PALETTE = [
        (255, 0, 0),    # RED
        (255, 150, 0),  # YELLOW
        (0, 255, 0),    # GREEN
        (0, 255, 255),  # CYAN
        (0, 0, 255),    # BLUE
        (180, 0, 255)   # PURPLE
    ]
    #colour = PALETTE[random.randint(0, len(PALETTE)-1)]
    colour = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))

    # Find brightest value
    m = colour[0]
    if colour[1] > m:
        m = colour[1]
    if colour[2] > m:
        m = colour[2]

    # Scale to full
    max_val = 255
    colour = (int(map_range(colour[0], 0, m, 0, max_val)), int(map_range(colour[1], 0, m, 0, max_val)), int(map_range(colour[2], 0, m, 0, max_val)))

    # Apply to coment head
    comet_colour[0] = colour

    # Scale down brightness for second segment
    max_val = 128
    colour = (int(map_range(colour[0], 0, m, 0, max_val)), int(map_range(colour[1], 0, m, 0, max_val)), int(map_range(colour[2], 0, m, 0, max_val)))

    # Apply to coment head
    comet_colour[1] = colour

    # Scale down brightness for second segment
    max_val = 64
    colour = (int(map_range(colour[0], 0, m, 0, max_val)), int(map_range(colour[1], 0, m, 0, max_val)), int(map_range(colour[2], 0, m, 0, max_val)))

    # Apply to coment head
    comet_colour[2] = colour

    return

def draw_comet(start_pos):
    if start_pos < 0 or start_pos >= num_pixels:
        start_pos = 0

    # Clear the strip
    pixels.fill(BLACK)
    pixels.show()

    for c in range(0, len(comet_colour)):
        for s in range(0, comet_size):
            pixels[num_pixels-start_pos-1] = comet_colour[c]
            if start_pos < 1:
                start_pos = num_pixels
            start_pos -= 1
    pixels.show()

    return


comet_pos = 0
comet_timer = int(time.monotonic()*1000)
get_comet_colour()

while True:
    now = time.monotonic()
    now_millis = int(now*1000)

    if now_millis - comet_timer > comet_speed:
        draw_comet(comet_pos)
        comet_pos += 1

        if comet_pos >= num_pixels:
            comet_pos = 0

        if comet_pos == 9:
            comet_speed = random.randint(3,20)
            get_comet_colour()

        comet_timer = now_millis


    #time.sleep(1)
