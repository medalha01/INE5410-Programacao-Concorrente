# Includes the project configurations
include project.conf

# Validating project variables defined in project.conf
ifndef BINARY
$(error Missing BINARY. Put variables at project.conf file)
endif

# Gets the Operating system name
OS := $(shell uname -s)

# Default shell
SHELL := bash

# Color prefix for Linux distributions
COLOR_PREFIX := e

ifeq ($(OS),Darwin)
	COLOR_PREFIX := 033
endif

# Color definition for print purpose
BROWN=\$(COLOR_PREFIX)[0;33m
BROWN=\$(COLOR_PREFIX)[1;34m
END_COLOR=\$(COLOR_PREFIX)[0m

# Source code directory structure
BINDIR := bin
SRCDIR := src
LOGDIR := log
LIBDIR := lib

# Source code file extension
SRCEXT := c

# Defines the C Compiler
CC := gcc

# Defines the language standards for GCC
STD := -std=gnu99 # See man gcc for more options

# Protection for stack-smashing attack
STACK := -fstack-protector-all -Wstack-protector

# Specifies to GCC the required warnings
WARNS := -Wall -Wextra -pedantic # -pedantic warns on language standards

# Flags for compiling
CFLAGS := -pthread -D_POSIX_C_SOURCE=200809L -O3 $(STD) $(STACK) $(WARNS)

# Disable sanitizer flags if we are using Cygwin or Vgbuild
ifneq ($(OS),Windows_NT)
	ifneq ($(DISABLE_SANS),1)
		CFLAGS +=  -fsanitize=address -fsanitize=undefined
	endif
endif

# Debug options
DEBUG := -g3 -DDEBUG=1

# Dependency libraries
LIBS := # -lm  -I some/path/to/library

# %.o file names
NAMES := $(notdir $(basename $(wildcard $(SRCDIR)/*.$(SRCEXT))))
OBJECTS :=$(patsubst %,$(LIBDIR)/%.o,$(NAMES))


# COMPILATION RULES
default: help

# Help message
help:
	@echo "C Project Template"
	@echo "Target rules:"
	@echo "    build    - Compiles and generates binary file"
	@echo "    clean    - Clean the project by removing binaries"
	@echo "    help     - Prints this help message with target rules"

# Rule for link and generate the binary file
build: $(OBJECTS)
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(BINDIR)/$(BINARY) $+ $(DEBUG) $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at $(BROWN)$(BINDIR)/$(BINARY)$(END_COLOR)\n";

# Rule for object binaries compilation
$(LIBDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC) -c $^ -o $@ $(DEBUG) $(CFLAGS) $(LIBS)

# Rule for cleaning the project
clean:
	@rm -rvf $(BINDIR)/* $(LIBDIR)/* $(LOGDIR)/*;

run-docker:  ## Build and run docker sandbox
	docker build . -t gcc-sandbox
	docker run -it gcc-sandbox
