#include "src/common.hpp"

// Include all programs:
#include "src/balls.hpp"
#include "src/menu.hpp"
#include "src/badge.hpp"

int main() {
  createSt7789(pico::ROTATE_180);
  createGraphics(st7789);
  st7789->set_backlight(255);
  menu();
  return 0;
}