/**
 * @file type.hpp
 * @brief 型を定義する
 */
#ifndef TYPE_HPP
#define TYPE_HPP

#include "pos.hpp"

/**
 * @brief 型を定義する．
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
        //! デバッグ出力用の関数．いずれ消す．
        virtual void debug_print(int = 0) const = 0;
    };

    /**
     * @brief 32 ビット整数型
     */
    class Integer : public Type {
        void debug_print(int) const override;
    };

    /**
     * @brief 論理型
     */
    class Boolean : public Type {
        void debug_print(int) const override;
    };
}

#endif
