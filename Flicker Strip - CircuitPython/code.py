# Using Feather M4 Express
import time
import board
import neopixel
import random

pixel_pin = board.D5
num_pixels = 400

pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.1, auto_write=False, pixel_order='RGB')


def get_random_colour():
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
    return colour

flicker_enable = True
flicker_run = random.randint(50, 1000)
flicker_pause = random.randint(200, 5000)
flicker_timer = int(time.monotonic()*1000)
pixels.fill(get_random_colour())
pixels.show()

while True:
    now = time.monotonic()
    now_millis = int(now*1000)

    if flicker_enable:
        pixels.brightness = random.random()
        pixels.show()
    else:
        pixels.brightness = 0.0
        pixels.show()

    if flicker_enable and now_millis - flicker_timer > flicker_run:
        flicker_enable = False
        flicker_timer = now_millis

    if not flicker_enable and now_millis - flicker_timer > flicker_pause:
        flicker_enable = True
        flicker_run = random.randint(50, 1000)
        flicker_pause = random.randint(200, 5000)
        flicker_timer = now_millis
        pixels.fill(get_random_colour())
        pixels.show()

    #time.sleep(1)
