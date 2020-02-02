#! /usr/bin/make -f

################################################################################
# Copyright (C) 2019	Alejandro Colomar Andrés
# SPDX-License-Identifier:	BSD-2-Clause
################################################################################
# *AUTHOR*
# FULL NAME	"Alejandro Colomar Andrés"
# EMAIL		"1903716@gmail.com"
################################################################################

VERSION		= 0
PATCHLEVEL	= 0
SUBLEVEL	= 0
EXTRAVERSION	=
NAME		= da0101

export	VERSION
export	PATCHLEVEL
export	SUBLEVEL

################################################################################
# Beautify output
# ---------------------------------------------------------------------------
# Prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)some command here
#
# If BUILD_VERBOSE equals 0 then the above command will be hidden.
# If BUILD_VERBOSE equals 2 then the above command is displayed.
#
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see some verbose output
# Use 'make V=2' to see the full commands

ifeq ("$(origin V)","command line")
  BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 0
endif

ifeq ($(BUILD_VERBOSE), 2)
  Q =
  v = -v
else ifeq ($(BUILD_VERBOSE), 1)
  Q = @
  v = -v
else
  Q = @
  v =
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
  Q = @
  v =
endif

export	Q
export	BUILD_VERBOSE

################################################################################
PROGRAMVERSION	= $(VERSION)$(if $(PATCHLEVEL),.$(PATCHLEVEL)$(if $(SUBLEVEL),.$(SUBLEVEL)))$(EXTRAVERSION)
export	PROGRAMVERSION

################################################################################
# Make variables (CC, etc...)
  CC	= gcc
  AS	= as
  LD	= ld
  SZ	= size --format=SysV

export	CC
export	AS
export	LD
export	SZ

################################################################################
# cflags
CFLAGS_STD	= -std=gnu17

CFLAGS_OPT	= -O3
CFLAGS_OPT     += -march=native
CFLAGS_OPT     += -flto

CFLAGS_W	= -Wall
CFLAGS_W       += -Wextra
CFLAGS_W       += -Wstrict-prototypes
CFLAGS_W       += -Werror

CFLAGS_PKG	= `pkg-config --cflags libalx-base`

CFLAGS		= $(CFLAGS_STD)
CFLAGS         += $(CFLAGS_OPT)
CFLAGS         += $(CFLAGS_W)
CFLAGS         += $(CFLAGS_PKG)

export	CFLAGS

################################################################################
# libs
LDFLAGS_OPT	= -O3
LDFLAGS_OPT    += -march=native
LDFLAGS_OPT    += -flto
LDFLAGS_OPT    += -fuse-linker-plugin

LDFLAGS_PKG	= `pkg-config --libs libalx-base`

LDFLAGS		= $(LDFLAGS_OPT)
LDFLAGS        += $(LDFLAGS_PKG)

export	LDFLAGS

################################################################################
# executable

BIN_NAME	= da0101

export	BIN_NAME

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(BIN_NAME) size


$(BIN_NAME).s: $(BIN_NAME).c
	@echo	"	CC	$@"
	$(Q)$(CC) $(CFLAGS) -S $< -o $@

$(BIN_NAME).o: $(BIN_NAME).s
	@echo	"	AS	$@"
	$(Q)$(AS) $< -o $@

$(BIN_NAME): $(BIN_NAME).o
	@echo	"	CC	$@"
	$(Q)$(CC) $^ -o $@ $(LDFLAGS)

PHONY += size
size: $(BIN_NAME)
	@echo	"	SZ	$(BIN_NAME)"
	$(Q)$(SZ) $(BIN_NAME)


PHONY += clean
clean:
	$(Q)rm -f *.o *.s $(BIN_NAME)

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)




################################################################################
######## End of file ###########################################################
################################################################################
