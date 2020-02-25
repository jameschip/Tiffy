# Makefile for avalanche assembler
# By James Henderson, 2019
# Devantech Ltd
# www.robot-electronics.co.uk


CXX := g++
LXX = g++

CXXFLAGS := -Os -std=c++17 -c
LXXFLAGS := -s -Os


BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj

SRCS := $(notdir $(shell find -name '*.cpp'))
OBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

tiffy: builddir $(OBJS) $(SRCS) 
	$(LXX) $(LXXFLAGS) $(OBJS) -o $(BUILDDIR)/tiffy

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: builddir
builddir:
	@mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@rm -f -r build/obj/*.o
	@rm -f build/avalanche