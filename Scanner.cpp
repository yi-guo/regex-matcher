#include "Scanner.h"

Token::Token()
{
    type = 0;
    text = string();
}

Token::Token(string text, int type)
{
    this->text = text;
    this->type = type;
}

Token::~Token()
{
    ;
}

void Token::addChar(char character)
{
    text = text + character;
}

string Token::getText()
{
    return text;
}

void Token::setType(int type)
{
    this->type = type;
}

int Token::getType()
{
    return type;
}

Scanner::Scanner()
{
    index = 0;
    input = string();
    automaton = NULL;
}

Scanner::Scanner(string input, Graph* automaton)
{
    this->index = 0;
    this->input = input;
    this->automaton = automaton;
}

Scanner::Scanner(string input, string automaton)
{
    this->index = 0;
    this->input = input;
    setAutomaton(automaton);
}

Scanner::~Scanner()
{
    delete automaton;
}

void Scanner::setInput(string input)
{
    this->input = input;
}

string Scanner::getInput()
{
    return input;
}

void Scanner::setAutomaton(Graph* automaton)
{
    this->automaton = automaton;
}

void Scanner::setAutomaton(string filename)
{
    ifstream input(filename.c_str());
    automaton = new Graph();
    int count = 0;
    while (!input.eof()) {
        string line;
        getline(input, line);
        if (!line.empty()) {
            int state;
            line = line.substr(1, line.length() - 2);
            switch (count) {
                case 0:
                    while (!line.empty()) {
                        stringstream ss;
                        if (line.find(",") != string::npos) {
                            ss << line.substr(0, line.find(","));
                            line = line.substr(line.find(",") + 1);
                        }
                        else {
                            ss << line;
                            line = line.substr(line.length());
                        }
                        ss >> state;
                        automaton->insertVertex(new Vertex(state));
                    }
                    break;

                case 1:
                {
                    stringstream ss;
                    ss << line;
                    ss >> state;
                    automaton->setHead(automaton->findVertex(state));
                    break;
                }

                case 2:
                    while (!line.empty()) {
                        stringstream ss;
                        if (line.find(",") != string::npos) {
                            ss << line.substr(0, line.find(","));
                            line = line.substr(line.find(",") + 1);
                        }
                        else {
                            ss << line;
                            line = line.substr(line.length());
                        }
                        ss >> state;
                        automaton->findVertex(state)->setFinal(true);
                    }
                    break;

                case 3:
                    while (!line.empty()) {
                        string block = line.substr(line.find("(") + 1, line.find(")"));
                        int head, tail;
                        stringstream ss;
                        ss << block.substr(0, block.find(","));
                        ss >> head;
                        ss.clear();
                        block = block.substr(block.find(",") + 1);
                        string text = block.substr(0, block.find(","));
                        if (text == "left_paren")
                            text = "(";
                        else if (text == "right_paren")
                            text = ")";
                        block = block.substr(block.find(",") + 1);
                        ss << block.substr(0, block.find(")"));
                        ss >> tail;
                        Edge* edge = new Edge(automaton->findVertex(head), automaton->findVertex(tail), text);
                        automaton->insertEdge(edge);
                        edge->getHead()->insertEdge(edge);
                        line = line.substr(line.find(")") + 1);
                    }
                    break;
            }
            count++;
        }
    }
}

Graph* Scanner::getAutomaton()
{
    return automaton;
}

Token* Scanner::readNext()
{
    return next();
}

Token* Scanner::peekNext()
{
    unsigned int index = this->index;
    Token* token = next();
    this->index = index;
    return token;
}

Token* Scanner::next()
{
    if (index < input.length()) {
        Token* token = new Token();
        Vertex* currentState = automaton->getHead();
        while (index < input.length()) {

            char nextCharacter = input[index];
            Vertex* nextState = getNextState(currentState, nextCharacter);

            if (!currentState->isFinal() && nextState == NULL) {
                throw new string(token->getText() + nextCharacter + "\t(The system cannot identify the regular expression specified.)");
            }
            else if (currentState->isFinal() && nextState == NULL) {
                token->setType(currentState->getState());
                return token;
            }
            else {
                currentState = nextState;
                token->addChar(input[index]);
                index++;
            }
        }
        if (!currentState->isFinal() && !token->getText().empty())
            throw new string(token->getText() + "\t(The system cannot identify the regular expression specified.)");
        token->setType(currentState->getState());
        return token;
    }
    else
        return new Token("$$", 16);
}

Vertex* Scanner::getNextState(Vertex* currentState, char nextCharacter)
{
    Vertex* nextState = NULL;
    for (unsigned int i = 0; i < currentState->getEdges().size(); i++) {
        if (match(currentState->getEdges()[i]->getText(), nextCharacter))
            nextState = currentState->getEdges()[i]->getTail();
    }
    return nextState;
}

bool Scanner::match(string text, char character)
{
    if (text == "ordinary_char")
        return isOrdinary(character);
    else if (text == "special_char")
        return isSpecial(character);
    else if (text[0] == '.' && text.length() == 1)
        return true;
    else if (text[0] == '\\' && text.length() > 1) {
        switch (text[1]) {
            case 'a':
                return isalpha(character);
            case 'A':
                return !isalpha(character);
            case 'd':
                return isdigit(character);
            case 'D':
                return !isdigit(character);
            case 'w':
                if (character == '_')
                    return true;
                return isalnum(character);
            case 'W':
                if (character == '_')
                    return false;
                return !isalnum(character);
            case 's':
                if (character == ' ' || character == '\t' || character == '\n')
                    return true;
                else
                    return false;
            case 'S':
                if (character == ' ' || character == '\t' || character == '\n')
                    return false;
                else
                    return true;
            case 't':
                if (character == '\t')
                    return true;
                else
                    return false;
            case 'n':
                if (character == '\n')
                    return true;
                else
                    return false;
            default:
                if (isSpecial(character))
                    return true;
                else
                    return false;
        }
    }
    else if (text[0] == '[' && text.length() > 1) {
        set<char> charSet = getCharSet(text);
        if (charSet.find(character) != charSet.end())
            return true;
        else
            return false;
    }
    else
        return text[0] == character;
}
