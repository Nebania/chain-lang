#pragma once

#include <string>
#include <functional>
#include <vector>
#include "../types.h" 

using NativeFn = std::function<Obj(const std::vector<Obj>&)>;

class CREL {
public:
    
    using RegisterCallback = std::function<void(const std::string&, NativeFn)>;
    CREL(RegisterCallback cb) : callback(std::move(cb)) {}

    // Fungsi 'expose' ini nantinya akan dipanggil oleh file eksternal (.so / .dll)
    // untuk mendaftarkan fungsi C++ ke dalam ChainLang.
    void expose(const std::string& name, NativeFn fn) {
        if (callback) {
            callback(name, std::move(fn));
        }
    }

private:
    RegisterCallback callback;
};