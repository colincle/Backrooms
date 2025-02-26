TARGET = SDLRaycaster
CC = gcc
CFLAGS =	-Wall -Wextra -Wno-deprecated-declarations -arch x86_64 -Ofast -march=native -flto -ffast-math \
			-funroll-loops -fno-exceptions -fomit-frame-pointer -mmacosx-version-min=13.0

DEBUG_FLAGS = -fsanitize=address -g

LDFLAGS = -arch x86_64 -L$(LIBS)/lib -L/usr/local/lib -lpthread \
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
          -Wl,-undefined,dynamic_lookup \
          -mmacosx-version-min=14.3 \
          -lc++ -lpng \
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
TOTAL_SRCS = $(words $(SRC))

# Colors
DEF_COLOR   = \033[0;39m
GREEN       = \033[0;92m
YELLOW      = \033[0;93m
BAR_COLOR   = \033[42m
RESET_COLOR = \033[0m

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(@D)
	@BAR_WIDTH=30; \
	NUM_OBJS=$$(find $(OBJ_DIR) -type f | wc -l); \
	COMPLETED=$$(((NUM_OBJS + 1) * BAR_WIDTH / $(TOTAL_SRCS))); \
	FILENAME=$$(basename "$<"); \
	printf "\r\033[K$(BAR_COLOR)%-*s$(RESET_COLOR)%-*s $(YELLOW)%s$(DEF_COLOR)" \
		$$COMPLETED "" $$((BAR_WIDTH - COMPLETED)) "" "$$FILENAME"; \
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR) -I$(LIBS)/include

$(TARGET): $(OBJ)
	@printf "\r\033[K"
	@printf "$(GREEN)Linking $(TARGET)...$(DEF_COLOR)\n"
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)
	@echo "$(GREEN)All done 👍$(DEF_COLOR)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)$(TARGET) object files cleaned!$(DEF_COLOR)"

fclean: clean
	@rm -rf $(TARGET)
	@echo "$(GREEN)$(TARGET) executable files cleaned!$(DEF_COLOR)"

re: fclean all

debug: $(OBJ)
	@$(CC) $(DEBUG_FLAGS) $(OBJ) -o $(TARGET)
	@echo "\n$(GREEN)Rebuilt everything for $(TARGET) in debug mode!$(DEF_COLOR)"

run:
	@export DYLD_LIBRARY_PATH=./libs/lib:$$DYLD_LIBRARY_PATH; ./$(TARGET)

.PHONY: all clean fclean re debug run
