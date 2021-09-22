SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
OUT = interpreter

$(OUT): $(OBJ)
	clang++ -lLLVM-12 $(OBJ) -o $(OUT)

clean:
	rm $(OUT) *.o

%.o: %.cpp
	clang++ -c -std=c++20 $< -o $@ \
		-Weverything -Wno-padded -Wno-c++98-compat -Wno-shadow-field-in-constructor \
		-D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS