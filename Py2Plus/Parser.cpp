#include <iostream>
#include <string>
#include <vector>
#include "Scanner.h"
#include <regex>

using namespace std;

#include "Parser.h"



/* Match the look-ahead token with the current token */
void Parser::Match(string token)
{
    if (lookAheadToken == token)
    {
        py2PlusCode += lookAheadToken;          /* Add the token to the converted code */
        IncrementToken();                       /* Get next token */
    }
    else
        Error();
}

/* Increment the token */
void Parser::IncrementToken()
{
    if (currentToken < lookAheadTokenList.size() - 1)
        lookAheadToken = lookAheadTokenList[++currentToken].tokenValue;
    else
        return;
}


/* Statements: 
 * <statements>   -> <statement> <statements> | epslion
*/
void Parser::Statements()
{
    /* If the current token is not the end of the file */
    /* Check the current statement */
    if (lookAheadToken == "if" || lookAheadToken == "for" || lookAheadToken == "while" ||
        (NameIdentifier(lookAheadToken) && (lookAheadTokenList[currentToken].token == tokens[TOKEN_STRING])))
    {
        Statement();
        //return;
        Statements();
    }
    else if ( (lookAheadToken == "else" || lookAheadToken == "elif") && condition_if != true )
        Error();
    else
        return;
}

/* Statement: 
 * <statement>    -> <if_statement> | <for_statement> | <while_statement> | <do_while_statement> | <assignment_statement>
*/
void Parser::Statement()
{
    /* Check the current statement */
    if (lookAheadToken == "if")
    {
        condition_if = true;
        If_Statement();
        condition_if = false;
    }
    else if (lookAheadToken == "for")
        ;//For_Statement();
    else if (lookAheadToken == "while")
        ;//While_Statement();
    else if (lookAheadToken == "do")
        ;//DoWhile_Statement();
    else if (NameIdentifier(lookAheadToken) && (lookAheadTokenList[currentToken].token == tokens[TOKEN_STRING]) )
        Assignment_Statement();
    else
        Error();
}

/************************************************************************************************
 ************************************** Statements *********************************************
************************************************************************************************/

/* If Statement:
 * <if_statement> -> "if" <conditional_statement> ":" <statements> <if_opt_stmt>*
*/
void Parser::If_Statement()
{
    Match("if");
    if (lookAheadToken == "(")      /* If the next token is an opening parenthesis */
        Match("(");
    else
        py2PlusCode += " (";
    
    Conditional_Statement();

    if (lookAheadToken == ")")      /* If the next token is an opening parenthesis */
        Match(" )");
    else
        py2PlusCode += ")";
        
    /* If the next token is a colon */
    if (lookAheadToken == ":")
        IncrementToken();
    else
        Match(":");
    py2PlusCode += "\n{\n\t";
    Statements();
    py2PlusCode += "}\n";
    If_optStmt();
}

/* If Optional Statement:
 * <if_opt_stmt>  -> <elif_statement> <if_opt_stmt> | <else_statement> | epslion
*/
void Parser::If_optStmt()
{
    if (lookAheadToken == "elif")
    {
        Elif_Statement();
        If_optStmt();
    }
    else if (lookAheadToken == "else")
        Else_Statement();
    else
        return;
}

/* Elif Statement:
 * <elif_statement> -> "elif" <conditional_statement> ":" <statements> | epslion
*/
void Parser::Elif_Statement()
{
    Match("elif");
    if (lookAheadToken == "(")      /* If the next token is an opening parenthesis */
        Match("(");
    else
        py2PlusCode += " (";
    
    Conditional_Statement();

    if (lookAheadToken == ")")      /* If the next token is an opening parenthesis */
        Match(" )");
    else
        py2PlusCode += ")";
        
    /* If the next token is a colon */
    if (lookAheadToken == ":")
        IncrementToken();
    else
        Match(":");
    py2PlusCode += "\n{\t";
    Statements();
    py2PlusCode += "\n}\n";
}

/* Else Statement:
 * <else_statement> -> "else" ":" <statements>
*/
void Parser::Else_Statement()
{
    Match("else");
    /* If the next token is a colon */
    if (lookAheadToken == ":")
        IncrementToken();
    else
        Match(":");
    py2PlusCode += "\n{\t";
    Statements();
    py2PlusCode += "\n}\n";
}

