#!/bin/bash

cmake . -B build -G Ninja && ninja -C build && picotool load build/Tufty2040Epic.uf2 && picotool reboot
