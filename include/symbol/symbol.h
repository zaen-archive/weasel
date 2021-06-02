#include <vector>
#include <map>
#include <string>
#include "lex/token.h"

namespace llvm
{
    class Type;
} // namespace llvm

namespace underrated
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
        llvm::Type *_type;

    public:
        Attribute(std::string &identifier, AttributeScope scope, AttributeKind kind, llvm::Type *type) : _identifier(identifier), _scope(scope), _kind(kind), _type(type) {}

        AttributeKind getKind() const { return _kind; }
        AttributeScope getScope() const { return _scope; }
        std::string getIdentifier() const { return _identifier; }
        llvm::Type *getType() const { return _type; }
    };

    // Symbol Table
    class SymbolTable
    {
    private:
        std::vector<Attribute *> _table;
        std::vector<unsigned> _lookup;

    private:
        static SymbolTable *_instance;

        SymbolTable();

    public:
        void enterScope();
        bool exitScope();
        void insert(std::string key, Attribute *att);
        Attribute *get(std::string key);
        Attribute *lastFunction();

        std::vector<unsigned> getLookup() const { return _lookup; }

    public:
        static SymbolTable *getInstance() { return SymbolTable::_instance; }
    };

} // namespace underrated

namespace underrated
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
    };

} // namespace underrated
