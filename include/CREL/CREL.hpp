#pragma once

#include <string>
#include <functional>
#include <vector>


using NativeFn = std::function<Obj(const std::vector<Obj>&)>;

class CREL {
public:
    using RegisterFunctionFn = std::function<void(const std::string&, NativeFn)>;

private:
    RegisterFunctionFn registerFunctionFn;

public:
    explicit CREL(RegisterFunctionFn fn): registerFunctionFn(std::move(fn)){
    }

    void registerFunction(const std::string& name,NativeFn fn) {
        registerFunctionFn(name, std::move(fn));
    }
};