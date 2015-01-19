#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"

class Parser
{
public:
    Parser();
    Parser(Scanner*);
    ~Parser();
    void            setScanner(Scanner*);
    Scanner*        getScanner();
    vector<Token*>  getTokens();

private:
    Scanner*        scanner;
    vector<Token*>  tokens;

    void match(int);
    void program();
    void regex();
    void term_tail();
    void term();
    void factor_tail();
    void factor();
    void base_tail();
    void base();
    void charset();
    void text_tail();
    void text();
    void character();
    void rep_op();
    void insertToken(Token*);
};

#endif  // PARSER_H
