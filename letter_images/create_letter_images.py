from PIL import Image, ImageDraw, ImageFont
import os 

fontfamily = "Andale Mono" # monospace font fits in display

dir_path = os.path.dirname(os.path.realpath(__file__))

with open(os.path.join(dir_path, "letters.c"), encoding="utf8", mode="w") as c_file,\
     open(os.path.join(dir_path, "letters.h"), encoding="utf8", mode="w") as h_file:
    
    h_file.write("""
#ifndef LETTERS_H
#define LETTERS_H
#include <stdint.h>

// this is a compressed version to save space, store len and vlu
// render len many pixels with vlu color
struct vlu_seg {
  uint16_t len;
  uint8_t vlu;
};
 

struct vlu_img {
  struct vlu_seg* segs;
  uint16_t n_segs;
};
""")
    c_file.write("#include \"letters.h\"\n\n")

    for letter in list("ABCDEFGHIJKLMNOPQRSTUVWXYZ"):
        img = Image.new("1", (800, 480), "white")
        img_rot = Image.new("1", (480, 800), "white")
        draw = ImageDraw.Draw(img_rot)
        font = ImageFont.truetype(fontfamily, 760)
        draw.text((0, 0), letter, font=font)
        img.paste(img_rot.rotate(90, expand=1))
        img.save(os.path.join(dir_path, f"letter_{letter}.png"))

        # create a cfile and a h file
        pixels = img.load()
        w,h = img.size
        vluLen = 0
        vlus = []
        vlu = 0
        for y in range(h):
            for x in range(w):
                # compress it
                pix_vlu = 1 if pixels[x,y] else 0
                if pix_vlu != vlu or vluLen == 65535:
                    if vluLen:
                        vlus.append((vlu, vluLen))
                    vluLen = 0
                    vlu = pix_vlu
                vluLen += 1
            
        if vluLen:
            vlus.append((vlu, vluLen))
        h_file.write(f"\nextern const struct vlu_img letter_{letter};\n")
            
        c_file.write(f"\nstatic const struct vlu_seg letter_{letter}_segs[{len(vlus)}] = " + "{\n")
        for seg in vlus:
            c_file.write(f"  {{.len = {seg[1]}, .vlu = {seg[0]}}},\n")   
        c_file.write("};\n")
        c_file.write(f"""
const struct vlu_img letter_{letter} = {{
    .segs = &letter_{letter}_segs,
    .n_segs = {len(vlus)}
}};
""")

    h_file.write("\n#endif // LETTERS_H\n")
