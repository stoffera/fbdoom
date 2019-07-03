CXX=g++
CC=gcc

SOURCES = $(patsubst src/%, %, $(wildcard src/*.c))
SOURCES += device/main.c device/i_fb_video.c device/i_no_sound.c device/i_no_music.c
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
TARGET_OBJS = $(patsubst %, build/%, $(OBJECTS))
CCFLAGS = -DNORMALUNIX -std=gnu99

.PHONY:
all: doom

.PHONY: sources
sources:
	@echo $(SOURCES)

.PHONY: objects
objects:
	@echo $(TARGET_OBJS)

build:
	mkdir -p build/device

build/%.o: src/%.c
	@mkdir -p build/device
	@echo "Compiling $<..."
	@$(CC) -c $< -I src $(CCFLAGS) -g -o $@

build/device/%.o: src/device/%.c
	@echo "Compiling $<..."
	@$(CC) -c $< -I src $(CCFLAGS) -g -o $@

.PHONY:
link: $(TARGET_OBJS)
	@echo "Linking..."
	@$(CC) -ggdb -o doom $(TARGET_OBJS)

doom: link