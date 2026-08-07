#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct Expr {
    virtual ~Expr() = default;
    virtual void print() const = 0;
};

struct NumberExpr : public Expr {
    int value;
    NumberExpr(int v) : value(v) {}
    void print() const override { std::cout << value; }
};

struct FloatExpr : public Expr {
    double value;
    FloatExpr(double v) : value(v) {}
    void print() const override { std::cout << value; }
};

struct StringExpr : public Expr {
    std::string value;
    StringExpr(std::string v) : value(v) {}
    void print() const override { std::cout << "\"" << value << "\""; }
};

struct CharExpr : public Expr {
    char value;
    CharExpr(char v) : value(v) {}
    void print() const override { std::cout << "'" << value << "'"; }
};

struct BoolExpr : public Expr {
    bool value;
    BoolExpr(bool v) : value(v) {}
    void print() const override { std::cout << (value ? "true" : "false"); }
};

struct VariableExpr : public Expr {
    std::string name;
    VariableExpr(std::string n) : name(n) {}
    void print() const override { std::cout << name; }
};

struct CallExpr : public Expr {
    std::string func;
    std::vector<std::unique_ptr<Expr>> args;
    CallExpr(const std::string& f, std::vector<std::unique_ptr<Expr>> a) : func(f), args(std::move(a)) {}
    void print() const override { std::cout << func << "(...)"; }
};

struct MethodCallExpr : public Expr {
    std::unique_ptr<Expr> object; 
    std::string method;           
    std::vector<std::unique_ptr<Expr>> args; 
    
    MethodCallExpr(std::unique_ptr<Expr> o, std::string m, std::vector<std::unique_ptr<Expr>> a)
    : object(std::move(o)), method(m), args(std::move(a)) {}
    
    void print() const override {
        object->print(); std::cout << "." << method << "(...)";
    }
};

struct ThisExpr : public Expr {
    Token keyword;
    ThisExpr(Token k) : keyword(k) {}
    void print() const override { std::cout << "this"; }
}; 
struct GetExpr : public Expr {
    std::unique_ptr<Expr> object;
    std::string name;
    
    GetExpr(std::unique_ptr<Expr> obj, std::string n) 
    : object(std::move(obj)), name(n) {}
    
    void print() const override { 
        object->print(); std::cout << "." << name; 
    }
}; 
struct SetExpr : public Expr {
    std::unique_ptr<Expr> object;
    std::string name;
    std::unique_ptr<Expr> value;
    
    SetExpr(std::unique_ptr<Expr> obj, std::string n, std::unique_ptr<Expr> v)
    : object(std::move(obj)), name(n), value(std::move(v)) {}
    
    void print() const override {
        object->print(); std::cout << "." << name << " = "; value->print();
    }
};

struct ArrayExpr : public Expr {
    std::vector<std::unique_ptr<Expr>> elements;
    ArrayExpr(std::vector<std::unique_ptr<Expr>> el) : elements(std::move(el)) {}
    void print() const override { std::cout << "[...]"; }
};

struct DictExpr : public Expr {
    std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> pairs;
    DictExpr(std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> p) 
        : pairs(std::move(p)) {}
    void print() const override { std::cout << "{...}"; }
};

struct IndexExpr : public Expr {
    std::unique_ptr<Expr> object;
    std::unique_ptr<Expr> index; 
    IndexExpr(std::unique_ptr<Expr> o, std::unique_ptr<Expr> i) 
        : object(std::move(o)), index(std::move(i)) {}
    void print() const override { 
        object->print(); 
        std::cout << "["; index->print(); std::cout << "]"; 
    }
};

struct BinaryExpr : public Expr {
    char op;
    std::unique_ptr<Expr> lhs, rhs;
    BinaryExpr(char o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(o), lhs(std::move(l)), rhs(std::move(r)) {}
    void print() const override {
        std::cout << "("; lhs->print(); std::cout << " " << op << " "; rhs->print(); std::cout << ")";
    }
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void print(int indent = 0) = 0;
};

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
    void print() const { for (auto& stmt : statements) stmt->print(0); }
};

struct ClearStmt : public Stmt {
    void print(int indent = 0) override { std::cout << std::string(indent, ' ') << "ClearScreen\n"; }
};

struct SetStmt : public Stmt {
    std::string name;
    std::unique_ptr<Expr> expression;
    SetStmt(const std::string& n, std::unique_ptr<Expr> e) : name(n), expression(std::move(e)) {}
    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Set: " << name << " = ";
        if(expression) expression->print(); std::cout << "\n";
    }
};

struct SetIndexStmt : public Stmt {
    std::unique_ptr<Expr> list;   
    std::unique_ptr<Expr> index; 
    std::unique_ptr<Expr> value;  

    SetIndexStmt(std::unique_ptr<Expr> l, std::unique_ptr<Expr> i, std::unique_ptr<Expr> v)
    : list(std::move(l)), index(std::move(i)), value(std::move(v)) {}

    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "SetIndex [...]\n";
    }
};

struct WhileStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> body;
    WhileStmt(std::unique_ptr<Expr> cond) : condition(std::move(cond)) {}
    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "While\n";
        for (auto& s : body) s->print(indent + 2);
    }
};

struct IfStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> thenBranch;
    std::vector<std::unique_ptr<Stmt>> elseBranch;
    IfStmt(std::unique_ptr<Expr> cond) : condition(std::move(cond)) {}
    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "If\n";
        for (auto& s : thenBranch) s->print(indent + 2);
        if (!elseBranch.empty()) { std::cout << std::string(indent, ' ') << "Else\n"; for (auto& s : elseBranch) s->print(indent + 2); }
    }
};

