CXX = clang++
CXXFLAGS = -std=c++20 -Weverything \
	-Wno-c++98-compat \
	-Wno-shadow-field-in-constructor \
	-Wno-padded \
	-Wno-unused-template \
	-D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LDFLAGS = -lLLVM-13
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
TARGET = bin/interpreter

$(TARGET): $(OBJ)
	if [ ! -d bin ]; then mkdir bin; fi
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ)

obj/%.o: src/%.cpp
	if [ ! -d obj ]; then mkdir obj; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	if [ -d obj ]; then rm -r obj; fi
	if [ -d bin ]; then rm -r bin; fi
