# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/26 10:08:44 by mjallada          #+#    #+#              #
#    Updated: 2022/05/26 10:08:46 by mjallada         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# VARIABLES

NAME	:= philo

CFLAGS	:= -Wall -Wextra -Werror

INC_DIR	:= include
SRC_DIR	:= src
BIN_DIR	:= bin

# **************************************************************************** #
# COLORS

GREEN		:= \033[0;92m
YELLOW		:= \033[0;93m
BLUE		:= \033[0;94m
PURPLE		:= \033[0;35m
IPURPLE		:= \033[3;35m
END_COLOR	:= \033[0;39m

# **************************************************************************** #
# SOURCES

SRC_FILES   :=	main\
				init\
				errors\
				optimization\
				philos\
				setup\
				utils
OBJ_FILES	:=	$(addprefix $(BIN_DIR)/, $(addsuffix .o, $(SRC_FILES)))

# **************************************************************************** #
# RULES

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CC) -pthread -o $(NAME) $(OBJ_FILES)
	@echo "$(GREEN)$(NAME) compiled :) $(END_COLOR)"

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	@$(CC) -g -MD -c $(CFLAGS) -I $(INC_DIR) $< -o $@
	@echo "$(BLUE)> compiling $(notdir $<)$(END_COLOR)"

$(BIN_DIR):
	@mkdir $(BIN_DIR)
	@echo "$(IPURPLE)Created $(BIN_DIR)/ directory$(END_COLOR)"

clean:
	@rm -rf $(BIN_DIR)
	@echo "$(YELLOW)$(NAME) object & dependency files cleaned.$(END_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)$(NAME) executable file cleaned.$(END_COLOR)"

re: fclean all
	@echo "Cleaned all and rebuilt $(NAME)!"

-include $(OBJ_FILES:%.o=%.d)

# **************************************************************************** #
# PHONY

.PHONY: all clean fclean re
