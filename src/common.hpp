#pragma once

#include <time.h>

#include <button.hpp>
#include <pico_graphics.hpp>
#include <st7789.hpp>
#include <tufty2040.hpp>
#include <vector>

#include "pico.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico_graphics.hpp"
#include "pimoroni_common.hpp"

namespace pico = pimoroni;

pico::Tufty2040 tufty;

pico::Button button_a(pico::Tufty2040::A, pico::Polarity::ACTIVE_HIGH);
pico::Button button_b(pico::Tufty2040::B, pico::Polarity::ACTIVE_HIGH);
pico::Button button_c(pico::Tufty2040::C, pico::Polarity::ACTIVE_HIGH);
pico::Button button_up(pico::Tufty2040::UP, pico::Polarity::ACTIVE_HIGH);
pico::Button button_down(pico::Tufty2040::DOWN, pico::Polarity::ACTIVE_HIGH);
pico::Button button_user(23, pico::Polarity::ACTIVE_LOW);

static pico::ST7789 *st7789 = nullptr;
static pico::PicoGraphics *graphics = nullptr;

void createSt7789(pico::Rotation rotation) {
  using namespace pico;
  if (st7789) {
    st7789->cleanup();
    st7789->power_off();
    delete st7789;
  }
  st7789 = new ST7789(
      Tufty2040::WIDTH, Tufty2040::HEIGHT, rotation,
      ParallelPins{Tufty2040::LCD_CS, Tufty2040::LCD_DC, Tufty2040::LCD_WR,
                   Tufty2040::LCD_RD, Tufty2040::LCD_D0, Tufty2040::BACKLIGHT});
}

void createGraphics(pico::ST7789 *st7789) {
  if (graphics) {
    delete[] (uint8_t *)graphics->frame_buffer;  // Why does it not delete the buffer on delete!?
    delete graphics;
  }
  graphics =
      new pico::PicoGraphics_PenRGB565(st7789->width, st7789->height, nullptr);
}

uint32_t time() {
  absolute_time_t t = get_absolute_time();
  return to_ms_since_boot(t);
}

typedef void (*RunnableFn)(void);
class Runnable {
 private:
  const RunnableFn fn;

 public:
  std::string name;
  Runnable(const RunnableFn fn, const std::string _name) : fn(fn) , name(_name){
    std::replace(name.begin(), name.end(), '_', ' ');
  }

  void operator()() { fn(); }
};

// Use a function to hold the static vector (guaranteed initialized before use)
static std::vector<Runnable> &_programs() {
  static std::vector<Runnable> _programs;
  return _programs;
}
// Use a function to hold the static vector (guaranteed initialized before use)
static std::vector<Runnable> &_hidden_programs() {
  static std::vector<Runnable> _hprograms;
  return _hprograms;
}

std::vector<Runnable> &hidden_programs = _hidden_programs();
std::vector<Runnable> &programs = _programs();

static inline void _register_runnable(Runnable fn, bool hidden) {
  if (hidden) {
    _hidden_programs().push_back(fn);
  } else {
    _programs().push_back(fn);
  }
}

#define REGISTER_RUNNABLE(name, hidden)                            \
  static void name##_registrar(void) __attribute__((constructor)); \
  [[noreturn]] static void name();                                 \
  static void name##_registrar(void) {                             \
    _register_runnable(Runnable(name, #name), hidden);             \
  }                                                                \
  [[noreturn]] static void name()
