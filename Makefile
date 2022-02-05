# flipflip's glmatrix experiment
#
# Copyright (c) 2022 Philippe Kehl (flipflip at oinkzwurgl dot org),
# https://oinkzwurgl.org/hacking/ubloxcfg

CXX      := gcc
CXXFLAGS := -std=gnu++17 -Og -ggdb
CXXFLAGS += -Wall -Wextra -Werror -Wshadow -Wformat -Wpointer-arith -Wundef  -Isrc
CXXFLAGS += $(shell pkg-config --cflags glfw3 glm)
LDFLAGS  := $(shell pkg-config --libs   glfw3 glm) -lstdc++ -lm -lGL
SRC      := $(sort $(wildcard *.cpp) $(wildcard *.hpp) $(wildcard */*.cpp) $(wildcard */*.hpp) $(wildcard */*.h) )
CXXFLAGS += $(addprefix -I, $(sort $(dir $(SRC))))

glmatrix: $(SRC) Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(filter %.cpp, $^) $(LDFLAGS)

.PHONY: clean
clean:
	rm -vf glmatrix core.*
