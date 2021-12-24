#include "token.hpp"

namespace token {
    Token::~Token() = default;
    Identifier::Identifier(std::string name): name(std::move(name)) {}
    Integer::Integer(std::string value): value(std::move(value)) {}
}

#define define_debug_print(token) \
void token::debug_print() const { \
    std::cout << #token << std::endl; \
}

namespace token {
    void Identifier::debug_print() const {
        std::cout << "Identifier(" << name << ")" << std::endl;
    }
    void Integer::debug_print() const {
        std::cout << "Integer(" << value << ")" << std::endl;
    }
    define_debug_print(Plus)
    define_debug_print(PlusEqual)
    define_debug_print(Hyphen)
    define_debug_print(HyphenEqual)
    define_debug_print(Asterisk)
    define_debug_print(AsteriskEqual)
    define_debug_print(Slash)
    define_debug_print(SlashEqual)
    define_debug_print(Percent)
    define_debug_print(PercentEqual)
    define_debug_print(Ampersand)
    define_debug_print(AmpersandEqual)
    define_debug_print(DoubleAmpersand)
    define_debug_print(Bar)
    define_debug_print(BarEqual)
    define_debug_print(DoubleBar)
    define_debug_print(Circumflex)
    define_debug_print(CircumflexEqual)
    define_debug_print(Tilde)
    define_debug_print(Equal)
    define_debug_print(DoubleEqual)
    define_debug_print(Exclamation)
    define_debug_print(ExclamationEqual)
    define_debug_print(Less)
    define_debug_print(LessEqual)
    define_debug_print(DoubleLess)
    define_debug_print(DoubleLessEqual)
    define_debug_print(Greater)
    define_debug_print(GreaterEqual)
    define_debug_print(DoubleGreater)
    define_debug_print(DoubleGreaterEqual)
    define_debug_print(OpeningParenthesis)
    define_debug_print(ClosingParenthesis)
    define_debug_print(OpeningBrace)
    define_debug_print(ClosingBrace)
    define_debug_print(OpeningBracket)
    define_debug_print(ClosingBracket)
    define_debug_print(Dot)
    define_debug_print(Colon)
    define_debug_print(Semicolon)
    define_debug_print(Comma)
}
