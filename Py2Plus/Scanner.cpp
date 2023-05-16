#include "Scanner.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;
using namespace boost;


string code;
bool Scanner::IsOperator(char c) {
    static const std::string operators = "+-*/%=><!&|^~";
    return operators.find(c) != std::string::npos;
}

bool Scanner::IsSeprator(char c) {
    static const std::string separators = ".,:;()[]{}";
    return separators.find(c) != std::string::npos;
}


bool Scanner::Tokenize()
{
    try {
        std::istringstream iss(this->code);
        std::string line;
        int indentLevel = 0;
        while (getline(iss, line)) {
            // Count the number of spaces at the start of the line
            int spaces = 0;
            while (std::isspace(line[spaces])) spaces++;

            // Calculate the new indentation level
            int newIndentLevel = spaces / 4; // assuming 4 spaces per indent

            // Generate INDENT and DEDENT tokens
            while (newIndentLevel > indentLevel) {
                this->tokenList.push_back({ tokens[TOKEN_INDENT], "@" });
                indentLevel++;
            }
            while (newIndentLevel < indentLevel) {
                this->tokenList.push_back({ tokens[TOKEN_DEDENT], "#" });
                indentLevel--;
            }

            // Tokenize the rest of the line
            char_separator<char> sep; // default constructed
            boostTokenizer tokenList(line, sep);
            for (boostTokenizer::iterator token_iter = tokenList.begin(); token_iter != tokenList.end(); ++token_iter) {
                this->tokenList.push_back(ClassifyToken(*token_iter));
            }
        }
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
        return { tokens[TOKEN_SEPARATOR], tokenStr};
    }

    if (!std::isalnum(c) && !IsSeprator(c) && !IsOperator(c) && c != '_') {
        return { tokens[TOKEN_INVALID], tokenStr };
    }
	
    return { tokens[TOKEN_INVALID], tokenStr };
}

vector<TokenInfo> Scanner::GetTokenList()
{
    return this->tokenList;
}

std::string Scanner::removeEmptyLines(std::string input)
{
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string line;
    while (std::getline(iss, line))
    {
        if (line.find_first_not_of(" \t") != std::string::npos)
            oss << line << '\n';
    }
    return oss.str();
}