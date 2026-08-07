#pragma once
#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "os.h" 

struct Value;
struct Stmt;        
struct LinkClass;   
struct LinkInstance;

struct FuncDecl;           
struct Environment;        

struct LinkFunction {
    FuncDecl* declaration;
    std::shared_ptr<Environment> closure;
};

using List = std::vector<Value>;
using Dict = std::unordered_map<std::string, Value>;
struct Value {
    using ValVariant = std::variant<
        std::monostate, int, double, std::string, char, bool, 
        std::shared_ptr<List>, std::shared_ptr<Dict>,
        std::shared_ptr<LinkClass>, std::shared_ptr<LinkInstance>,
        std::shared_ptr<LinkFunction> // <--- New data type (Function)
    >;
    
    ValVariant as;

    Value() : as(std::monostate{}) {}
    Value(int v) : as(v) {}
    Value(double v) : as(v) {}
    Value(std::string v) : as(v) {}
    Value(const char* v) : as(std::string(v)) {} 
    Value(char v) : as(v) {}
    Value(bool v) : as(v) {}
    Value(std::shared_ptr<List> v) : as(v) {}
    Value(std::shared_ptr<Dict> v) : as(v) {}
    Value(std::shared_ptr<LinkClass> v) : as(v) {}
    Value(std::shared_ptr<LinkInstance> v) : as(v) {}
    Value(std::shared_ptr<LinkFunction> v) : as(v) {} // <--- Constructor baru
};

using Obj = Value;
struct LinkClass {
    std::string name;
    std::unordered_map<std::string, Stmt*> methods; 
};
struct LinkInstance {
    std::shared_ptr<LinkClass> klass;        
    std::unordered_map<std::string, Value> fields;  
};
inline void printObj(const Obj& val) {
    if (std::holds_alternative<int>(val.as)) std::cout << std::get<int>(val.as);
    else if (std::holds_alternative<double>(val.as)) std::cout << std::get<double>(val.as);
    else if (std::holds_alternative<std::string>(val.as)) std::cout << Sys::unescape(std::get<std::string>(val.as));
    else if (std::holds_alternative<char>(val.as)) std::cout << std::get<char>(val.as);
    else if (std::holds_alternative<bool>(val.as)) std::cout << (std::get<bool>(val.as) ? "true" : "false");
    else if (std::holds_alternative<std::shared_ptr<List>>(val.as)) {
        auto list = std::get<std::shared_ptr<List>>(val.as);
        std::cout << "[";
        for (size_t i = 0; i < list->size(); ++i) {
            printObj((*list)[i]);
            if (i < list->size() - 1) std::cout << ", ";
        }
        std::cout << "]";
    }
    else if (std::holds_alternative<std::shared_ptr<Dict>>(val.as)) {
        auto dict = std::get<std::shared_ptr<Dict>>(val.as);
        std::cout << "{";
        int i = 0;
        for (const auto& pair : *dict) {
            std::cout << "\"" << pair.first << "\": ";
            printObj(pair.second);
            if (i < (int)dict->size() - 1) std::cout << ", ";
            i++;
        }
        std::cout << "}";
    }
    else if (std::holds_alternative<std::shared_ptr<LinkClass>>(val.as)) {
        std::cout << "<Class " << std::get<std::shared_ptr<LinkClass>>(val.as)->name << ">";
    }
    else if (std::holds_alternative<std::shared_ptr<LinkInstance>>(val.as)) {
        auto instance = std::get<std::shared_ptr<LinkInstance>>(val.as);
        std::cout << "<Instance " << instance->klass->name << ">";
    }
    else if (std::holds_alternative<std::shared_ptr<LinkFunction>>(val.as)) {
        std::cout << "<Function>"; // Simply print this to avoid circular dependency
    }
    else std::cout << "nil";
}

struct ReturnException {
	Obj value; 
	ReturnException(Obj v) : value(v) {}
};

struct BreakException {}; 

struct ContinueException {}; 
struct RuntimeException {
    std::string message;
    RuntimeException(std::string msg) : message(msg) {}
};
