#include <vector>
#include <string>
#include "zero/lex/token.h"
#include "llvm/IR/Value.h"

namespace weasel
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
        Attribute(std::string identifier, AttributeScope scope, AttributeKind kind, llvm::Type *type) : _identifier(identifier), _scope(scope), _kind(kind), _type(type) {}
        Attribute(std::string identifier, AttributeScope scope, AttributeKind kind, llvm::Value *value) : _identifier(identifier), _scope(scope), _kind(kind), _value(value) {}

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
        SymbolTable();

        std::vector<std::shared_ptr<Attribute>> _table;
        std::vector<unsigned> _lookup;

    public:
        void reset();
        void enterScope();
        bool exitScope();
        void insert(std::string key, std::shared_ptr<Attribute> attr);
        std::shared_ptr<Attribute> get(std::string key);
        std::shared_ptr<Attribute> getLastFunction();

        inline std::vector<unsigned> getLookup() const { return _lookup; }

    public:
        SymbolTable(const SymbolTable &) = delete;
        SymbolTable &operator=(const SymbolTable &) = delete;
        static SymbolTable &getInstance()
        {
            static SymbolTable instance;
            return instance;
        }
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
        Error(std::shared_ptr<Token> token, std::string &msg) : _token(token), _msg(msg) {}
        Error(std::shared_ptr<Token> token, const char *msg) : _token(token), _msg(std::string(msg)) {}

        std::shared_ptr<Token> getToken() const { return _token; }
        std::string getMessage() const { return _msg; }
    };

    // Error Table
    class ErrorTable
    {
    private:
        ErrorTable() {}

        static std::vector<std::shared_ptr<Error>> _errors;

    public:
        static void showErrors();
        static std::vector<std::shared_ptr<Error>> getErrors() { return _errors; }
        static std::nullptr_t addError(std::shared_ptr<Token> token, std::string msg);
    };

} // namespace weasel
