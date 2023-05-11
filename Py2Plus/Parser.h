#pragma once


class Parser
{
private:
    string py2PlusCode;
    string lookAheadToken;
    vector<TokenInfo> lookAheadTokenList;
    int currentToken;

    void IncrementToken();          /* Increment the token */
    void Match(string token);                   /* Match the look-ahead token with the current token */

    /* Statement -> If_Statement | For_Statement | While_Statement | DoWhile_Statement | Assignment_Statement */
    /* Statements that we have */
    void Statement();
    void Statements();
    void If_Statement();
    void Elif_Statement();
    void Else_Statement();
    void For_Statement();
    void While_Statement();
    void DoWhile_Statement();

    /* Expression and mathematical statements */
    void Expression_Statement();
    void Term_Statement();
    void Factor_Statement();
    void Assignment_Statement();
    
    /* Names, numbers and operators */
    bool NameIdentifier(string id);          /* Identifier -> A, B, C,..., Z */
    void NumberIdentifier();        /* Number -> 0, 1, 2,..., 9 */
    void Operator();                /* Operator -> +, -, *, /, % */
    void ConditionalOperator();     /* Conditional Operator -> <, >, <=, >=, ==, != */
    //void LogicalOperator();         /* Logical Operator -> and, or, not */

    /* Error handling */
    void Error();

public:
    Parser(vector<TokenInfo> tokenList);
    string Parse();
};