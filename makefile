CXX := g++
LXX = g++

CXXFLAGS := -W -Wall -Os -std=c++17 -c
LXXFLAGS := -s -Os

BUILDDIR := build

SRCS := $(notdir $(shell find -name '*.cpp'))
OBJS := $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SRCS))

tiffy: builddir $(OBJS) $(SRCS) 
	$(LXX) $(LXXFLAGS) $(OBJS) -o $(BUILDDIR)/tiffy

$(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: install
install:
	@cp build/tiffy ~/.local/bin/tiffy

.PHONY: builddir
builddir:
	@mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	@rm -f -r build/*.o
	@rm -rf build