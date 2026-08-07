#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <variant>

// Include dependency
#include "types.h"
#include "env.h"
#include "parser.h" 

// Native Function type definition
using NativeFn = std::function<Obj(const std::vector<Obj>&)>;

class Runtime {
private:
    std::shared_ptr<Environment> globalEnv;
    std::shared_ptr<Environment> currentEnv;
    
    // Registry Map
    std::unordered_map<std::string, NativeFn> nativeRegistry;
    std::unordered_map<std::string, FuncDecl*> functionRegistry;

    std::vector<std::unique_ptr<Program>> loadedPrograms;

    // Helper Functions
    void initNativeFunctions();
    std::string objToString(const Obj& o);
    std::string getAnsiColor(const std::string& color);
    void printObj(const Obj& val);
    
    // Logic Helper
    bool isTruthy(const Obj& o);
    FuncDecl* findMethod(LinkClass* klass, const std::string& name);

public:
    Runtime(); // Constructor

    // Main execution function
    void run(const std::string& source, bool debug);
    void runStatement(Stmt* stmt);
    Obj evaluateExpr(Expr* expr);
    void execute(std::unique_ptr<Program> program); 
};
