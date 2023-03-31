# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/03 16:11:01 by zrebhi            #+#    #+#              #
#    Updated: 2023/03/30 16:25:10 by zrebhi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		   :=    minishell

INC            :=    minishell.h
INC_DIR        :=    includes/

HEADERS        := $(addprefix $(INC_DIR), $(INC))

SRC_DIR        :=    sources/
SRC            :=    cmdlist.c cmdlist_utils.c env_list_utils.c error.c exec.c \
					 expand.c expand_utils.c expand_heredoc.c heredoc.c main.c utils.c \
					 redirections.c signals.c signals_handlers.c builtins/builtins_env.c\
					 builtins/builtins_cd.c builtins/builtins_export.c parse_env.c \
					 builtins/builtins_unset.c builtins/builtins.c builtins/builtins_exit.c \
					 builtins/builtins_pwd.c builtins/builtins_echo.c builtins/builtins_export2.c

LIBFT_DIR	   :=    libft/
LIBFT_A		   :=    $(LIBFT_DIR)libft.a

OBJ_DIR        :=    build/
OBJ            :=    $(SRC:%.c=$(OBJ_DIR)%.o)

# Compiler options
CC            :=    cc
CC_FLAGS      :=    -Wextra -Werror -Wall
DEBUG_FLAG    :=    -ggdb3

# define standard colors
_END        :=    \x1b[0m
_BOLD       :=    \x1b[1m
_UNDER      :=    \x1b[4m
_REV        :=    \x1b[7m
_GREY       :=    \x1b[30m
_RED        :=    \x1b[31m
_GREEN      :=    \x1b[32m
_YELLOW     :=    \x1b[33m
_BLUE       :=    \x1b[34m
_PURPLE     :=    \x1b[35m
_CYAN       :=    \x1b[36m
_WHITE      :=    \x1b[37m

all: $(NAME)

$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.c $(LIBFT_DIR) $(HEADERS) ${LIBFT_A}
	@mkdir -p $(@D)
	@echo "$(_GREEN)compiling: $<$(_END)"
	@$(CC) $(CC_FLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT_A) $(INC_DIR)/$(INC) Makefile
	@$(CC) $(CC_FLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $@
	@echo "> minishell Done!\n"

${LIBFT_A}: FORCE
	@$(MAKE) -C ${LIBFT_DIR}
    
FORCE:

# Libft Makefile
build_libft:
	@$(MAKE) -C $(LIBFT_DIR)


# clean commands
clean:
	@$(MAKE) clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "remove $(NAME)"
	@rm -rf $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re build_libft
