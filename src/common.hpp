#pragma once

#include "tufty2040.hpp"
#include "st7789.hpp"
#include <button.hpp>
#include "pico_graphics.hpp"

#include <vector>

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
static pico::PicoGraphics_PenRGB565 *graphics = nullptr;
void createGraphics(pico::ST7789 st7789) {
    if (graphics) {
        delete graphics;
    }
    graphics = new pico::PicoGraphics_PenRGB565(st7789.width, st7789.height, nullptr);
}


typedef void (*RunnableFn)(void);
class Runnable{
private:
    const RunnableFn fn;
public:
    const std::string name;
    Runnable(RunnableFn fn, std::string name): fn(fn), name(name) {}

    void operator()(){
        fn();
    }
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

#define REGISTER_RUNNABLE(name, hidden) \
    static void __name##_registrar(void) __attribute__((constructor)); \
    static void name(); \
    static void __name##_registrar(void) { _register_runnable(Runnable(name, #name), hidden); } \
    static void name()
