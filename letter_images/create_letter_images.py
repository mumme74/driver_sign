#! /usr/bin/env python3

# This file creates an image file for screen size
# drawn 90deg ccw and save that file in this folder 
from PIL import Image, ImageDraw, ImageFont
from pathlib import Path
import os 

# settings
fontfamily = "Andale Mono" # monospace font fits in display
screen = (800, 480)
fontsize = 760
alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

# actual code below here
dir_path = os.path.dirname(os.path.realpath(__file__))

for letter in list(alphabet):
    img = Image.new("1", screen, "white")
    img_rot = Image.new("1", (screen[1], screen[0]), "white")
    draw = ImageDraw.Draw(img_rot)
    font = ImageFont.truetype(fontfamily, fontsize)
    draw.text((0, 0), letter, font=font)
    img.paste(img_rot.rotate(90, expand=1))
    img.save(os.path.join(dir_path, f"image_{letter}.png"))