/* Condtional Statement
 * <conditional_statement> -> <expression_statement> <conditional_operator> <expression_statement>
*/
void Parser::Conditional_Statement()
{
    Expression_Statement();
    if(lookAheadToken == "<" || lookAheadToken == ">" || lookAheadToken == "==" || lookAheadToken == "!=" || lookAheadToken == "<=" || lookAheadToken == ">=")
        Match(lookAheadToken);    /* Match the operator "<" or ">" or "==" or "!=" or "<=" or ">=" */
    else
        Error();
    Expression_Statement();
}

/* Expression Statement
 * <expression_statement>   -> <term> <expression_rest>
*/
void Parser::Expression_Statement()
{
    Term_Statement();
    Expression_Rest();
}

/* Eliminating Left Recursion Of Expression
 * <expression_rest>		-> <addsub_operator> <term> <expression_rest> | epslion
*/
void Parser::Expression_Rest()
{
    if(lookAheadToken == "+" || lookAheadToken == "-")
    {
        Match(lookAheadToken);    /* Match the operator "+" or "-" */
        Term_Statement();
        Expression_Rest();
    }
    else
        return;
}

/* Term Statement
 * <term_statement> -> <factor_statement> <term_rest>
*/
void Parser::Term_Statement()
{
    Factor_Statement();
    Term_Rest();
}

/* Eliminating Left Recursion Of Term
 * <term_rest>		-> <muldiv_operator> <factor_statement> <term_rest> | epslion
*/
void Parser::Term_Rest()
{
    if(lookAheadToken == "*" || lookAheadToken == "/" || lookAheadToken == "%")
    {
        Match(lookAheadToken);    /* Match the operator "*" or "/" or "%" */
        Factor_Statement();
        Term_Rest();
    }
    else
        return;
}

/* Factor Statement
 * <term_statement> -> <name_identifier> | <number_identifier> | "(" <expression_statement> ")"
*/
void Parser::Factor_Statement()
{
    if(NameIdentifier(lookAheadToken) || NumberIdentifier(lookAheadToken))
        Match(lookAheadToken);
    else if(lookAheadToken == "(")
    {
        Match("(");
        Expression_Statement();
        Match(")");
    }
    else
        Error();
}

/* Assignment Statement:
 * <assignment_statement>   ->  <name_identifier> "=" <expression_statement>
*/
void Parser::Assignment_Statement()
{
    if(NameIdentifier(lookAheadToken))
    {
        py2PlusCode += lookAheadToken;
        IncrementToken();
        Match("=");
        Expression_Statement();
    }
    else
        Error();
    py2PlusCode += ";\n";
}

void Parser::Print_Statement()
{
    if(lookAheadToken == "print")
    {
        IncrementToken();
        py2PlusCode += "cout << ";
    }
    else
        Error();
    Match("(");
    Match("\"");
    while(lookAheadToken != "\"")
    {
        py2PlusCode += lookAheadToken;
        IncrementToken();
    }
    Match(")");
    py2PlusCode += ";\n";
}

/************************************************************************************************
 ************************************** Identifiers ********************************************
************************************************************************************************/

/* Name Identifier (Based on Regex)
 * <name_identifier> -> [a-zA-Z_][a-zA-Z0-9_]*
*/
bool Parser::NameIdentifier(string id)
{
    bool status = true;
    
    regex nameRegex("[a-zA-Z_][a-zA-Z0-9_]*");
    regex_match(id, nameRegex) ? status = true : status = false;

    return status;
}


/* Number Identifier (Based on Regex)
 * <number_identifier> -> [-?[0-9]+\\.?[0-9]+]
*/
bool Parser::NumberIdentifier(string num)
{
    bool status = true;

    regex nameRegex("-?[0-9]+.?[0-9]*");
    regex_match(num, nameRegex) ? status = true : status = false;

    return status;
}




/* scanner.tokenList[0].tokenValue */


void Parser::Error()
{
    /* Syntax Error */
    parse.errorStatus = true;
}

Parser::Parser(vector<TokenInfo> tokenList)
{
    py2PlusCode = "";
    lookAheadTokenList = tokenList;
    currentToken = 0;
    parse.py2PlusCode = "";
    parse.errorStatus = false;
}

/* Parse the code */
Parser_t Parser::Parse()
{
    /* Get the list of tokens from the scanner */
    lookAheadToken = lookAheadTokenList[0].tokenValue;

    /* Starts the statements of the code */
    Statements();
    parse.py2PlusCode = py2PlusCode;
    return parse;
}