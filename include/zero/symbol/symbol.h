#include <vector>
#include <string>
#include "zero/lex/token.h"
#include "llvm/IR/Value.h"

namespace zero
{
    enum class AttributeKind
    {
        SymbolStruct,
        SymbolFunction,
        SymbolVariable,
        SymbolParameter
    };

    enum class AttributeScope
    {
        ScopeGlobal,
        ScopeParam,
        ScopeLocal,
    };

    // Attribute
    // scope, symbol kind, identifier
    class Attribute
    {
    private:
        AttributeKind _kind;
        AttributeScope _scope;
        std::string _identifier;
        llvm::Type *_type;   // Used when lexical process
        llvm::Value *_value; // used when LLVM Codegen process or semantical process

    public:
        Attribute(std::string &identifier, AttributeScope scope, AttributeKind kind, llvm::Type *type) : _identifier(identifier), _scope(scope), _kind(kind), _type(type) {}
        Attribute(std::string &identifier, AttributeScope scope, AttributeKind kind, llvm::Value *value) : _identifier(identifier), _scope(scope), _kind(kind), _value(value) {}

        AttributeKind getKind() const { return _kind; }
        AttributeScope getScope() const { return _scope; }
        std::string getIdentifier() const { return _identifier; }
        llvm::Type *getType() const { return _type; }
        llvm::Value *getValue() const { return _value; }
    };

    // Lexical Symbol
    class SymbolTable
    {
    private:
        SymbolTable();

        std::vector<Attribute *> _table;
        std::vector<unsigned> _lookup;

    public:
        void reset();
        void enterScope();
        bool exitScope();
        void insert(std::string key, Attribute *att);
        Attribute *get(std::string key);
        Attribute *getLastFunction();

        std::vector<unsigned> getLookup() const { return _lookup; }

    public:
        SymbolTable(const SymbolTable &) = delete;
        SymbolTable &operator=(const SymbolTable &) = delete;
        static SymbolTable &SymbolTable::getInstance()
        {
            static SymbolTable instance;
            return instance;
        }
    };

} // namespace zero

// Error
namespace zero
{
    // Error
    class Error
    {
    private:
        Token *_token;
        std::string _msg;

    public:
        Error(Token *token, std::string &msg) : _token(token), _msg(msg) {}
        Error(Token *token, const char *msg) : _token(token), _msg(std::string(msg)) {}

        Token *getToken() const { return _token; }
        std::string getMessage() const { return _msg; }
    };

    // Error Table
    class ErrorTable
    {
    private:
        ErrorTable() {}

        static std::vector<Error *> _errors;

    public:
        static void showErrors();
        static std::vector<Error *> getErrors() { return _errors; }
        static std::nullptr_t addError(Error *err);
        static std::nullptr_t addError(Token *token, std::string msg);
    };

} // namespace zero
