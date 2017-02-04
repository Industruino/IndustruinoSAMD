#!/bin/bash -ex

BOARD_ID=industruino_d21g NAME=samd21_sam_ba_industruino_d21g SAM_BA_INTERFACE=SAM_BA_USBCDC_ONLY make clean all

echo Done building bootloaders!

