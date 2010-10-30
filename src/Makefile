##
## Makefile for luasoul in /home/papin_g
## 
## Made by Guillaume Papin
## Login   <papin_g@epitech.net>
## 
## Started on  Wed Oct  6 00:43:13 2010 Guillaume Papin
## Last update Sat Oct 30 15:29:26 2010 Guillaume Papin
##

NAME		= luasoul

INC		= ./

SRCS		= ./lua_ui.c	\
		./utils.c	\
		./lua/ooutils.c	\
		./lua/lua_utils.c	\
		./ui/window.c	\
		./ui/style.c	\
		./ui/input.c	\
		./ui/ui_utils.c	\
		./luasoul.c

OBJ		= $(SRCS:.c=.o)

# _BSD_SOURCE for strdup()
CFLAGS		= -Wall -Wextra -Wshadow -ansi -pedantic -Wwrite-strings \
		-Wstrict-prototypes -Wuninitialized -I$(INC) -I../md5/ \
		-D_BSD_SOURCE -D_POSIX_SOURCE

DEBUG		= -ggdb3

LDFLAGS		= -llua -lform -lpanel -lncurses

CC		?= gcc

RM		= rm -f

VALGRIND	= valgrind

all		: $(NAME)

$(NAME)		: $(OBJ)
		@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)
		@echo -e "  [OUT]\t\t\e[1;34m$(NAME)\e[0m"

%.o		: %.c
		@echo -e "  [CC]\t\t$<"
		@$(CC) $(CFLAGS) -o $@ -c $<

clean   :
		@echo -e "  [RM]\t\t$(OBJ)"
		@$(RM) $(OBJ)

fclean	:	clean
		@echo -e "  [RM]\t\t$(NAME)"
		@$(RM) $(NAME)

re	:	fclean all

debug		: clean
		$(CC) $(DEBUG) $(CFLAGS) -o $(NAME) $(SRCS) $(LDFLAGS)

valgrind	:
		$(VALGRIND) ./$(NAME)

malloc		: debug
		$(VALGRIND) --tool=memcheck --leak-check=yes --show-reachable=yes ./$(NAME)

check-syntax	:
		gcc $(CFLAGS)  -Wall -Wextra -Wshadow -ansi -pedantic -Wwrite-strings \
		-Wstrict-prototypes -Wuninitialized -fsyntax-only $(CHK_SOURCES)
