#pragma once
#include "types.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex> 

struct Environment {
    std::shared_ptr<Environment> enclosing;
    std::unordered_map<std::string, Obj> values;
    std::mutex envMutex; //MUTEX

    Environment(std::shared_ptr<Environment> enc = nullptr) : enclosing(enc) {}

    void define(const std::string& name, Obj val) {
        std::lock_guard<std::mutex> lock(envMutex); 
        values[name] = val;
    }

    Obj get(const std::string& name) {
        {
            std::lock_guard<std::mutex> lock(envMutex); 
            if (values.count(name)) return values[name];
        }
        if (enclosing) return enclosing->get(name);
        return Obj();  
    }

    void assign(const std::string& name, Obj val) {
        {
            std::lock_guard<std::mutex> lock(envMutex); 
            if (values.count(name)) {
                values[name] = val;
                return;
            }
        }
        
        if (enclosing) {
            enclosing->assign(name, val);
            return;
        }
        std::lock_guard<std::mutex> lock(envMutex);
        values[name] = val;  
    }
};