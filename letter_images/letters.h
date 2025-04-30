
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

extern const struct vlu_img letter_A;

extern const struct vlu_img letter_B;

extern const struct vlu_img letter_C;

extern const struct vlu_img letter_D;

extern const struct vlu_img letter_E;

extern const struct vlu_img letter_F;

extern const struct vlu_img letter_G;

extern const struct vlu_img letter_H;

extern const struct vlu_img letter_I;

extern const struct vlu_img letter_J;

extern const struct vlu_img letter_K;

extern const struct vlu_img letter_L;

extern const struct vlu_img letter_M;

extern const struct vlu_img letter_N;

extern const struct vlu_img letter_O;

extern const struct vlu_img letter_P;

extern const struct vlu_img letter_Q;

extern const struct vlu_img letter_R;

extern const struct vlu_img letter_S;

extern const struct vlu_img letter_T;

extern const struct vlu_img letter_U;

extern const struct vlu_img letter_V;

extern const struct vlu_img letter_W;

extern const struct vlu_img letter_X;

extern const struct vlu_img letter_Y;

extern const struct vlu_img letter_Z;

#endif // LETTERS_H