struct ForStmt : public Stmt { 
    std::string iteratorName;
    std::unique_ptr<Expr> collection; 
    std::vector<std::unique_ptr<Stmt>> body;

    ForStmt(const std::string& iter, std::unique_ptr<Expr> col) 
        : iteratorName(iter), collection(std::move(col)) {}
        
    void print(int indent = 0) override { 
        std::cout << std::string(indent, ' ') << "For " << iteratorName << " in Expr\n";
        for(auto& s : body) s->print(indent + 2);
    }
};

struct FuncDecl : public Stmt {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<Stmt>> body;
    FuncDecl(const std::string& n, std::vector<std::string> p) : name(n), params(std::move(p)) {}
    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Func " << name << "\n";
        for (auto& stmt : body) stmt->print(indent + 2);
    }
};

 
struct ClassDecl : public Stmt {
    std::string name;
    std::vector<std::unique_ptr<FuncDecl>> methods;  

    ClassDecl(std::string n, std::vector<std::unique_ptr<FuncDecl>> m) 
    : name(n), methods(std::move(m)) {}

    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Class " << name << "\n";
        for (auto& m : methods) m->print(indent + 2);
    }
};

struct CallStmt : public Stmt {
    std::string func;
    std::vector<std::unique_ptr<Expr>> args;
    CallStmt(const std::string& f, std::vector<std::unique_ptr<Expr>> a) : func(f), args(std::move(a)) {}
    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Call " << func << "\n";
    }
};

struct UpdateStmt : public Stmt {
    std::string name;
    std::string op;
    UpdateStmt(const std::string& n, const std::string& o) : name(n), op(o) {}
    void print(int indent = 0) override { std::cout << std::string(indent, ' ') << "Update " << name << "\n"; }
};

struct PropertyStmt : public Stmt {
    std::string name;
    std::string value;
    PropertyStmt(const std::string& n, const std::string& v) : name(n), value(v) {}
    void print(int indent = 0) override { std::cout << "Prop " << name << "\n"; }
};

struct AppDecl : public Stmt {
    std::string name;
    std::vector<std::unique_ptr<Stmt>> body;
    AppDecl(const std::string& n) : name(n) {}
    void print(int indent = 0) override { std::cout << "App " << name << "\n"; for (auto& s : body) s->print(indent+2); }
};

struct WindowDecl : public Stmt {
    std::string name;
    std::vector<std::unique_ptr<Stmt>> body;
    WindowDecl(const std::string& n) : name(n) {}
    void print(int indent = 0) override { std::cout << "Window " << name << "\n"; for (auto& s : body) s->print(indent+2); }
};

struct ConnectStmt : public Stmt {
    std::string source, event, target;
    ConnectStmt(const std::string& s, const std::string& e, const std::string& t) : source(s), event(e), target(t) {}
    void print(int indent = 0) override { std::cout << "Connect\n"; }
};

struct ReturnStmt : public Stmt {
	std::unique_ptr<Expr> value; 
	ReturnStmt(std::unique_ptr<Expr> v) : value(std::move(v)) {} 
	
	void print(int indent = 0) override {
		std::cout << std::string(indent, ' ') << "Return "; 
		std::cout << "\n"; 
	}
}; 

struct ImportStmt : public Stmt { 
	std::string path; 
	ImportStmt(std::string p) : path(p) {}
	
	void print(int indent = 0) override {
		std::cout << std::string(indent, ' ') << "Import: " << path << "\n"; 
	}
}; 

struct TryStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> tryBody;
    std::vector<std::unique_ptr<Stmt>> catchBody;
    std::string errorVar; 

    TryStmt(std::vector<std::unique_ptr<Stmt>> tb, 
            std::vector<std::unique_ptr<Stmt>> cb, 
            std::string ev) 
    : tryBody(std::move(tb)), catchBody(std::move(cb)), errorVar(ev) {}

    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Try\n";
        for (auto& s : tryBody) s->print(indent + 2);
        std::cout << std::string(indent, ' ') << "Catch (" << errorVar << ")\n";
        for (auto& s : catchBody) s->print(indent + 2);
    }
};

 
struct NewExpr : public Expr {
    std::string className;
    std::vector<std::unique_ptr<Expr>> args;
    
    NewExpr(std::string n, std::vector<std::unique_ptr<Expr>> a) 
    : className(n), args(std::move(a)) {}
    
    void print() const override { std::cout << "new " << className << "(...)"; }
}; 

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    ExprStmt(std::unique_ptr<Expr> e) : expression(std::move(e)) {}
    
    void print(int indent = 0) override { 
        std::cout << std::string(indent, ' ') << "ExprStmt\n";
        if(expression) expression->print(); 
        std::cout << "\n";
    }
};

struct BreakStmt : public Stmt {
    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Break\n";
    }
};

struct ContinueStmt : public Stmt {
    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Continue\n";
    }
};

struct ExternStmt : public Stmt {
    std::string lang;
    std::string flags; 
    std::string code;
    
    ExternStmt(std::string l, std::string f, std::string c) : lang(l), flags(f), code(std::move(c)) {}

    void print(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Extern \"" << lang << "\" Flags: [" << flags << "] {\n";
        std::cout << std::string(indent + 2, ' ') << "[Raw Native Code Block]\n";
        std::cout << std::string(indent, ' ') << "}\n";
    }
};
