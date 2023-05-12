#pragma once
#include <boost/tokenizer.hpp>
#include <map>

typedef boost::tokenizer<boost::char_separator<char> > boostTokenizer;

const std::string tokens[] = {
        "TOKEN_END_OF_FILE",
        "TOKEN_WHITESPACE",
        "TOKEN_INVALID",
        "TOKEN_NUMBER",
        "TOKEN_IDENTIFIER",
        "TOKEN_STRING",
        "TOKEN_OPERATOR",
        "TOKEN_SEPARATOR",
        "KEYWORD_IMPORT",
        "KEYWORD_AND",
        "KEYWORD_OR",
        "KEYWORD_NONE",
        "KEYWORD_CONTINUE",
        "KEYWORD_BREAK",
        "KEYWORD_FOR",
        "KEYWORD_IN",
        "KEYWORD_WHILE",
        "KEYWORD_DEF",
        "KEYWORD_IF",
        "KEYWORD_ELIF",
		"KEYWORD_ELSE",
        "KEYWORD_PRINT",
        "KEYWORD_RETURN" };

const std::map<std::string, std::string> keywords = {
       {"import"    ,   "KEYWORD_IMPORT"      },
       {"and"       ,      "KEYWORD_AND"      },
       {"or"        ,       "KEYWORD_OR"      },
       {"None"      ,     "KEYWORD_NONE"      },
       {"continue"  , "KEYWORD_CONTINUE"      },
       {"break"     ,    "KEYWORD_BREAK"      },
       {"for"       ,      "KEYWORD_FOR"      },
       {"in"        ,       "KEYWORD_IN"      },
       {"while"     ,    "KEYWORD_WHILE"      },
       {"def"       ,      "KEYWORD_DEF"      },
       {"if"        ,       "KEYWORD_IF"      },
       {"elif"      ,     "KEYWORD_ELIF"      },
       {"else"      ,     "KEYWORD_ELSE"      },
       {"print"     ,    "KEYWORD_PRINT"      },
       {"return"    ,   "KEYWORD_RETURN"      }
};

enum Token {
    TOKEN_END_OF_FILE,
    TOKEN_WHITESPACE,
    TOKEN_INVALID,
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    KEYWORD_IMPORT,
    KEYWORD_AND,
    KEYWORD_OR,
    KEYWORD_NONE,
    KEYWORD_CONTINUE,
    KEYWORD_BREAK,
    KEYWORD_FOR,
    KEYWORD_IN,
    KEYWORD_WHILE,
    KEYWORD_DEF,
    KEYWORD_IF,
    KEYWORD_ELIF,
	KEYWORD_ELSE,
    KEYWORD_PRINT,
    KEYWORD_RETURN,
    //    KEYWORD_GLOBAL,
    //    KEYWORD_LAMBDA,
    //    KEYWORD_PASS,
    //    KEYWORD_RAISE,
    //    KEYWORD_REP,                        // Unused keywords
    //    KEYWORD_TRY,
    //    KEYWORD_WITH,
    //    KEYWORD_VAR,
    //    KEYWORD_YIELD
};

struct TokenInfo {
    std::string token;
    std::string tokenValue;
};

class Scanner
{
private:
    std::string code;
    bool IsOperator(char c);
    bool IsSeprator(char c);
    std::vector<TokenInfo> tokenList;
public:
    Scanner(std::string code) { this->code = code; }
    bool Tokenize();
    TokenInfo ClassifyToken(const std::string& tokenStr);
	std::vector<TokenInfo> GetTokenList();
};