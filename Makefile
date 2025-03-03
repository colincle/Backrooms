TARGET = SDLRaycaster
CC = gcc
ARCH ?= $(shell uname -m) # Default to current architecture
LIBS = libs
VENDOR_MAKEFILE = vendor/Makefile

CFLAGS =	-Wall -Wextra -Wno-deprecated-declarations -arch $(ARCH) -Ofast -march=native -flto -ffast-math \
			-funroll-loops -fno-exceptions -fomit-frame-pointer -mmacosx-version-min=10.15

DEBUG_FLAGS = -fsanitize=address -g

LDFLAGS = -arch $(ARCH) -L$(LIBS)/lib -L/usr/local/lib -lpthread \
          -force_load $(LIBS)/lib/libSDL2.a \
          -force_load $(LIBS)/lib/libSDL2_image.a \
          -force_load $(LIBS)/lib/libSDL2_mixer.a \
          -Wl,-undefined,dynamic_lookup \
          -mmacosx-version-min=10.15 -lc++ -lpng \
          -framework Cocoa -framework IOKit -framework CoreVideo \
          -framework CoreFoundation -framework AudioToolbox -framework ForceFeedback \
          -framework GameController -framework Metal -framework CoreHaptics \
          -framework CoreAudio -framework Carbon

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
BLUE        = \033[0;94m
YELLOW      = \033[0;93m
BAR_COLOR   = \033[42m
RESET_COLOR = \033[0m

# Check if libs exist; if not, build them
ifeq ($(wildcard $(LIBS)),)
$(shell $(MAKE) -C vendor $(ARCH))
endif

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
	@printf "$(BLUE)\n"
	@printf "  ____	 	   _								  \n"
	@printf " |  _ \\		  | |								 \n"
	@printf " | |_) | __ _  ___| | ___ __ ___   ___  _ __ ___  ___ \n"
	@printf " |  _ < / _\` |/ __| |/ | '__/ _ \\ / _ \\| '_ \` _ \\/ __|\n"
	@printf " | |_) | (_| | (__|   <| | | (_) | (_) | | | | | \\__ \\ \n"
	@printf " |____/ \\__,_|\\___|_|\\_|_|  \\___/ \\___/|_| |_| |_|___/\n"
	@printf "$(RESET_COLOR)\n"
	@printf "\033[1;36m---------------------------------------\033[0m\n"
	@printf "\033[1;32m CONTROLS:\033[0m\n"
	@printf "\033[1;34m ➤ Look Around:\033[0m ⌨️ Mouse / 🎮 Right stick\n"
	@printf "\033[1;34m ➤ Move:\033[0m ⌨️ WASD / 🎮 Left Stick\n"
	@printf "\033[1;34m ➤ Run:\033[0m ⌨️ Shift / 🎮 L3\n"
	@printf "\033[1;34m ➤ Jump:\033[0m ⌨️ Space / 🎮 Cross\n"
	@printf "\033[1;34m ➤ Crouch:\033[0m ⌨️ C / 🎮 Circle\n"
	@printf "\033[1;34m ➤ Crawl:\033[0m ⌨️ Hold C / 🎮 Hold Circle\n"
	@printf "\033[1;36m---------------------------------------\033[0m\n"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)$(TARGET) object files cleaned!$(DEF_COLOR)"

fclean: clean
	@rm -rf $(TARGET)
	@echo "$(GREEN)$(TARGET) executable files cleaned!$(DEF_COLOR)"

re: fclean all

x86_64:
	$(MAKE) ARCH=x86_64

arm64:
	$(MAKE) ARCH=arm64

debug: $(OBJ)
	@$(CC) $(DEBUG_FLAGS) $(OBJ) -o $(TARGET)
	@echo "\n$(GREEN)Rebuilt everything for $(TARGET) in debug mode!$(DEF_COLOR)"

run:
	@export DYLD_LIBRARY_PATH=./libs/lib:$$DYLD_LIBRARY_PATH; ./$(TARGET)

.PHONY: all clean fclean re debug run x86_64 arm64
