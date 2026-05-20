CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -O2

PROGRAMS = wcat wgrep wzip wunzip

.PHONY: all clean

all: $(PROGRAMS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)
