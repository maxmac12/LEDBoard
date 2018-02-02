TARGET = LEDBoard

BOARD_TAG = teensy36

PROJECT_DIR = D:/workspace/C++/LEDBoard
ARDUINO_DIR = C:/Arduino
ARDMK_DIR = D:/workspace/C++/Arduino-Makefile

USER_LIB_PATH = $(PROJECT_DIR)/common \
      $(PROJECT_DIR)/console \
      $(PROJECT_DIR)/ledControl

INC = -I. \
      -I$(PROJECT_DIR)/common \
      -I$(PROJECT_DIR)/console \
      -I$(PROJECT_DIR)/ledControl

CFLAGS += $(INC)
CXXFLAGS += $(INC)

RESET_CMD = $(ARDUINO_DIR)/hardware/tools/teensy_restart

include $(ARDMK_DIR)/Teensy.mk
include $(ARDMK_DIR)/Arduino.mk
