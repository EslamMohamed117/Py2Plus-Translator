#include "Scanner.h"

#include <iostream>
#include <string>
using namespace std;
using namespace boost;


string code;
bool Scanner::IsOperator(char c) {
    static const std::string operators = "+-*/%=><!&|^~";
    return operators.find(c) != std::string::npos;
}

bool Scanner::Tokenize()
{
    try {
        char_separator<char> sep; // default constructed
        boostTokenizer tokenList(this->code, sep);
        for (boostTokenizer::iterator token_iter = tokenList.begin(); token_iter != tokenList.end(); ++token_iter)
            this->tokenList.push_back(ClassifyToken(*token_iter));
        return true;
    }
    catch (const std::exception& e) {
        // Handle the exception here.
        std::cerr << "Tokenizer error: " << e.what() << std::endl;
        return false;
    }
}

TokenInfo Scanner::ClassifyToken(const std::string& tokenStr) {
    if (tokenStr.empty()) {
        return { tokens[TOKEN_END_OF_FILE], "" };
    }

    auto it = keywords.find(tokenStr);
    if (it != keywords.end()) return { keywords.at(tokenStr), tokenStr };

    char c = tokenStr[0];
    if (std::isspace(c)) {
        return { tokens[TOKEN_WHITESPACE], tokenStr };
    }

    if (!std::isalnum(c) && !IsOperator(c) && c != '_') {
        return { tokens[TOKEN_INVALID], tokenStr };
    }

    if (std::isdigit(c)) {
        std::string value(tokenStr);
        while (value.size() < tokenStr.size() && std::isdigit(tokenStr[value.size()])) {
            value += tokenStr[value.size()];
        }
        return { tokens[TOKEN_NUMBER], value };
    }
    if (c == '"' || c == '\'' || (std::isalnum(c) && !std::isdigit(c))) {
        std::string value(tokenStr);
        while (value.size() < tokenStr.size() && value.back() != c) {
            value += tokenStr[value.size()];
        }
        return { tokens[TOKEN_STRING], value };
    }

    if (IsOperator(c)) {
        return { tokens[TOKEN_OPERATOR], tokenStr };
    }

    static const std::string separators = ".,:;()[]{}";
    if (separators.find(c) != std::string::npos) {
        std::string value(tokenStr);
        std::string tokenStr = "TOKEN_SEPARATOR_" + value;
        Token token = static_cast<Token>(std::hash<std::string>{}(tokenStr));
        return { tokens[token], value };
    }

    return { tokens[TOKEN_INVALID], tokenStr };
}

vector<TokenInfo> Scanner::GetTokenList()
{
    return this->tokenList;
}
