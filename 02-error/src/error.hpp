#ifndef ERROR_HPP
#define ERROR_HPP

#include <vector>
#include <string>
#include <memory>

namespace error {
    class Error {
    public:
        virtual ~Error();
        void virtual eprint(const std::vector<std::string> &) const = 0;
    };

    template<class Err, class... Args>
    std::unique_ptr<Error> make(Args&&... args){
        return std::make_unique<Err>(std::forward<Args>(args)...);
    }
}

#endif
