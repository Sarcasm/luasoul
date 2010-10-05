##
## Makefile for luasoul in /home/papin_g
## 
## Made by Guillaume Papin
## Login   <papin_g@epitech.net>
## 
## Started on  Wed Oct  6 00:02:14 2010 Guillaume Papin
## Last update Wed Oct  6 00:42:44 2010 Guillaume Papin
##

CURRENT_DIR	= ./

SRC_DIR		= ./src/

.PHONY		: all clean fclean debug re

all		:
		$(MAKE) -C $(SRC_DIR)

clean		:
		$(MAKE) -C $(SRC_DIR) clean

fclean		:
		$(MAKE) -C $(SRC_DIR) fclean

debug		:
		$(MAKE) -C $(SRC_DIR) debug

re		:
		$(MAKE) -C $(SRC_DIR) re
