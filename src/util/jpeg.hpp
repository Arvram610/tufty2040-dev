#pragma once
#include <JPEGDEC.h>
#include <pico.h>
#include <pico/stdlib.h>

#include <pico_graphics.hpp>

enum FLAGS : uint8_t { FLAG_NO_DITHER = 1u };

class JPEGImage {
 private:
  uint8_t *data;
  const int dataSize;
  JPEGDEC img;
  pimoroni::PicoGraphics *graphics;

 private:
  void open();

 public:
  JPEGImage(pimoroni::PicoGraphics *graphics, uint8_t *data, int dataSize)
      : data(data), dataSize(dataSize), graphics(graphics) {}
  void draw(int x, int y, int options, bool noDither = false);
};
