##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
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

# PREFIX	?= arm-none-eabi
PREFIX		?= arm-elf
DESTDIR		?= /usr/local
INCDIR		= $(DESTDIR)/$(PREFIX)/include
LIBDIR		= $(DESTDIR)/$(PREFIX)/lib
INSTALL		= install

TARGETS = stm32 lpc13xx lm3s

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

all: build

build: lib examples

lib:
	$(Q)for i in $(addprefix $@/,$(TARGETS)); do \
		if [ -d $$i ]; then \
			printf "  BUILD   $$i\n"; \
			$(MAKE) -C $$i; \
		fi; \
	done

examples:
	$(Q)for i in $(addsuffix /*/*,$(addprefix $@/,$(TARGETS))); do \
		if [ -d $$i ]; then \
			printf "  BUILD   $$i\n"; \
			$(MAKE) -C $$i; \
		fi; \
	done

install: lib 
	@printf "  INSTALL headers\n"
	$(Q)$(INSTALL) -d $(INCDIR)/libopencm3
	$(Q)$(INSTALL) -d $(LIBDIR)
	$(Q)cp -r include/libopencm3/* $(INCDIR)/libopencm3
	@printf "  INSTALL libs\n"
	$(Q)$(INSTALL) -m 0644 lib/*/*.a $(LIBDIR)
	@printf "  INSTALL ldscripts\n"
	$(Q)$(INSTALL) -m 0644 lib/*/*.ld $(LIBDIR)

clean:
	$(Q)for i in $(addprefix lib/,$(TARGETS)) \
		     $(addsuffix /*/*,$(addprefix examples/,$(TARGETS))); do \
		if [ -d $$i ]; then \
			printf "  CLEAN   $$i\n"; \
			$(MAKE) -C $$i clean; \
		fi; \
	done

.PHONY: build lib examples install clean

