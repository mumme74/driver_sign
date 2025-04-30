from PIL import Image, ImageDraw, ImageFont
import os 

dir_path = os.path.dirname(os.path.realpath(__file__))

with open(os.path.join(dir_path, "letters.c"), encoding="utf8", mode="w") as c_file,\
     open(os.path.join(dir_path, "letters.h"), encoding="utf8", mode="w") as h_file:

    for letter in list("ABCDEFGHIJKLMNOPQRSTUVWXYZ"):
        img = Image.new("1", (800, 480), "white")
        img_rot = Image.new("1", (480, 800), "white")
        draw = ImageDraw.Draw(img_rot)
        font = ImageFont.truetype("Arial.ttf", 720)
        draw.text((0, 0), letter, font=font)
        img.paste(img_rot.rotate(90, expand=1))
        img.save(os.path.join(dir_path, f"letter_{letter}.png"))

        # create a cfile and a h file
        pixels = img.load()
        w,h = img.size
        c_file.write(f"\nconst char letter_{letter}[{w}*{h}] = " + "{") 
        h_file.write(f"\nextern const char letter_{letter};\n")
        for y in range(h):
            byt = p = 0
            for x in range(w):
                if x % 64 == 0:
                    c_file.write("\n    ")
                byt |= (0 if pixels[x,y] else 1) << p
                p += 1
                if p == 8: 
                    c_file.write(f"0x{byt:02x}, ")
                    byt = p = 0
            if p != 0:
                c_file.write(f"0x{byt:02x}, ")        
        c_file.write("};\n")
