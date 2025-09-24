#include "src/common.hpp"

// Include all programs:
#include "src/balls.cpp"
#include "src/menu.cpp"

int main() {
  createSt7789(pico::ROTATE_180);
  createGraphics(st7789);
  st7789->set_backlight(255);
  menu();
  return 0;
}