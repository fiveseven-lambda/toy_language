CXX = clang++
CXXFLAGS = -std=c++20 -Weverything -Wno-c++98-compat -Wno-shadow-field-in-constructor \
	-D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LDFLAGS = -lLLVM-13
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
TARGET = target/interpreter

$(TARGET): $(OBJ)
	if [ ! -d target ]; then mkdir target; fi
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ)

obj/%.o: src/%.cpp
	if [ ! -d obj ]; then mkdir obj; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	if [ -d obj ]; then rm -r obj; fi
	if [ -d target ]; then rm -r target; fi
