# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: satkins <satkins@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/25 14:05:53 by satkins           #+#    #+#              #
#    Updated: 2018/04/21 12:11:18 by satkins          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# NAME = libft_malloc_
# NAME += $(HOSTTYPE)
# NAME += .so


ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_${HOSTTYPE}.so

SRCS_DIR = srcs



LIBPATH = libft/

LIBFT = libftprintf.a

CC = gcc

FLAGS = -Wall -Wextra -Werror

TFLAGS = -Wall -Wextra -Werror -g -fsanatize=address

CFLAGS = -Wall -Wextra -Werror -c -fPIC

LFLAGS = -g -shared

TARG = $(patsubst %, obj/%.o, $(SRCS))

################################################################################
# SOURCE FILES                                                                 #
################################################################################

SRC_MALLOC =  \
		ft_malloc \
		ft_realloc \
		free

################################################################################
# Source directories identifiers                                               #
################################################################################

SRCDIR_MALLOC = src/

OBJSRC = $(patsubst %, %.o, $(addprefix $(SRCDIR_MALLOC), $(SRC_MALLOC)))

################################################################################
# INCLUDE PATHS                                                                #
################################################################################

INCLUDES = \
		-I libft/inc \
		-I inc

################################################################################
# COLOR                                                                        #
################################################################################

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
CYAN = \033[1;36m
RES = \033[0m

################################################################################
# RULES                                                                        #
################################################################################

all: $(NAME)

$(NAME): $(LIBFT) $(OBJSRC)
	@ echo "$(YELLOW)Compiling program$(RES)"
	@$(CC) $(LFLAGS) -L $(LIBPATH) -lftprintf $(INCLUDES) $(OBJSRC) -o $(NAME)
	@rm -f libft_malloc.so
	@ln -s $@ libft_malloc.so
	@echo "$(GREEN)Shared Lib Compiled$(RES)"


$(LIBFT):
	@make -C $(LIBPATH)

%.o: %.c
	@ echo "$(YELLOW)Compiling $<...$(RES)"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJSRC)
	@make -C $(LIBPATH) clean
	@ echo "$(RED)Cleaning folders of object files...$(RES)"

fclean: clean
	rm -f $(NAME)
	rm -f libft_malloc.so
	@make -C $(LIBPATH) fclean
	@ echo "$(RED)Removing library file and binary...$(RES)"

re: fclean all
	@ echo "$(GREEN)Shared Lib Remade$(RES)"

#debug: re
#	@$(CC) $(TFLAGS) -I$(HEADER) -I$(LIBHEADER) -o $(NAME) $(SRCS) -L$(LIBPATH) -l$(LIBNAME) $(LIB_LINK)
