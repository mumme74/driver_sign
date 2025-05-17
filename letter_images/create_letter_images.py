#! /usr/bin/env python3

# This file creates an image file for screen size
# drawn 90deg ccw and save that file in this folder 
from PIL import Image, ImageDraw, ImageFont
from pathlib import Path
from sys import platform
import configparser, os

# search paths to find fonts
fontsuffixes = ['','.ttf','.TTF','.otf','.OTF']

fontpaths = ['']
if platform == 'darwin': # apple
  fontpaths += ['~/Library/Fonts/', 
               '/System/Library/Fonts/Supplemental/',
               '/Library/Fonts/']
elif platform in ('linux', 'linux2'):
  fontpaths += ['~/.local/fonts/', 
               '/usr/share/fonts/',
               '/usr/local/share/fonts/',
               '/usr/share/fonts/truetype/'] 
elif platform == 'win32':
  fontpaths += [f'C:\\Users\\{os.getusername()}\\AppData\\Local\\Microsoft\\Windows\\Fonts\\',
                'C:\\Windows\\Fonts\\']
else:
  print(f'Unsupported os: {platform}, may not work correctly')

dir_path = Path(__file__).absolute().parent

def load_font(fontfamily, fontsize):
  for path in fontpaths:
    for suf in fontsuffixes:
      font = os.path.join(path, f"{fontfamily}{suf}")
      try:
        return ImageFont.truetype(font, fontsize)
      except OSError as e:
        pass
  raise OSError(f"Could not locate font: {fontfamily}, make sure it is installed in your system or select another font.")

config = configparser.ConfigParser()
config.read(dir_path / "config.ini")

# settings: from .ini file in this dir
fontfamily = config['settings']['fontfamily'] # monospace font fits in display
fontfamily_driver = config['settings']['fontfamily_driver']
screenX = config['settings'].getint('screenX') 
screenY = config['settings'].getint('screenY')
fontsize = config['settings'].getint('fontsize')
fontsize_driver = config['settings'].getint('fontsize_driver')
alphabet = config['settings']['alphabet']
rotation = config['settings'].getint('rotation')
invert = config['settings'].getboolean('invert')

# actual code below here
font1 = load_font(fontfamily, fontsize)
font2 = load_font(fontfamily_driver, fontsize_driver)
bgcolor = "black" if invert else "white"
fontcolor = "white" if invert else "black"

for letter in list(alphabet):
  img = Image.new("1", (screenX, screenY), bgcolor)
  img_rot = Image.new("1", (screenY, screenX), bgcolor)
  draw = ImageDraw.Draw(img_rot)
  draw.text((0, 0), letter, 
            font=font1, 
            fill=fontcolor)
  if letter in config['drivers'].keys():
    draw.text((20, 10),
              config['drivers'][letter],
              font=font2,
              fill=fontcolor)
  img.paste(img_rot.rotate(rotation, expand=1))
  img.save(dir_path / f"image_{letter}.png")

