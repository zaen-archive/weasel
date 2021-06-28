#include <utility>
#include <vector>
#include <string>
#include "llvm/IR/Value.h"
#include "weasel/lex/token.h"

namespace weasel
{
    enum class AttributeKind
    {
        SymbolStruct,
        SymbolFunction,
        SymbolVariable,
        SymbolParameter,
        SymbolArray,
        SymbolPointer,
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
        std::string _identifier;
        AttributeScope _scope;
        AttributeKind _kind;
        llvm::Type *_type = nullptr;   // Used when lexical process
        llvm::Value *_value = nullptr; // used when LLVM Codegen process or semantically process

    public:
        Attribute(std::string identifier, AttributeScope scope, AttributeKind kind, llvm::Type *type) : _identifier(std::move(identifier)), _scope(scope), _kind(kind), _type(type) {}
        Attribute(std::string identifier, AttributeScope scope, AttributeKind kind, llvm::Value *value) : _identifier(std::move(identifier)), _scope(scope), _kind(kind), _value(value) {}

        inline bool isKind(AttributeKind kind) const { return _kind == kind; }
        inline bool isScope(AttributeScope scope) const { return _scope == scope; }

        inline AttributeKind getKind() const { return _kind; }
        inline AttributeScope getScope() const { return _scope; }
        inline std::string getIdentifier() const { return _identifier; }
        inline llvm::Type *getType() const { return _type; }
        inline llvm::Value *getValue() const { return _value; }
    };

    // Lexical Symbol
    class SymbolTable
    {
    private:
        SymbolTable() = default;

        static std::vector<std::shared_ptr<Attribute>> _table;
        static std::vector<unsigned> _lookup;

    public:
        static void reset();
        static void enterScope();
        static bool exitScope();
        static void insert(const std::string& key, const std::shared_ptr<Attribute>& attr);
        static std::shared_ptr<Attribute> get(const std::string& key);
        static std::shared_ptr<Attribute> getLastFunction();

        static inline std::vector<unsigned> getLookup() { return _lookup; }
    };

} // namespace weasel

// Error
namespace weasel
{
    // Error
    class Error
    {
    private:
        std::shared_ptr<Token> _token;
        std::string _msg;

    public:
        Error(std::shared_ptr<Token> token, std::string &msg) : _token(std::move(token)), _msg(msg) {}
        Error(std::shared_ptr<Token> token, const char *msg) : _token(std::move(token)), _msg(std::string(msg)) {}

        std::shared_ptr<Token> getToken() const { return _token; }
        std::string getMessage() const { return _msg; }
    };

    // Error Table
    class ErrorTable
    {
    private:
        ErrorTable() = default;

        static std::vector<std::shared_ptr<Error>> _errors;

    public:
        static void showErrors();
        static std::vector<std::shared_ptr<Error>> getErrors() { return _errors; }
        static std::nullptr_t addError(const std::shared_ptr<Token>& token, std::string msg);
    };

} // namespace weasel
