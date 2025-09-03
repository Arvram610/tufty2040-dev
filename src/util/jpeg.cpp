#include "jpeg.hpp"

using namespace pimoroni;

uint8_t current_flags = 0;

int JPEGDraw(JPEGDRAW *pDraw);

void JPEGImage::open() {
  img.openFLASH(data, dataSize, JPEGDraw);
  img.setUserPointer(graphics);
  img.setPixelType(RGB565_BIG_ENDIAN);
}

void JPEGImage::draw(int x, int y, int options, bool noDither) {
  current_flags = noDither ? (current_flags | FLAG_NO_DITHER)
                           : (current_flags & ~FLAG_NO_DITHER);
  open();
  img.decode(x, y, options);
}

int JPEGDraw(JPEGDRAW *pDraw) {
  PicoGraphics *current_graphics = (PicoGraphics *)pDraw->pUser;
  // "pixel" is slow and clipped,
  // guaranteeing we wont draw jpeg data out of the framebuffer..
  // Can we clip beforehand and make this faster?

  if (pDraw->iBpp == 8) {
    uint8_t *pixel = (uint8_t *)pDraw->pPixels;
    for (int y = 0; y < pDraw->iHeight; y++) {
      for (int x = 0; x < pDraw->iWidth; x++) {
        if (x < pDraw->iWidthUsed) {  // Clip to the used width
          current_graphics->set_pen((uint8_t)(*pixel >> 4));
          current_graphics->pixel({pDraw->x + x, pDraw->y + y});
        }
        pixel++;
      }
    }
  } else if (pDraw->iBpp == 4) {
    uint8_t *pixels = (uint8_t *)pDraw->pPixels;
    for (int y = 0; y < pDraw->iHeight; y++) {
      for (int x = 0; x < pDraw->iWidth; x++) {
        if (x >= pDraw->iWidthUsed) break;  // Clip to the used width
        int i = y * pDraw->iWidth + x;
        uint8_t p = pixels[i / 2];
        p >>= (i & 0b1) ? 0 : 4;
        p &= 0xf;
        current_graphics->set_pen(p);
        current_graphics->pixel({pDraw->x + x, pDraw->y + y});
      }
    }
  } else if (pDraw->iBpp == 1) {
    uint8_t *pixels = (uint8_t *)pDraw->pPixels;
    for (int y = 0; y < pDraw->iHeight; y++) {
      for (int x = 0; x < pDraw->iWidth; x++) {
        if (x >= pDraw->iWidthUsed) break;  // Clip to the used width
        int i = y * pDraw->iWidth + x;
        uint8_t p = pixels[i / 8];
        p >>= 7 - (i & 0b111);
        p &= 0x1;
        current_graphics->set_pen(p);
        current_graphics->pixel({pDraw->x + x, pDraw->y + y});
      }
    }
  } else {
    for (int y = 0; y < pDraw->iHeight; y++) {
      for (int x = 0; x < pDraw->iWidth; x++) {
        if (x >= pDraw->iWidthUsed) break;  // Clip to the used width
        int i = y * pDraw->iWidth + x;
        if (current_graphics->pen_type == PicoGraphics::PEN_RGB332) {
          if (current_flags & FLAG_NO_DITHER) {
            // Posterized output to RGB332
            current_graphics->set_pen(
                RGB((RGB565)pDraw->pPixels[i]).to_rgb332());
            current_graphics->pixel({pDraw->x + x, pDraw->y + y});
          } else {
            // Dithered output to RGB332
            current_graphics->set_pixel_dither({pDraw->x + x, pDraw->y + y},
                                               (RGB565)(pDraw->pPixels[i]));
          }
        } else if (current_graphics->pen_type == PicoGraphics::PEN_RGB888 ||
                   current_graphics->pen_type == PicoGraphics::PEN_DV_RGB888) {
          current_graphics->set_pen(RGB((RGB565)pDraw->pPixels[i]).to_rgb888());
          current_graphics->pixel({pDraw->x + x, pDraw->y + y});
        } else if (current_graphics->pen_type == PicoGraphics::PEN_P8 ||
                   current_graphics->pen_type == PicoGraphics::PEN_P4 ||
                   current_graphics->pen_type == PicoGraphics::PEN_DV_P5 ||
                   current_graphics->pen_type == PicoGraphics::PEN_3BIT ||
                   current_graphics->pen_type == PicoGraphics::PEN_INKY7) {
          if (current_flags & FLAG_NO_DITHER) {
            int closest = RGB((RGB565)pDraw->pPixels[i])
                              .closest(current_graphics->get_palette(),
                                       current_graphics->get_palette_size());
            if (closest == -1) {
              closest = 0;
            }
            current_graphics->set_pen(closest);
            current_graphics->pixel({pDraw->x + x, pDraw->y + y});
          } else {
            current_graphics->set_pixel_dither({pDraw->x + x, pDraw->y + y},
                                               RGB((RGB565)pDraw->pPixels[i]));
          }
        } else if (current_graphics->pen_type == PicoGraphics::PEN_DV_RGB555) {
          current_graphics->set_pen(RGB((RGB565)pDraw->pPixels[i]).to_rgb555());
          current_graphics->pixel({pDraw->x + x, pDraw->y + y});
        } else {
          current_graphics->set_pen(pDraw->pPixels[i]);
          current_graphics->pixel({pDraw->x + x, pDraw->y + y});
        }
      }
    }
  }
  return 1;
}