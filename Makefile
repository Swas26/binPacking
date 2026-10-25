CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall -Isrc

# HiGHS
HIGHS_INC = -I/opt/homebrew/include/highs
HIGHS_LIB = -L/opt/homebrew/lib -lhighs -Wl,-rpath,/opt/homebrew/lib

SRCS = src/common/instance.cpp src/ffd/ffd.cpp src/ilp/ilp.cpp bench/main.cpp

binpack: $(SRCS)
	$(CXX) $(CXXFLAGS) $(HIGHS_INC) $(SRCS) -o binpack $(HIGHS_LIB)

run: binpack
	./binpack

clean:
	rm -f binpack