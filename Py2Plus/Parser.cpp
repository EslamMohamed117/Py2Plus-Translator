#include <iostream>
#include <string>
#include <vector>
#include "Scanner.h"
#include <regex>

using namespace std;

#include "Parser.h"


/************************************************************************************************
 ***************************************** Match **********************************************
************************************************************************************************/

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
        lookAheadToken = "";    /* Clear the lookahead (epsilon) */
}

/************************************************************************************************
 ************************************** Statements *********************************************
************************************************************************************************/

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
        For_Statement();
    else if (lookAheadToken == "while")
        While_Statement();
    else if (lookAheadToken == "do")
        ;//DoWhile_Statement();
    else if (NameIdentifier(lookAheadToken) && (lookAheadTokenList[currentToken].token == tokens[TOKEN_STRING]) )
        Assignment_Statement();
    else
        Error();
}

/************************************************************************************************
 ********************************** Conditions & Loops *****************************************
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

/* While Statement:
 * <while_statement> -> "while" <expression_statement> ":" <statements>
*/
void Parser::While_Statement()
{
    Match("while");
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

/*
 * For Statement:
 * CFG: <for_statement> -> "for" <name_identifier> "in" <range> ":" <statements>
 * NOTES: Only works for the following syntax:
 * 	      -> for identifier in range(num):
 *        -> for identifier in range(num, num):
 * 	      -> for identifier in range(num, num, num):
 *        Only increamenting loop is supported for now  
 */
void Parser::For_Statement() {
    /* Declare the iterator */
    string iterator = "";

    /* Match 'for' keyword */
    Match("for");

    /* Match the name identifier */
    if (NameIdentifier(lookAheadToken) && (lookAheadTokenList[currentToken].token == tokens[TOKEN_STRING])) {
        /* Append the for loop syntax: for( int */
        py2PlusCode += "( int ";
        iterator = lookAheadToken;
        Match(lookAheadToken);  /* for( int var */
    }
    else
    {
        Error();
    }
    
    /* Match 'in' keyword */
    if( lookAheadToken == "in" ) {
		IncrementToken();
	}
    else
    {
		Error();
	}

    /* range keyword parsing */
    Range(iterator);

    /* Match the colon */
    if (lookAheadToken == ":") {
		IncrementToken();
	}
    else {
		Error();
	}
    
    /* Append the starting curly braces */
	py2PlusCode += "\n{\t";

    /* Continue parsing statements */
	Statements();

    /* Append the closing curly braces */
	py2PlusCode += "\n}\n";
}

/* Range Statement
 * <range> -> "range" "(" <number_identifier> <op_range> ")"
 * NOTES: Part of the For_Statement CFG
 */
void Parser::Range(string iterator) {
    if (lookAheadToken == "range") {
        /* Match range keyword */
        IncrementToken();

        /* Match opening parenthesis */
        if (lookAheadToken == "(") {
            IncrementToken();
        }
        else {
			Error();
		}

        /* Match the number identifier */
        if (NumberIdentifier(lookAheadToken) && lookAheadTokenList[currentToken + 1].tokenValue != ",") {

            py2PlusCode += " = 0; " + iterator + " < "; /* for( int var = 0; var < */

            Match(lookAheadToken);                      /* for( int var = 0; var < num*/

            py2PlusCode += "; " + iterator + "++";      /* for( int var = 0; var < num; var++ */
        }
        else if (NumberIdentifier(lookAheadToken) && lookAheadTokenList[currentToken + 1].tokenValue == ",") {

            py2PlusCode += " = " + lookAheadToken + "; " + iterator + " < ";   /* for( int var = num1; var < */ 

            IncrementToken();

            /* Parse optional range parameters (step parameter) */
            Op_Range(iterator);
		}
        else {
			Error();
        }

        /* Match closing parenthesis */
        if (lookAheadToken == ")") {
            Match(lookAheadToken);
        }
        else {
            Error();
        }
    }
    else {
        Error();
    }
}

/* Optional Range Statement
 * <op_range> -> "," <number_identifier> <op_range> | epsilon
 * NOTES: Part of the For_Statement CFG
 */
void Parser::Op_Range(string iterator) {
	/* If the next token is a comma */
    if (lookAheadToken == ",") {
		/* Match the comma */
        IncrementToken();

		/* Match the number identifier */
        if (NumberIdentifier(lookAheadToken)) {
			Match(lookAheadToken);          /* for( int var = num1; var < num2*/ 
		}
        else {
			Error();
		}

		/* Append the ending syntax */
        py2PlusCode += "; " + iterator;     /* for( int var = num1; var < num2; var*/ 

        if (lookAheadToken != ",") {
            py2PlusCode += "++";            /* for( int var = num1; var < num2; var++ */ 
        }
        else {

            if (lookAheadToken == ",") {
				IncrementToken();           
			}
            else {
				Error();
			}

            py2PlusCode += "+=";            /* for( int var = num1; var < num2; var +=*/

            /* Match the number identifier */
            if (NumberIdentifier(lookAheadToken)) {
                Match(lookAheadToken);      /* for( int var = num1; var < num2; var += num3*/
            }
            else {
                Error();
            }
		}
	}
    else {
        return;
	}
}


/************************************************************************************************
 ******************************** Mathematical Statements **************************************
************************************************************************************************/

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
    
    /* Check if the string starts with a 0 and has more than one character */
    if (num.size() > 1 && num[0] == '0')
        status = false;
    else
    {
        regex nameRegex("[-+]?[0-9]*.?[0-9]+([eE][-+]?[0-9]+)?");
        regex_match(num, nameRegex) ? status = true : status = false;
    }
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