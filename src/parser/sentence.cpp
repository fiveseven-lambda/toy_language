#include "sentence.hpp"
#include "expression.hpp"
#include "type.hpp"

namespace parser {
    namespace sentence {
        std::unique_ptr<syntax::sentence::Sentence> parse(lexer::Lexer &lexer){
            auto expression = expression::parse(lexer);
            auto next = lexer.next();
            if(!next){
                // ほんとはエラー
                return nullptr;
            }

            std::unique_ptr<syntax::sentence::Sentence> ret;
            pos::Range pos;
            if(next->is_semicolon()){
                if(expression){
                    pos = expression->pos;
                    pos += next->pos;
                }else{
                    pos = next->pos;
                }
                ret = std::make_unique<syntax::sentence::Expression>(std::move(expression));
            }else if(next->is_colon()){
                if(!expression){
                    // ほんとはエラー
                    return nullptr;
                }
                pos = expression->pos;
                std::optional<std::string> identifier = expression->identifier();
                if(!identifier){
                    // ほんとはエラー
                    return nullptr;
                }
                auto type = type::parse(lexer);
                auto next2 = lexer.next();
                if(!next2){
                    // ほんとはエラー
                    return nullptr;
                }
                if(next2->is_semicolon()){
                    pos += next2->pos;
                    ret = std::make_unique<syntax::sentence::Declaration>(std::move(identifier).value(), std::move(type), nullptr);
                }else if(next2->is_equal()){
                    auto rhs = expression::parse(lexer);
                    if(!rhs){
                        // エラー
                        return nullptr;
                    }
                    auto next3 = lexer.next();
                    if(!next3){
                        // エラー
                    }
                    if(!next3->is_semicolon()){
                        // エラー
                    }
                    pos += next3->pos;
                    ret = std::make_unique<syntax::sentence::Declaration>(std::move(identifier).value(), std::move(type), std::move(rhs));
                }else{
                    // ほんとはエラー
                    return nullptr;
                }
            }else{
                // ほんとはエラー
                return nullptr;
            }
            ret->pos = pos;
            return ret;
        }
    }
}
