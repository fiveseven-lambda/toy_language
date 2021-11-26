#include <cctype>

#include "lexer.hpp"
#include "error.hpp"

namespace lexer {
    Lexer::Lexer():
        source(std::cin),
        prompt(true) {}
    Lexer::Lexer(std::ifstream &source):
        source(source),
        prompt(false) {}

    const std::vector<std::string> &Lexer::get_log() const {
        return log;
    }

    token::TokenWithPos &Lexer::peek() {
        while(tokens.empty()){
            if(source){
                auto line_num = log.size();
                log.emplace_back();
                if(prompt) std::cout << "> ";
                std::getline(source, log.back());
                inner.run(line_num, log.back(), tokens);
            }else{
                tokens.emplace();
            }
        }
        return tokens.front();
    }
    token::TokenWithPos Lexer::next() {
        auto ret = std::move(peek());
        tokens.pop();
        return ret;
    }

    void Inner::run(std::size_t line_num, const std::string &str, std::queue<token::TokenWithPos> &queue){
        std::size_t cursor = 0;
        auto advance_if = [&](char c){
            bool ret = str[cursor] == c;
            if(ret) ++cursor;
            return ret;
        };
        while(true){
            while(true){
                if(cursor == str.size()) return;
                if(!std::isspace(str[cursor])) break;
                ++cursor;
            }
            std::size_t start = cursor;
            std::unique_ptr<token::Token> token;
            if(std::isdigit(str[start])){
                while(std::isdigit(str[cursor])) ++cursor;
                token = std::make_unique<token::Integer>(str.substr(start, cursor - start));
            }else if(std::isalpha(str[start]) || str[start] == '_'){
                while(std::isalnum(str[cursor]) || str[cursor] == '_') ++cursor;
                token = std::make_unique<token::Identifier>(str.substr(start, cursor - start));
            }else if(advance_if('+')){
                if(advance_if('=')) token = std::make_unique<token::PlusEqual>();
                else token = std::make_unique<token::Plus>();
            }else if(advance_if('-')){
                if(advance_if('=')) token = std::make_unique<token::HyphenEqual>();
                else token = std::make_unique<token::Hyphen>();
            }else if(advance_if('*')){
                if(advance_if('=')) token = std::make_unique<token::AsteriskEqual>();
                else token = std::make_unique<token::Asterisk>();
            }else if(advance_if('/')){
                if(advance_if('=')) token = std::make_unique<token::SlashEqual>();
                else token = std::make_unique<token::Slash>();
            }else if(advance_if('%')){
                if(advance_if('=')) token = std::make_unique<token::PercentEqual>();
                else token = std::make_unique<token::Percent>();
            }else if(advance_if('&')){
                if(advance_if('=')) token = std::make_unique<token::AmpersandEqual>();
                else if(advance_if('&')) token = std::make_unique<token::DoubleAmpersand>();
                else token = std::make_unique<token::Ampersand>();
            }else if(advance_if('|')){
                if(advance_if('=')) token = std::make_unique<token::BarEqual>();
                else if(advance_if('|')) token = std::make_unique<token::DoubleBar>();
                else token = std::make_unique<token::Bar>();
            }else if(advance_if('^')){
                if(advance_if('=')) token = std::make_unique<token::CircumflexEqual>();
                else token = std::make_unique<token::Circumflex>();
            }else if(advance_if('~')){
                token = std::make_unique<token::Tilde>();
            }else if(advance_if('=')){
                if(advance_if('=')) token = std::make_unique<token::DoubleEqual>();
                else token = std::make_unique<token::Equal>();
            }else if(advance_if('!')){
                if(advance_if('=')) token = std::make_unique<token::ExclamationEqual>();
                else token = std::make_unique<token::Exclamation>();
            }else if(advance_if('<')){
                if(advance_if('<'))
                    if(advance_if('=')) token = std::make_unique<token::DoubleLessEqual>();
                    else token = std::make_unique<token::DoubleLess>();
                else if(advance_if('=')) token = std::make_unique<token::LessEqual>();
                else token = std::make_unique<token::Less>();
            }else if(advance_if('>')){
                if(advance_if('>'))
                    if(advance_if('=')) token = std::make_unique<token::DoubleGreaterEqual>();
                    else token = std::make_unique<token::DoubleGreater>();
                else if(advance_if('=')) token = std::make_unique<token::GreaterEqual>();
                else token = std::make_unique<token::Greater>();
            }else if(advance_if('(')){
                token = std::make_unique<token::OpeningParenthesis>();
            }else if(advance_if(')')){
                token = std::make_unique<token::ClosingParenthesis>();
            }else if(advance_if('{')){
                token = std::make_unique<token::OpeningBrace>();
            }else if(advance_if('}')){
                token = std::make_unique<token::ClosingBrace>();
            }else if(advance_if('[')){
                token = std::make_unique<token::OpeningBracket>();
            }else if(advance_if(']')){
                token = std::make_unique<token::ClosingBracket>();
            }else if(advance_if('.')){
                token = std::make_unique<token::Dot>();
            }else if(advance_if(':')){
                token = std::make_unique<token::Colon>();
            }else if(advance_if(';')){
                token = std::make_unique<token::Semicolon>();
            }else if(advance_if(',')){
                token = std::make_unique<token::Comma>();
            }else{
                throw error::make<error::UnexpectedCharacter>(pos::Pos(line_num, cursor));
            }
            queue.emplace(pos::Range(line_num, start, cursor), std::move(token));
        }
    }
}
