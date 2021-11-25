#ifndef ERROR_HPP
#define ERROR_HPP

namespace error {
    class Error {
    public:
        void virtual eprint() const = 0;
        virtual ~Error();
    };
}

#endif
