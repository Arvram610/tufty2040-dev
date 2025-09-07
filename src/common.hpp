#pragma once

#include "tufty2040.hpp"
#include "st7789.hpp"
#include <button.hpp>

namespace pico = pimoroni;

pico::Button button_a(pico::Tufty2040::A, pico::Polarity::ACTIVE_HIGH);
pico::Button button_b(pico::Tufty2040::B, pico::Polarity::ACTIVE_HIGH);
pico::Button button_c(pico::Tufty2040::C, pico::Polarity::ACTIVE_HIGH);
pico::Button button_up(pico::Tufty2040::UP, pico::Polarity::ACTIVE_HIGH);
pico::Button button_down(pico::Tufty2040::DOWN, pico::Polarity::ACTIVE_HIGH);

pico::ST7789 getSt7789(pico::Rotation rotation) {
    using namespace pico;
    return {Tufty2040::WIDTH, Tufty2040::HEIGHT, rotation,
              ParallelPins{Tufty2040::LCD_CS, Tufty2040::LCD_DC,
                           Tufty2040::LCD_WR, Tufty2040::LCD_RD,
                           Tufty2040::LCD_D0, Tufty2040::BACKLIGHT}};
}

