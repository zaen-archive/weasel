#include <iostream>
#include "zero/symbol/symbol.h"

/// SYMBOL ///
std::vector<std::shared_ptr<zero::Error>> zero::ErrorTable::_errors;

zero::SymbolTable::SymbolTable()
{
    enterScope();
}

void zero::SymbolTable::enterScope()
{
    _lookup.push_back(0);
}

bool zero::SymbolTable::exitScope()
{
    if (_lookup.size() == 1)
    {
        return false;
    }

    auto n = _lookup.back();
    while (n--)
    {
        _table.pop_back();
    }

    _lookup.pop_back();
    return true;
}

void zero::SymbolTable::insert(std::string key, std::shared_ptr<Attribute> attr)
{
    _table.push_back(attr);
    _lookup[_lookup.size() - 1]++;
}

std::shared_ptr<zero::Attribute> zero::SymbolTable::get(std::string key)
{
    auto n = _table.size() - 1;
    for (int i = n; i >= 0; i--)
    {
        if (_table[i]->getIdentifier() == key)
        {
            return _table[i];
        }
    }

    return nullptr;
}

std::shared_ptr<zero::Attribute> zero::SymbolTable::getLastFunction()
{
    auto i = _table.size() - 1;
    for (; i >= 0; i--)
    {
        auto attr = _table[i];

        if (attr->getKind() == AttributeKind::SymbolFunction)
        {
            return attr;
        }
    }

    return nullptr;
}

void zero::SymbolTable::reset()
{
    while (!_table.empty())
    {
        _table.pop_back();
    }

    while (!_lookup.empty())
    {
        _lookup.pop_back();
    }

    enterScope(); // Enter Global Scope
}

std::nullptr_t zero::ErrorTable::addError(std::shared_ptr<Token> token, std::string msg)
{
    _errors.push_back(std::make_shared<Error>(token, msg));
    return nullptr;
}

void zero::ErrorTable::showErrors()
{
    if (_errors.empty())
    {
        std::cerr << "No Error found\n";
    }
    else
    {
        for (auto item : _errors)
        {
            auto token = item->getToken();
            auto loc = token->getLocation();

            std::cerr << "Error : " << item->getMessage() << " but found " << token->getValue() << " kind of " << token->getTokenKindToInt();
            std::cerr << " At (" << loc.row << ":" << loc.col << ")\n";
        }
    }
}
