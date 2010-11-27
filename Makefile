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
