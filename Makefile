# The name of your project (used to name the compiled .hex file)
TARGET = LEDBoard

# set your MCU type here, or make command line `make MCU=MK20DX256`
MCU = MK66FX1M0

# make it lower case
LOWER_MCU := $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$(MCU)))))))))))))))))))))))))))
MCU_LD = $(LOWER_MCU).ld

# use "cortex-m4" for Teensy 3.x
# use "cortex-m0plus" for Teensy LC
CPUARCH = cortex-m4

ARDUINOPATH = C:/Arduino

# path location for Teensy Loader, teensy_post_compile and teensy_reboot (on Linux)
TOOLSPATH = $(ARDUINOPATH)/hardware/tools

# path location for Arduino libraries (currently not used)
LIBRARYPATH = $(ARDUINOPATH)/libraries

# path location for the arm-none-eabi compiler
COMPILERPATH = $(ARDUINOPATH)/hardware/tools/arm/bin

INC = -I. \
      -Icommon \
      -Iconsole \
      -Icontrol \
      -Iio \
      -Imode \
      -Ischeduler \
      -Iteensy3 \
      -Iteensy3/avr \
      -Iteensy3/util

OBJDIR = ./bins

ELF = $(OBJDIR)/$(TARGET).elf
HEX = $(OBJDIR)/$(TARGET).hex
MAP = $(OBJDIR)/$(TARGET).map

#************************************************************************
# Configurable options
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall \
           -MMD \
           -c \
           -Os \
           -g \
           -mthumb \
           -DLAYOUT_US_ENGLISH \
           -DUSB_SERIAL \
           -D__PROG_TYPES_COMPAT__ \
           -DARDUINO_ARCH_ARM \
           -mcpu=$(CPUARCH) \
           -DF_CPU=240000000 \
           -D__$(MCU)__ \
           -DARDUINO=185 \
           -DTEENSYDUINO=141 \
           -ffunction-sections \
           -fdata-sections \
           -nostdlib \
           -mfloat-abi=hard \
           -mfpu=fpv4-sp-d16 \
           -fsingle-precision-constant \
           -flto \
           -fdiagnostics-color \
           $(INC)
           

# compiler options for C++ only
CXXFLAGS = -std=gnu++11 \
           -fno-exceptions \
           -felide-constructors \
           -fno-rtti \
           -fpermissive

# compiler options for C only
CFLAGS =  -std=gnu11 \
          -fno-fat-lto-objects 

# linker options
LDFLAGS = -Os \
          -mfloat-abi=hard \
          -mfpu=fpv4-sp-d16 \
          -Wl,--Map=$(MAP),--gc-sections,--relax \
          -mcpu=$(CPUARCH) \
          -fsingle-precision-constant \
          -fuse-linker-plugin \
          -flto \
          -mthumb \
          -T$(MCU_LD)

# additional libraries to link
LIBS = -lm -lc -larm_cortexM4lf_math

# names for the compiler programs
CC      = $(COMPILERPATH)/arm-none-eabi-gcc
CXX     = $(COMPILERPATH)/arm-none-eabi-g++
OBJCOPY = $(COMPILERPATH)/arm-none-eabi-objcopy
SIZE    = $(COMPILERPATH)/arm-none-eabi-size

# automatically create lists of the sources and objects
C_FILES   := $(wildcard *.c) $(wildcard */*.c)
CPP_FILES := $(wildcard *.cpp) $(wildcard */*.cpp)
AS_FILES  := $(wildcard *.S) $(wildcard */*.S)
OBJS      := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(AS_FILES:.S=.o)

# the actual makefile rules (all .o files built by GNU make's default implicit rules)

all: $(HEX)

$(ELF): $(OBJS) $(ASM_OBJS) $(MCU_LD)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d $(wildcard */*.o) $(wildcard */*.d) $(ELF) $(HEX) $(MAP)
