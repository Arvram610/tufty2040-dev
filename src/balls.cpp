#include "common.hpp"

using namespace pimoroni;

REGISTER_RUNNABLE(secret_balls, 0) {
  st7789->set_backlight(255);

  Pen WHITE = graphics->create_pen(255, 255, 255);
  Pen BG = graphics->create_pen(120, 40, 60);

  struct pt {
    float x;
    float y;
    uint8_t r;
    float dx;
    float dy;
    uint16_t pen;
  };

  std::vector<pt> shapes;
  for (int i = 0; i < 100; i++) {
    pt shape;
    shape.x = rand() % graphics->bounds.w;
    shape.y = rand() % graphics->bounds.h;
    shape.r = (rand() % 10) + 3;
    shape.dx = float(rand() % 255) / 64.0f;
    shape.dy = float(rand() % 255) / 64.0f;
    shape.pen = graphics->create_pen(rand() % 255, rand() % 255, rand() % 255);
    shapes.push_back(shape);
  }

  Point text_location(0, 0);
  uint8_t i = 0;

  while (true) {
    graphics->set_pen(BG);
    graphics->clear();

    for (auto &shape : shapes) {
      shape.x += shape.dx;
      shape.y += shape.dy;
      if ((shape.x - shape.r) < 0) {
        shape.dx *= -1;
        shape.x = shape.r;
      }
      if ((shape.x + shape.r) >= graphics->bounds.w) {
        shape.dx *= -1;
        shape.x = graphics->bounds.w - shape.r;
      }
      if ((shape.y - shape.r) < 0) {
        shape.dy *= -1;
        shape.y = shape.r;
      }
      if ((shape.y + shape.r) >= graphics->bounds.h) {
        shape.dy *= -1;
        shape.y = graphics->bounds.h - shape.r;
      }

      graphics->set_pen(shape.pen);
      graphics->circle(Point(shape.x, shape.y), shape.r);
    }

    graphics->set_pen(WHITE);
    graphics->text("Hello World", text_location, 320);

    // update screen
    st7789->update(graphics);

    i += 10;
    tufty.led(i);
  }
}