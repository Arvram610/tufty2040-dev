
#include "common.hpp"
#include "util/jpeg.hpp"
#include "img/selflove.h"

REGISTER_RUNNABLE(trovy_badge, 1) {
  // update screen
  JPEGImage img(graphics, data::selflove, sizeof(data::selflove));
  graphics->set_font("serif_italic");
  while (1) {
    graphics->set_pen(0, 255, 255);
    graphics->clear();
    img.draw(0,0,0,1);


    graphics->set_font("serif_italic");
    graphics->set_thickness(3);
    graphics->set_pen(0,0,0);
    graphics->text("Trovy", {180, 30}, 140, 1.4f);  // Outline
    graphics->set_thickness(2);
    graphics->set_pen(255,178,102);
    graphics->text("Trovy", {180, 30}, 140, 1.4f);
   

    graphics->set_font("serif");
    graphics->set_pen(0,0,0); 
    graphics->set_thickness(3);
    graphics->text("Arctic fox", {185, 65}, 140, 0.80f);
    graphics->set_thickness(2);
    graphics->set_pen(255,100,100);
    graphics->text("Arctic fox", {185, 65}, 140, 0.8f);

    graphics->set_font("serif");
    graphics->set_pen(0,0,0); 
    graphics->set_thickness(3);
    graphics->text("He/them", {185, 90}, 140, 0.85f);
    graphics->set_thickness(2);
    graphics->set_pen(255,100,100);
    graphics->text("He/them", {185, 90}, 140, 0.85f);

    graphics->set_font("serif");
    graphics->set_pen(0,0,0); 
    graphics->set_thickness(3);
    graphics->text("Bisexual", {185, 115}, 140, 0.85f);
    graphics->set_thickness(2);
    graphics->set_pen(255,100,100);
    graphics->text("Bisexual", {185, 115}, 140, 0.85f);

    st7789->update(graphics);
  }
}