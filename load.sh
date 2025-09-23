#!/bin/bash

cmake build && ninja -C build && picotool load build/Tufty2040Epic.uf2 && picotool reboot
