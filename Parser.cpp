#include "Parser.h"

Parser::Parser()
{
    scanner = NULL;
}

Parser::Parser(Scanner* scanner)
{
    this->scanner = scanner;
}

Parser::~Parser()
{
    delete scanner;
}

void Parser::setScanner(Scanner* scanner)
{
    this->scanner = scanner;
}

Scanner* Parser::getScanner()
{
    return scanner;
}

vector<Token*> Parser::getTokens()
{
    program();
    return tokens;
}

void Parser::match(int expected)
{
    Token* next = scanner->readNext();
    if (next->getType() == expected)
        insertToken(next);
    else
        throw new string("Syntax Error!");
}

void Parser::program()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        case 15:
        case 16:
            regex();
            break;
        case 14:
            match(14);
            regex();
            break;
        default:
            throw new string("Syntax Error!");
    }
    delete next;
    next = scanner->peekNext();
    if (next->getType() == 15)
        match(15);
    else
        match(16);
    delete next;
}

void Parser::regex()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
            term();
            term_tail();
            break;
        case 15:
        case 16:
            break;
        default:
            throw new string("Syntax Error!");
    }
    delete next;
}

void Parser::term_tail()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 13:
            match(13);
            regex();
            break;
        case 9:
        case 15:
        case 16:
            break;
        default:
            throw new string("Syntax Error!");
    }
    delete next;
}

void Parser::term()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
            factor();
            factor_tail();
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::factor_tail()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
            factor();
            factor_tail();
            break;
        case 9:
        case 13:
        case 15:
        case 16:
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::factor()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
            base();
            base_tail();
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::base_tail()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 10:
        case 11:
        case 12:
            rep_op();
            base_tail();
            break;
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        case 9:
        case 13:
        case 15:
        case 16:
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::base()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
            character();
            break;
        case 6:
            charset();
            break;
        case 8:
            match(8);
            regex();
            match(9);
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::rep_op()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 10:
            match(10);
            break;
        case 11:
            match(11);
            break;
        case 12:
            match(12);
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::charset()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 6:
            match(6);
            delete next;
            next = scanner->peekNext();
            switch(next->getType()) {
                case 14:
                    match(14);
                    break;
            }
            text();
            text_tail();
            match(7);
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::text_tail()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
        case 6:
            text();
            text_tail();
            break;
        case 7:
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::text()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
        case 3:
        case 5:
            character();
            break;
        case 6:
            charset();
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::character()
{
    Token* next = scanner->peekNext();
    switch (next->getType()) {
        case 2:
            match(2);
            break;
        case 3:
            match(3);
            break;
        case 5:
            match(5);
            break;
        default:
            throw new string ("Syntax Error!");
    }
    delete next;
}

void Parser::insertToken(Token* token)
{
    static int count = 0;
    if (count == 0) {
        if (token->getText() == "[")
            count++;
        tokens.push_back(token);
    } else {
        switch (token->getText()[0]) {
            case '\\':
                tokens[tokens.size() - 1]->addChar(token->getText()[0]);
                tokens[tokens.size() - 1]->addChar(token->getText()[1]);
                break;
            case '[':
                tokens[tokens.size() - 1]->addChar(token->getText()[0]);
                count++;
                break;
            case ']':
                count--;
                // No break is needed
            default:
                tokens[tokens.size() - 1]->addChar(token->getText()[0]);
        }
        delete token;
    }
}
