##
## Makefile for luasoul
## 
## Copyright © 2010 Guillaume Papin <guillaume.papin@epitech.eu>
## 
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

NAME		= libnetsoul.a

INC		= -I./

SRCS		= ./recv.c	\
		./utils.c	\
		./close.c	\
		./set_status.c	\
		./netsoul_connect.c	\
		./md5.c	\
		./get_msg_type.c	\
		./send.c	\

OBJ		= $(SRCS:%.c=%.o)

CFLAGS		= -Wall -Wextra -Wshadow -ansi -pedantic -Wwrite-strings \
		-Wstrict-prototypes -Wuninitialized $(INC) $(DEBUG) \
		-D_XOPEN_SOURCE
# _XOPEN_SOURCE for getaddrinfo()

DEBUG		= -ggdb3

LDFLAGS		= -lopenssl # (for md5)

CC		?= gcc

AR		?= ar

RM		= rm -f

all		: $(NAME)

$(NAME)		: $(OBJ)
		@$(AR) rcs $(NAME) $(OBJ)
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

check-syntax	:
		gcc $(CFLAGS) -Wall -Wextra -Wshadow -ansi -pedantic -Wwrite-strings \
		-Wstrict-prototypes -Wuninitialized -fsyntax-only $(CHK_SOURCES)
