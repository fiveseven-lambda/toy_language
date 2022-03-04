/**
 * @file type.hpp
 * @brief 型を定義する
 */
#ifndef TYPE_HPP
#define TYPE_HPP

#include "pos.hpp"
#include "value.hpp"

/**
 * @brief ソースコード中に書かれた型名．
 */
namespace type {
    /**
     * @brief 全ての型の基底クラス
     */
    class Type {
    public:
        //! ソースコード中の位置．
        pos::Range pos;
        virtual ~Type();
        virtual std::unique_ptr<value::Type> into() const = 0;
        //! デバッグ出力用の関数．いずれ消す．
        virtual void debug_print(int = 0) const = 0;
    };

    /**
     * @brief `value::Integer`
     */
    class Integer : public Type {
        std::unique_ptr<value::Type> into() const override;
        void debug_print(int) const override;
    };

    /**
     * @brief `value::Boolean`
     */
    class Boolean : public Type {
        std::unique_ptr<value::Type> into() const override;
        void debug_print(int) const override;
    };
}

#endif
