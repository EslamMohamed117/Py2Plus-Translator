#include "Scanner.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;
using namespace boost;


string code;
/**
 * @brief Checks if a character is an operator.
 * @param c The character to check.
 * @return true if the character is an operator, false otherwise.
 */
bool Scanner::IsOperator(char c) {
    static const std::string operators = "+-*/%=><!&|^~";
    return operators.find(c) != std::string::npos;
}

/**
 * @brief Checks if a character is a separator.
 * @param c The character to check.
 * @return true if the character is a separator, false otherwise.
 */
bool Scanner::IsSeprator(char c) {
    static const std::string separators = ".,:;()[]{}";
    return separators.find(c) != std::string::npos;
}

/**
 * @brief Tokenizes the code stored in the `code` member variable of the `Scanner` class.
 * @return true if tokenization is successful, false otherwise.
 */
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

/**
 * @brief Classifies a token.
 * @param tokenStr The token to classify.
 * @return A `TokenInfo` object representing the classification of the token.
 */
TokenInfo Scanner::ClassifyToken(const std::string& tokenStr) {
	
    // If the token is empty, return an end-of-file token
    if (tokenStr.empty()) {
        return { tokens[TOKEN_END_OF_FILE], "" };
    }

    // Check if the token is a keyword
    auto it = keywords.find(tokenStr);
    if (it != keywords.end()) return { keywords.at(tokenStr), tokenStr };

    // Get the first character of the token
    char c = tokenStr[0];
	
    // Check if the first character is whitespace
    if (std::isspace(c)) {
        return { tokens[TOKEN_WHITESPACE], tokenStr };
    }

    // Check if the first character is a digit
    if (std::isdigit(c)) {
        std::string value(tokenStr);
        while (value.size() < tokenStr.size() && std::isdigit(tokenStr[value.size()])) {
            value += tokenStr[value.size()];
        }
        return { tokens[TOKEN_NUMBER], value };
    }
	
    // Check if the first character is a quote or an alphanumeric character that is not a digit
    if (c == '"' || c == '\'' || (std::isalnum(c) && !std::isdigit(c))) {
        std::string value(tokenStr);
        while (value.size() < tokenStr.size() && value.back() != c) {
            value += tokenStr[value.size()];
        }
        return { tokens[TOKEN_STRING], value };
    }
	
    // Check if the first character is an operator
    if (IsOperator(c)) {
        return { tokens[TOKEN_OPERATOR], tokenStr };
    }

    // Check if the first character is a separator
    static const std::string separators = ".,:;()[]{}";
    if (separators.find(c) != std::string::npos) {
        return { tokens[TOKEN_SEPARATOR], tokenStr};
    }
	
    // If the first character is not alphanumeric, not a separator, not an operator, and not an underscore,
    // return an invalid token
    if (!std::isalnum(c) && !IsSeprator(c) && !IsOperator(c) && c != '_') {
        return { tokens[TOKEN_INVALID], tokenStr };
    }
	
    // Otherwise, return an invalid token
    return { tokens[TOKEN_INVALID], tokenStr };
}

/**
 * @brief Returns the tokenized code.
 * @return A vector of `TokenInfo` objects representing the tokenized code.
 */
vector<TokenInfo> Scanner::GetTokenList()
{
    return this->tokenList;
}

/**
 * @brief Removes empty lines from a string.
 * @param input The string to remove empty lines from.
 * @return A new string with all empty lines removed.
 */
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