TARGET = SDLRaycaster
CC = gcc
CFLAGS =	-Wall -Wextra -Wno-deprecated-declarations -arch x86_64 -Ofast -march=native -flto -ffast-math \
			-funroll-loops -fno-exceptions -fomit-frame-pointer
			
LDFLAGS = -arch x86_64 -L$(LIBS)/lib -L/usr/local/lib -lpthread  \
          -force_load $(LIBS)/lib/libSDL2.a \
          -force_load $(LIBS)/lib/libSDL2_image.a \
          -force_load $(LIBS)/lib/libSDL2_ttf.a \
          -force_load $(LIBS)/lib/libSDL2_mixer.a \
          -force_load $(LIBS)/lib/libFLAC.a \
          -force_load $(LIBS)/lib/libmpg123.a \
          -force_load $(LIBS)/lib/libogg.a \
          -force_load $(LIBS)/lib/libvorbis.a \
          -force_load $(LIBS)/lib/libvorbisfile.a \
          -force_load $(LIBS)/lib/libopusfile.a \
          -force_load $(LIBS)/lib/libmodplug.a \
          -force_load $(LIBS)/lib/libxmp.a \
          -force_load $(LIBS)/lib/libwavpack.a \
          -force_load $(LIBS)/lib/libfluidsynth.a \
          -force_load $(LIBS)/lib/libportaudio.a \
          -force_load $(LIBS)/lib/libglib-2.0.a \
          -force_load $(LIBS)/lib/libgme.a \
          -force_load $(LIBS)/lib/libsndfile.a \
          -force_load $(LIBS)/lib/libz.a \
          -force_load $(LIBS)/lib/libmp3lame.a \
          -force_load $(LIBS)/lib/libintl.a \
          -force_load $(LIBS)/lib/libopus.a \
          -force_load $(LIBS)/lib/libvorbisenc.a \
          -force_load $(LIBS)/lib/libiconv.a \
          -Wl,-alias_list,$(LIBS)/iconv.alias \
          -lc++ -lpng -liconv \
          -framework Cocoa -framework IOKit -framework CoreVideo \
          -framework CoreFoundation -framework AudioToolbox -framework ForceFeedback \
          -framework GameController -framework Metal -framework CoreHaptics \
          -framework CoreAudio -framework Carbon



LIBS = libs

INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
SRC = $(shell find $(SRC_DIR) -type f -name "*.c")
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR) -I$(LIBS)/include

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

fclean: clean

run:
	export DYLD_LIBRARY_PATH=./libs/lib:$$DYLD_LIBRARY_PATH; ./$(TARGET)
