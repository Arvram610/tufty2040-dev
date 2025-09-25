#pragma once

#include <stdio.h>

#include <algorithm>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

#include "common.hpp"

REGISTER_RUNNABLE(test123, 0) {exit(0);}
REGISTER_RUNNABLE(test321, 0) {exit(0);}

// Menu, automatically starts on launch
int menu() {
  using namespace pimoroni;

  int selected_item = 0;
  double scroll_position = 0, target_scroll_position = 0;

  Pen selected_pen = graphics->create_pen(255, 255, 255);
  Pen unselected_pen = graphics->create_pen(80, 80, 100);
  Pen background_pen = graphics->create_pen(50, 50, 70);
  Pen shadow_pen = graphics->create_pen(0, 0, 0);

  uint32_t lastTime = 0u, thisTime, pressTime = 0;
  std::vector<Runnable> *currentList = &programs;

  std::deque<char> inputBuffer{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<char> targetBuffer{'u', 'u', 'd', 'd', 'l', 'r',
                                'l', 'r', 'b', 'l', 's'};  // For secret :3

  int selectCooldown;
  while (true) {
    thisTime = time();

    scroll_position += (target_scroll_position - scroll_position) / 5;
    // work out which item is selected (closest to the current scroll position)
    selected_item = round(target_scroll_position);

    if (button_up.read()) {
      inputBuffer.pop_front();
      inputBuffer.push_back('u');
      target_scroll_position -= 1;
      target_scroll_position = target_scroll_position >= 0
                                   ? target_scroll_position
                                   : currentList->size() - 1;
    }
    if (button_down.read()) {
      inputBuffer.pop_front();
      inputBuffer.push_back('d');
      target_scroll_position += 1;
      target_scroll_position = target_scroll_position < currentList->size()
                                   ? target_scroll_position
                                   : 0;
    }
    if (button_user.read() && thisTime - pressTime > 1000) {
      inputBuffer.pop_front();
      inputBuffer.push_back('s');

      if (std::equal(inputBuffer.begin(), inputBuffer.end(),
                     targetBuffer.begin())) {  // Secret :3
        pressTime = thisTime;
        currentList = currentList == &programs ? &hidden_programs : &programs;
        selected_item = 0;  // Reset scroll
        scroll_position = 0;
        target_scroll_position = 0;
        continue;
      }
      do {
      } while (button_user.raw());
      graphics->set_pen(0, 0, 0);
      graphics->clear();
      st7789->update(graphics);
      currentList->at(selected_item)();
    }
    if (button_a.read()) {
      inputBuffer.pop_front();
      inputBuffer.push_back('l');
    }
    if (button_b.read()) {
      inputBuffer.pop_front();
      inputBuffer.push_back('b');
    }
    if (button_c.read()) {
      inputBuffer.pop_front();
      inputBuffer.push_back('r');
    }

    graphics->set_pen(background_pen);
    graphics->clear();

    static int thickness = 2;
    for (int y = 0; y < st7789->height / thickness; ++y) {
      float h = 70 + y / (3*thickness);
      h = h / 30;
      graphics->set_pen(graphics->create_pen_hsv(h, 0.85, 1));
      graphics->rectangle({0, y * thickness, st7789->width, thickness});
    }

    for (int list_index = 0; list_index < currentList->size(); ++list_index) {
      double distance = list_index - scroll_position;

      int text_size = selected_item == list_index ? 4 : 3;

      // center text horixontally
      int title_width = graphics->measure_text(currentList->at(list_index).name,
                                               text_size, 1, false);
      int text_x = (int)(160 - title_width / 2);

      int row_height = text_size * 5 + 20;

      // center list items vertically
      int text_y = (int)(120 + distance * row_height - (row_height / 2));

      // draw the text, selected item brightest and with shadow
      if (selected_item == list_index) {
        graphics->set_pen(shadow_pen);
        graphics->text(currentList->at(list_index).name,
                       {text_x + 1, text_y + 1}, -1, text_size);
      }

      Pen text_pen =
          selected_item == list_index ? selected_pen : unselected_pen;
      graphics->set_pen(text_pen);
      graphics->text(currentList->at(list_index).name, {text_x, text_y}, -1,
                     text_size);
    }

    graphics->text(std::to_string(st7789->height), {0, 0}, 320);
    //graphics->text(std::to_string(1000 / (thisTime - lastTime)), {0, 0}, 320);
    lastTime = thisTime;

    // update screen
    st7789->update(graphics);
  }
}