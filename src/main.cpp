#include "pos.hpp"
#include "error.hpp"

#include <cassert>
#include <sstream>

static void test_pos(){
    pos::Pos pos(20, 30);
    auto [line, byte] = pos.into_inner();
    assert(line == 20);
    assert(byte == 30);
    std::stringstream ss;
    ss << pos;
    assert(ss.str() == "21:31");
}

static void test_range(){
    pos::Range range(2, 3, 5);
    std::stringstream ss;
    ss << range;
    assert(ss.str() == "3:4-3:5");
}

int main(){
    test_pos();
    test_range();
}
