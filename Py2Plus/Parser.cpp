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
        IncrementToken();
    else
        Error();
}

/* Increment the token */
void Parser::IncrementToken()
{
    lookAheadToken = lookAheadTokenList[++currentToken].tokenValue;
}

bool Parser::NameIdentifier(string id)
{
    return true;
}

/* Statements: 
 * <statements>   -> <statement> <statements> | epslion
*/
void Parser::Statements()
{
    /* If the current token is not the end of the file */
    /* Check the current statement */
    if(lookAheadToken == "if" || lookAheadToken == "for" || lookAheadToken == "while" || NameIdentifier(lookAheadToken))
    {
        Statement();
        //return;
        Statements();
    }
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
        If_Statement();
    else if (lookAheadToken == "for")
        ;//For_Statement();
    else if (lookAheadToken == "while")
        ;//While_Statement();
    else if (lookAheadToken == "do")
        ;//DoWhile_Statement();
    else
        ;//Assignment_Statement();
}

/************************************************************************************************
 ************************************** Statements *********************************************
************************************************************************************************/

/* If Statement:
 * <if_statement> -> "if" <expression> ":" <statements> <elif_statement>* <else_statement>?
*/
void Parser::If_Statement()
{
    Match("if");
    if (lookAheadToken == "(")      /* If the next token is an opening parenthesis */
        Match("(");
    py2PlusCode += "if (";
    
    Expression_Statement();

    if (lookAheadToken == ")")      /* If the next token is an opening parenthesis */
        Match(")");
    py2PlusCode += ")";
    //Match(":");
    //Statements();
    //Elif_Statement();
    //Else_Statement();
    /*
        if x == 5:
            print("x is 5")
        elif x == 6
            print("x is 6") 
            print("OK") ??? statments???
        elif x == 7
            print("x is 7")
        else
            print("x is not 6")
    */
}

/* Elif Statement:
 * <elif_statement> -> "elif" <expression_statement> ":" <statements> | epslion
*/
void Parser::Elif_Statement()
{
    //Match("elif");
    //Expression_Statement();
    //Match(":");
    //Statements();
}

/* Else Statement:
 * <else_statement> -> "else" ":" <statements>
*/
void Parser::Else_Statement()
{
    //Match("else");
    //Match(":");
    //Statements();
}

/* Expression Statement
 * <expression_statement> -> <term> | <expression> <add_operator> <term>
*/
void Parser::Expression_Statement()
{

}

/* Expression Statement
 * <if_statement> -> <term> | <expression> <add_operator> <term>
*/
void Parser::Term_Statement()
{

}

/************************************************************************************************
 ************************************** Identifiers ********************************************
************************************************************************************************/

/* Name Identifier (Based on Regex)
 * <name_identifier> -> [a-zA-Z_][a-zA-Z0-9_]*
*/
bool NameIdentifier(string id)
{
    bool status = true;
    
    regex nameRegex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    regex_match(id, nameRegex) ? status = true : status = false;

    return status;
}
void NumberIdentifier()
{

}




/* scanner.tokenList[0].tokenValue */


void Parser::Error()
{
    cerr << "Error";
}

Parser::Parser(vector<TokenInfo> tokenList)
{
    py2PlusCode = "";
    lookAheadTokenList = tokenList;
    currentToken = 0;
}

/* Parse the code */
string Parser::Parse()
{

    /* Get the list of tokens from the scanner */
    lookAheadToken = lookAheadTokenList[0].tokenValue;

    /* Starts the statements of the code */
    Statements();

    return py2PlusCode;
}