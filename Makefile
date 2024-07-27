CXX								:= clang++
HEADERS						:= -Iinclude -Icolor
CXXFLAGS					:= -Wall -Wextra -Werror -g3 -std=c++98 
TARGET						:= ircserv

SRC_PATH					:= src
OBJ_PATH					:= obj

SRC 							:= $(shell find $(SRC_PATH) -name '*.cpp')
OBJ 							:= $(SRC:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)

MAKEFLAGS   			+= --no-print-directory
RM          			:= rm -rf

TOTAL_FILES     	:= $(words $(SRC))
CURRENT_INDEX   	:= 0

all:	$(TARGET)

$(TARGET):	$(OBJ)
	@$(CXX) $(OBJ) -o $@
	@echo "\n$(BOLD)┗▷$(GREEN)『./$(TARGET) Successfully created [✅]$(RESET)"

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.cpp
	@mkdir -p $(@D)
	@$(eval CURRENT_INDEX=$(shell echo $$(($(CURRENT_INDEX)+1))))
	@$(eval PERCENT=$(shell echo $$(($(CURRENT_INDEX) * 100 / $(TOTAL_FILES)))))
	@printf "\r$(YELLOW)🔧 $(GREEN)%3d%% $(YELLOW)$(BOLD)Compiling: $(RESET)$(BLUE)$(ITALIC)%-50s $(MAGENTA)[%3d/%3d]$(RESET)" $(PERCENT) "$<" $(CURRENT_INDEX) $(TOTAL_FILES)
	@$(CXX) $(CXXFLAGS) $(HEADERS) -c $< -o $@

clean:
	@echo "$(BOLD) [ 🗑️ ] $(YELLOW)$(REVERSED)Cleaning up$(RESET)"
	@$(RM) $(OBJ_PATH)
	@echo "┗▷$(YELLOW)『$(ITALIC)./$(TARGET)'s$(RESET)$(YELLOW) object files cleaned』$(RESET)"

fclean: clean
	@$(RM) $(TARGET)
	@echo "┗▷$(YELLOW)『$(ITALIC)./$(TARGET)'s$(RESET)$(YELLOW) executable cleaned』$(RESET)"

re: fclean all

.PHONY: all clean fclean re

# Colors
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
MAGENTA := \033[0;35m
CYAN := \033[0;36m
WHITE := \033[0;37m
RESET := \033[0m
BOLD := \033[1m
UNDERLINE := \033[4m
REVERSED := \033[7m
ITALIC := \033[3m