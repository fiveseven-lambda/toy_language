CXX = clang++
CXXFLAGS = -std=c++20 -Weverything -Wno-c++98-compat -Wno-shadow-field-in-constructor -Wno-padded \
	-D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LDFLAGS = -lLLVM-13
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
TARGET = target/interpreter

target/interpreter: obj/syntax/expression.o obj/syntax/sentence.o obj/syntax/type.o obj/parser/expression.o obj/parser/sentence.o obj/parser/type.o obj/error.o obj/lexer.o obj/pos.o obj/token.o obj/main.o
	if [ ! -d target ]; then mkdir target; fi
	$(CXX) $(LDFLAGS) -o $@ obj/syntax/expression.o obj/syntax/sentence.o obj/syntax/type.o obj/parser/expression.o obj/parser/sentence.o obj/parser/type.o obj/error.o obj/lexer.o obj/pos.o obj/token.o obj/main.o

obj/syntax/expression.o: src/syntax/expression.cpp
	if [ ! -d obj/syntax ]; then mkdir -p obj/syntax; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/syntax/sentence.o: src/syntax/sentence.cpp
	if [ ! -d obj/syntax ]; then mkdir -p obj/syntax; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/syntax/type.o: src/syntax/type.cpp
	if [ ! -d obj/syntax ]; then mkdir -p obj/syntax; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/parser/expression.o: src/parser/expression.cpp
	if [ ! -d obj/parser ]; then mkdir -p obj/parser; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/parser/sentence.o: src/parser/sentence.cpp
	if [ ! -d obj/parser ]; then mkdir -p obj/parser; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/parser/type.o: src/parser/type.cpp
	if [ ! -d obj/parser ]; then mkdir -p obj/parser; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/error.o: src/error.cpp
	if [ ! -d obj ]; then mkdir -p obj; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/lexer.o: src/lexer.cpp
	if [ ! -d obj ]; then mkdir -p obj; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/pos.o: src/pos.cpp
	if [ ! -d obj ]; then mkdir -p obj; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/token.o: src/token.cpp
	if [ ! -d obj ]; then mkdir -p obj; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<
obj/main.o: src/main.cpp
	if [ ! -d obj ]; then mkdir -p obj; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	if [ -d obj ]; then rm -r obj; fi
	if [ -d target ]; then rm -r target; fi
