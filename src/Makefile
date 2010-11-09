##
## Makefile for luasoul in /home/papin_g
## 
## Made by Guillaume Papin
## Login   <guillaume.papin@epitech.eu>
## 
## Started on  Wed Oct  6 00:43:13 2010 Guillaume Papin
## Last update Mon Nov  8 22:38:16 2010 Guillaume Papin
##

NAME		= luasoul

INC		= ./

SRCS		= ./lua_ui.c	\
		./utils.c	\
		./lua/ooutils.c	\
		./lua/lua_utils.c	\
		./ui/window.c	\
		./ui/chatbox.c	\
		./ui/style.c	\
		./ui/input.c	\
		./ui/ui_utils.c	\
		./luasoul.c

OBJ		= $(SRCS:%.c=%.o)

# _BSD_SOURCE for strdup()
CFLAGS		= -Wall -Wextra -Wshadow -ansi -pedantic -Wwrite-strings \
		-Wstrict-prototypes -Wuninitialized -I$(INC) -I../md5/ \
		-D_BSD_SOURCE -D_POSIX_SOURCE

DEBUG		= -ggdb3

LDFLAGS		= -llua -lpanel -lncurses

CC		?= gcc

RM		= rm -f

VALGRIND	= valgrind --fullpath-after=./

PTY		= $(shell tty)

all		: $(NAME)

$(NAME)		: $(OBJ)
		@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)
		@echo -e "  [OUT]\t\t\e[1;34m$(NAME)\e[0m"

%.o		: %.c
		@echo -e "  [CC]\t\t"$<
		@$(CC) $(CFLAGS) -o $@ -c $<

clean		:
		@echo -e "  [RM]\t\t$(OBJ)"
		@$(RM) $(OBJ)

fclean		: clean
		@echo -e "  [RM]\t\t$(NAME)"
		@$(RM) $(NAME)

re		: clean all

debug		: clean
		$(CC) $(DEBUG) $(CFLAGS) -o $(NAME) $(SRCS) $(LDFLAGS)

valgrind	: debug
		urxvtc -e sh -c "$(VALGRIND) ./$(NAME) 2>$(PTY)"

malloc		: debug
		urxvtc -e sh -c "$(VALGRIND) --tool=memcheck --leak-check=yes \
		--show-reachable=yes ./$(NAME) 2>$(PTY)"

check-syntax	:
		gcc $(CFLAGS) -Wall -Wextra -Wshadow -ansi -pedantic -Wwrite-strings \
		-Wstrict-prototypes -Wuninitialized -fsyntax-only $(CHK_SOURCES)
