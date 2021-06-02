#include <iostream>
#include "symbol/symbol.h"

/// SYMBOL ///
underrated::SymbolTable *underrated::SymbolTable::_instance = new underrated::SymbolTable();

std::vector<underrated::Error *> underrated::ErrorTable::_errors;

underrated::SymbolTable::SymbolTable()
{
    enterScope();
}

void underrated::SymbolTable::enterScope()
{
    _lookup.push_back(0);
}

bool underrated::SymbolTable::exitScope()
{
    if (_lookup.size() == 1)
    {
        return false;
    }

    auto n = _lookup.back();
    while (n--)
    {
        delete _table.back();
        _table.pop_back();
    }

    _lookup.pop_back();
    return true;
}

void underrated::SymbolTable::insert(std::string key, Attribute *att)
{
    _table.push_back(att);
    _lookup[_lookup.size() - 1]++;
}

underrated::Attribute *underrated::SymbolTable::get(std::string key)
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

underrated::Attribute *underrated::SymbolTable::lastFunction()
{
    auto i = _table.size() - 1;
    for (; i >= 0; i--)
    {
        auto *attr = _table[i];

        if (attr->getKind() == AttributeKind::SymbolFunction)
        {
            return attr;
        }
    }

    return nullptr;
}

void underrated::ErrorTable::showErrors()
{
    for (auto *item : _errors)
    {
        auto *token = item->getToken();
        auto loc = token->getLocation();

        std::cerr << "Error : Location (" << loc.row << ":" << loc.col << ") Length (" << loc.length << ") Position (" << loc.position << ")\n";
        std::cerr << "Error Message : " << item->getMessage() << "\n";
    }
}

std::nullptr_t underrated::ErrorTable::addError(Error *err)
{
    _errors.push_back(err);
    return nullptr;
}
