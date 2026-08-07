#pragma once
#include "types.h"
#include <unordered_map>
#include <string>
#include <memory>

struct Environment {
    std::shared_ptr<Environment> enclosing; // Berubah jadi shared_ptr
    std::unordered_map<std::string, Obj> values;

    Environment(std::shared_ptr<Environment> enc = nullptr) : enclosing(enc) {}

    void define(const std::string& name, Obj val) {
        values[name] = val;
    }

    Obj get(const std::string& name) {
        if (values.count(name)) return values[name];
        if (enclosing) return enclosing->get(name);
        return Obj();  
    }

    void assign(const std::string& name, Obj val) {
        if (values.count(name)) {
            values[name] = val;
            return;
        }
        if (enclosing) {
            enclosing->assign(name, val);
            return;
        }
        values[name] = val;  
    }
};