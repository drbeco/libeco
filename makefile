#***************************************************************************
#*   Autoversion makefile                     v.20170706.015932   (libeco) *
#*   Copyright (C) 2014-2017 by Ruben Carlo Benante <rcb@beco.cc>          *
#*                                                                         *
#*   This makefile sets BUILD and allows to set MAJOR.MINOR version,       *
#*   DEBUG and OBJ to compile a range of different targets                 *
#***************************************************************************
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; version 2 of the License.               *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************
#*   To contact the author, please write to:                               *
#*   Ruben Carlo Benante                                                   *
#*   Email: rcb@beco.cc                                                    *
#*   Webpage: http://drbeco.github.io/                                     *
#***************************************************************************
#
# Usage:
#
#	* Options:
#		- DEBUG=[0|1] turn on/off debug MACRO
#		- Version is MAJOR.MINOR.DATE.TIME
#			+ MAJOR=N makes this major version N
#			+ MINOR=N makes this minor version N
#		- OBJ=someobjetc.o : compile and link with objetc or source
#		- You can force update with flag -B
#
# Examples:
#
# * From linux prompt:
#    - Normal C program (ex1.c)
#        $ make ex1.x
#    - Aspipo program (ex1.c) with object:
#        $ make ex1.x OBJ=libaspipo-ux64.o
#    - Multiple objects:
#        $ make ex1.x OBJ="mylib1.o mylib2.o"
#    - Multiple sources to generate a single binary ex1.x:
#    	 $ make ex1.x SRC="mysrc1.c mysrc2.c"
#	 - Normal C program (ex2.c) version 2.3, no debug
#	 	 $ make ex2.x MAJOR=2 MINOR=3 DEBUG=0
#
# * Directly from vim editor command line:
#    - Normal C program (ex1.c)
#        :make ex1.x
#    - Aspipo program (ex1.c)
#        :make ex1.x OBJ=libaspipo-ux64.o
#
#
# * CTAGS
# 	 - Generate a 'tags' index file with all tags in all C source codes
# 	 	$ make tags
# 	 - Use with vi:
# 	 	:make tags
# 	 	- To find a function or other tag, use $ vim -t tag
# 	 	- Inside vi, use :ta tag
#
#
# * clean
#   Clean temporary files *.o and errors.err
#   	$ make clean
#
# * wipe 
#	- WARNING!! Clean all created files: *.x *.so *.o errors.err and tags
# 		$ make wipe
#
# Log:
# 2017-03-03:
# 		* added -fdiagnostics-color=always to colorize tee output
# 		* added -pg -fprofile-arcs to allow gprof command on debug
# 2017-03-05:
# 		* added variable CCCOLOR to work with vi
# 			- set makeprg=make\ o=%< CCCOLOR=never\ $* 
# 			- set makeprg=make\ %<.x CCCOLOR=never\ $*
# 			- set makeprg=make\ CCCOLOR=never\ $*
#		* added CPPFLAGS += -D$(D) so we can pass values to
#			C-Pre-processor via make prog.x D=VARIABLE=VALUE
# 2017-03-06:
# 		* ifeq "$(DEBUG)" "1" to set CFLAGS
# 		* added _XOPEN_SOURCE=700 to pair with -ansi
# 		* added _FORTIFY_SOURCE=1 to help catch overflows
# 2017-03-18:
# 		* added -lcurl to link with libcurl

# disable builtin rules with MAKEFLAGS and .SUFFIXES
MAKEFLAGS += --no-builtin-rules
#.SUFFIXES:
.PHONY: clean wipe nomatch
.PRECIOUS: %.o
SHELL=/bin/bash -o pipefail

FORTIFY ?= 1
DEBUG ?= 1
MAJOR ?= 0
MINOR ?= 1
OBJ ?=
SRC ?=
CCCOLOR ?= always
D ?= D_
BUILD = $(shell date +"%g%m%d.%H%M%S")
DEFSYM = $(subst .,_,$(BUILD))
VERSION = "\"$(MAJOR).$(MINOR).$(BUILD)\""
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -fdiagnostics-color=$(CCCOLOR)
#CFLAGS = -Wall -Wextra -g -O0 -std=gnu99 -pg -fprofile-arcs -fdiagnostics-color=$(CCCOLOR)
ifeq "$(DEBUG)" "0"
CFLAGS += -Ofast
else ifeq "$(DEBUG)" "1"
CFLAGS += -g -O0
else
CFLAGS += -g -O0 -pg -fprofile-arcs -ansi -Wpedantic
endif
#-pedantic-errors -Werror
#-Ofast -c
CPPFLAGS = -DVERSION=$(VERSION) -DBUILD="\"$(BUILD)\"" -DDEBUG=$(DEBUG) -D$(D) -D_FORTIFY_SOURCE=$(FORTIFY)
ifeq "$(DEBUG)" "2"
CPPFLAGS += -D_XOPEN_SOURCE=700 
endif
LDLIBS = -Wl,--defsym,BUILD_$(DEFSYM)=0 -lm -lpthread -lncurses -lcurl
CCSHARED = -shared -fPIC

# prevent built-in rules for %.o
%.o : %.c

# Shared library
%.so : %.c $(OBJ) $(SRC)
	-$(CC) $(CCSHARED) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o $@ 2>&1 | tee errors.err
ifeq "$(CCCOLOR)" "always"
	@sed -i -r "s/\x1B\[(([0-9]+)(;[0-9]+)*)?[m,K,H,f,J]//g" errors.err
endif

# Programa em C (incluindo bibliotecas como allegro ou libaspipo).
# Inclui VERSION, data de BUILD e DEBUG (opcional).
%.x : %.c $(OBJ) $(SRC)
	-$(CC) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o $@ 2>&1 | tee errors.err
ifeq "$(CCCOLOR)" "always"
	@sed -i -r "s/\x1B\[(([0-9]+)(;[0-9]+)*)?[m,K,H,f,J]//g" errors.err
endif
	-@[ ! -s errors.err ] && echo $@ version $(VERSION) > VERSION

# override built-in rules for mathing everything (exactly the same rule as %.x above)
% : %.c $(OBJ) $(SRC)
	-$(CC) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o $@ 2>&1 | tee errors.err
	@echo $@ version $(VERSION) > VERSION
ifeq "$(CCCOLOR)" "always"
	@sed -i -r "s/\x1B\[(([0-9]+)(;[0-9]+)*)?[m,K,H,f,J]//g" errors.err
endif
	-@[ ! -s errors.err ] && echo $@ version $(VERSION) > VERSION

nomatch :
	@echo 'makefile error: no rules for the given goal(s)' $(warning nomatch)

# CUIDADO! Apaga tudo que o makefile pode criar.
wipe :
	rm -f *.x *.so *.o errors.err tags a.out

# Apaga temporarios desnecessarios.
clean :
	rm -f *.o errors.err

# Gera arquivo de indice tags com funcoes de todos fontes em C
tags :
	ctags libeco.h libeco.c
	ctags -x libeco.h libeco.c | less -F

#* ------------------------------------------------------------------- *
#* makefile config for Vim modeline                                    *
#* vi: set ai noet ts=4 sw=4 tw=0 wm=0 fo=croqlt :                     *
#* Template by Dr. Beco <rcb at beco dot cc> Version 20170506.191339   *

