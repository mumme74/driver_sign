#! /usr/bin/env python3

# This file creates an image file for screen size
# drawn 90deg ccw and save that file in this folder 

from PIL import Image, ImageDraw, ImageFont
from pathlib import Path
from sys import platform
from io import BytesIO
import urllib.error, urllib.request
import configparser, os, zipfile

dir_path = Path(__file__).absolute().parent

# search paths to find fonts
fontsuffixes = ['','.ttf','.TTF','.otf','.OTF']

fontpaths = [dir_path / "fonts/", '']
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


# last resort try to download font and unpack
def dl_font(fontname):
  print(f"Font {fontname} not found, downloading it.")
  font = fontname.lower().replace(' ','-')
  params = urllib.request.Request(
    url=f"https://font.download/dl/font/{font}.zip",
    headers={
      "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
      "Accept-Encoding": "gzip, deflate, br, zstd",
      "Accept-Language": "en-US,en;q=0.9",
      "Sec-Ch-Ua": "\"Google Chrome\";v=\"123\", \"Not:A-Brand\";v=\"8\", \"Chromium\";v=\"123\"",
      "Referer":"https://www.google.com/",
      "Sec-Ch-Ua-Platform": "\"Windows\"",
      "Sec-Fetch-Mode": "navigate",
      "Sec-Fetch-Site": "cross-site",
      "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36"
    }
  )

  try:
    with urllib.request.urlopen(params) as resp:
      with zipfile.ZipFile(BytesIO(resp.read())) as z:
        for name in z.namelist():
          n = Path(name)
          if n.suffix.lower() in (".ttf", ".otf") and \
            not name.startswith('.'):
            z.extract(name, dir_path / "fonts/")
            # take a copy of first font to requested fontname
            if not os.path.exists(dir_path / f"fonts/{fontname}{n.suffix}"):
              z.getinfo(name).filename = f"{fontname}{n.suffix}"
              z.extract(name, dir_path / "fonts/")
  except (urllib.error.HTTPError, 
          urllib.error.URLError) as e:
    raise OSError(f"Error fetching font '{fontname}'\n{e}")


def load_font(fontfamily, fontsize, font_download = True):
  for path in fontpaths:
    for suf in fontsuffixes:
      font = os.path.join(path, f"{fontfamily}{suf}")
      try:
        return ImageFont.truetype(font, fontsize)
      except OSError as e:
        pass
  if font_download:
    dl_font(fontfamily)
    return load_font(fontfamily, fontsize, False)
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
  print(f"Created file: image_{letter}.png")

