/**
 * @file expression.cpp
 */
#include "expression.hpp"

#include <string_view>
#include "error.hpp"

namespace expression {
    Expression::~Expression() = default;
    //! コンストラクタ
    Identifier::Identifier(std::string name): name(std::move(name)) {}
    //! コンストラクタ
    Integer::Integer(std::int32_t value): value(value) {}
    //! コンストラクタ
    UnaryOperation::UnaryOperation(
        UnaryOperator unary_operator,
        std::unique_ptr<Expression> operand
    ):
        unary_operator(unary_operator),
        operand(std::move(operand)) {}
    //! コンストラクタ
    BinaryOperation::BinaryOperation(
        BinaryOperator binary_operator,
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right
    ):
        binary_operator(binary_operator),
        left(std::move(left)),
        right(std::move(right)) {}
    //! コンストラクタ
    Group::Group(std::unique_ptr<Expression> expression):
        expression(std::move(expression)) {}
    //! コンストラクタ
    Invocation::Invocation(
        std::unique_ptr<Expression> function,
        std::vector<std::unique_ptr<Expression>> arguments
    ):
        function(std::move(function)),
        arguments(std::move(arguments)) {}

    /**
     * @brief 単一の識別子からなる式なら，識別子名を返す．
     * @retval std::nullopt 単一の識別子からなる式ではない
     */
    std::optional<std::string> Expression::identifier() { return std::nullopt; }
    std::optional<std::string> Identifier::identifier() { return std::move(name); }

    /**
     * @brief 識別子をコンパイルする．
     *
     * 名前を `local_variables`，`global_variables` の順に検索する．
     *
     * `local_variables` に見つかったら…… `value::Value` に `type` と `pointer` が入っているので，
     * 1. `type` に `context` を渡して `llvm_type` を得る．
     * 2. `builder` に `llvm_type` と `pointer` を渡して `createLoad` を呼び出す．
     *
     * `local_variables` に見つからず，`global_variables` に見つかったら…… `module_number` と `type` が入っているので，
     * 1. `module_number` から `global_variable_name` を得る．
     * 2. `type` に `context` を渡して `llvm_type` を得る．
     * 3. `module`，`llvm_type`，`global_variable_name` を渡して `pointer = new llvm::GlobalVariable` を作る．
     * 4. `builder` に `llvm_type` と `pointer` を渡して `createLoad` を呼び出す．
     * 5. `local_variables` に `type` と `pointer` を保存する．
     *
     * どちらにも見つからなかったら…… `error::UndefinedVariable` を投げる．
     */
    value::Value Identifier::compile(Context &context, std::unordered_map<std::string, value::Value> &local_variables){
        auto local = local_variables.find(name);
        std::shared_ptr<value::Type> return_type;
        llvm::Value *return_value;
        if(local != local_variables.end()){
        }else{
            auto global = context.global_variables.find(name);
            if(global != context.global_variables.end()){
                auto llvm_type = global->second.second->llvm_type(*context.context.getContext());
                auto pointer = new llvm::GlobalVariable(
                    context.get_module(),
                    llvm_type,
                    false,
                    llvm::GlobalValue::ExternalLinkage,
                    nullptr,
                    context.global_variable_name(global->second.first)
                );
                return_type = global->second.second;
                return_value = context.builder.CreateLoad(llvm_type, pointer);
            }else{
                throw error::make<error::UndefinedVariable>(std::move(pos));
            }
        }
        return value::Value(std::move(return_type), return_value);
    }
    /**
     * @brief 整数リテラルをコンパイルする．
     *
     * 方法は 2 通りあって，
     * - `context` から `getInt32Ty` して，`llvm::ConstantInt` を使う
     * - `builder` の `getInt32` を使う
     */
    value::Value Integer::compile(Context &context, std::unordered_map<std::string, value::Value> &){
        return value::make<value::Integer>(context.builder.getInt32(value));
    }
    value::Value UnaryOperation::compile(Context &, std::unordered_map<std::string, value::Value> &){}
    value::Value BinaryOperation::compile(Context &, std::unordered_map<std::string, value::Value> &){}
    value::Value Group::compile(Context &, std::unordered_map<std::string, value::Value> &){}
    value::Value Invocation::compile(Context &, std::unordered_map<std::string, value::Value> &){}

    static constexpr std::string_view INDENT = "    ";
    void Identifier::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Identifier(" << name << ")" << std::endl;
    }
    void Integer::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Integer(" << value << ")" << std::endl;
    }
    void UnaryOperation::debug_print(int depth) const {
        std::string_view name;
        switch(unary_operator){
            case UnaryOperator::Plus: name = "plus"; break;
            case UnaryOperator::Minus: name = "minus"; break;
            case UnaryOperator::LogicalNot: name = "logical not"; break;
            case UnaryOperator::BitNot: name = "bitwise not";
        }
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": UnaryOperation(" << name << ")" << std::endl;
        operand->debug_print(depth + 1);
    }
    void BinaryOperation::debug_print(int depth) const {
        std::string_view name;
        switch(binary_operator){
            case BinaryOperator::Mul: name = "mul"; break;
            case BinaryOperator::Div: name = "div"; break;
            case BinaryOperator::Rem: name = "rem"; break;
            case BinaryOperator::Add: name = "add"; break;
            case BinaryOperator::Sub: name = "sub"; break;
            case BinaryOperator::LeftShift: name = "left shift"; break;
            case BinaryOperator::RightShift: name = "right shift"; break;
            case BinaryOperator::BitAnd: name = "bitwise and"; break;
            case BinaryOperator::BitOr: name = "bitwise or"; break;
            case BinaryOperator::BitXor: name = "bitwise xor"; break;
            case BinaryOperator::Equal: name = "equal"; break;
            case BinaryOperator::NotEqual: name = "not equal"; break;
            case BinaryOperator::Less: name = "less"; break;
            case BinaryOperator::Greater: name = "greater"; break;
            case BinaryOperator::LessEqual: name = "less or equal to"; break;
            case BinaryOperator::GreaterEqual: name = "greater or equal to"; break;
            case BinaryOperator::LogicalAnd: name = "logical and"; break;
            case BinaryOperator::LogicalOr: name = "logical or"; break;
            case BinaryOperator::Assign: name = "assign"; break;
            case BinaryOperator::MulAssign: name = "mul assign"; break;
            case BinaryOperator::DivAssign: name = "div assign"; break;
            case BinaryOperator::RemAssign: name = "rem assign"; break;
            case BinaryOperator::AddAssign: name = "add assign"; break;
            case BinaryOperator::SubAssign: name = "sub assign"; break;
            case BinaryOperator::BitAndAssign: name = "bitwise and assign"; break;
            case BinaryOperator::BitOrAssign: name = "bitwise or assign"; break;
            case BinaryOperator::BitXorAssign: name = "bitwise xor assign"; break;
            case BinaryOperator::RightShiftAssign: name = "right shift assign"; break;
            case BinaryOperator::LeftShiftAssign: name = "left shift assign"; break;
        }
        left->debug_print(depth + 1);
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": BinaryOperation(" << name << ")" << std::endl;
        right->debug_print(depth + 1);
    }
    void Group::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Group" << std::endl;
	expression->debug_print(depth + 1);
    }
    void Invocation::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Invocation" << std::endl;
        function->debug_print(depth + 1);
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << "arguments: " << std::endl;
        for(auto &argument: arguments){
            argument->debug_print(depth + 1);
        }
    }
}
