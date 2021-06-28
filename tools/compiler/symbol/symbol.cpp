#include <iostream>
#include "weasel/symbol/symbol.h"

/// Symbol Init ///
std::vector<std::shared_ptr<weasel::Error>> weasel::ErrorTable::_errors;
std::vector<unsigned> weasel::SymbolTable::_lookup;
std::vector<std::shared_ptr<weasel::Attribute>> weasel::SymbolTable::_table;

/// Symbol Function ///
void weasel::SymbolTable::enterScope()
{
    _lookup.push_back(0);
}

bool weasel::SymbolTable::exitScope()
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

void weasel::SymbolTable::insert(const std::string &key, const std::shared_ptr<Attribute> &attr)
{
    if (_lookup.empty())
    {
        _lookup.push_back(0);
    }

    _table.push_back(attr);
    _lookup[_lookup.size() - 1]++;
}

std::shared_ptr<weasel::Attribute> weasel::SymbolTable::get(const std::string &key)
{
    auto i = (int)_table.size() - 1;
    for (; i >= 0; i--)
    {
        if (_table[i]->getIdentifier() == key)
        {
            return _table[i];
        }
    }

    return nullptr;
}

std::shared_ptr<weasel::Attribute> weasel::SymbolTable::getLastFunction()
{
    auto i = (int)_table.size() - 1;
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

void weasel::SymbolTable::reset()
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

std::nullptr_t weasel::ErrorTable::addError(const std::shared_ptr<Token> &token, std::string msg)
{
    _errors.push_back(std::make_shared<Error>(token, msg));
    return nullptr;
}

void weasel::ErrorTable::showErrors()
{
    if (_errors.empty())
    {
        std::cerr << "No Error found\n";
    }
    else
    {
        for (const auto &item : _errors)
        {
            auto token = item->getToken();
            auto loc = token->getLocation();

            std::cerr << "Error : " << item->getMessage() << " but found " << token->getValue() << " kind of " << token->getTokenKindToInt();
            std::cerr << " At (" << loc.row << ":" << loc.col << ")\n";
        }
    }
}
