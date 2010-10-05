##
## Makefile for luasoul in /home/papin_g
## 
## Made by Guillaume Papin
## Login   <papin_g@epitech.net>
## 
## Started on  Wed Oct  6 00:43:13 2010 Guillaume Papin
## Last update Wed Oct  6 00:46:18 2010 Guillaume Papin
##

NAME		= luasoul

INC		= ./

SRCS		= ./luasoul.c

OBJ		= $(SRCS:.c=.o)

CFLAGS		= -Wall -Wextra -Wshadow -ansi -pedantic -Wwrite-strings \
		-Wstrict-prototypes -Wuninitialized -I$(INC) -I../md5/

DEBUG		= -ggdb3

DEBUG		= -ggdb3

LDFLAGS		= -llua -lmenu -lform -lncurses -lssl

CC		?= gcc

RM		= rm -f

VALGRIND	= valgrind

all		: $(NAME)

$(NAME)		: $(OBJ)
		@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)
		@echo -e "  [OUT]\t\t\e[1;34m$(NAME)\e[0m"

.c.o	:
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
