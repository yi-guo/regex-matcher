#include "Match.h"

Match::Match()
{
    flag = false;
    begin = end = -1;
}

string Match::toString()
{
    stringstream output;
    output << "[Flag: ";
    if (flag)
        output << "true";
    else
        output << "false";
    output << ", Begin: " << begin << ", End: " << end << "]" << endl;
    return output.str();
}

bool matches(string S, string E)
{
    if (S.empty() && E.empty())
        return true;
    bool match = true;
    Scanner* scanner = new Scanner(S, getDFA(E));
    try {
        Token* next = scanner->readNext();
        if (next->getText() != S)
            match = false;
        delete next;
    }
    catch (string* message) {
        delete message;
        match = false;
    }
    delete scanner;
    return match;
}

Match firstMatch(string S, string E)
{
    Match M;
    for (unsigned int i = 0; i < S.size(); i++) {
        for (unsigned int j = S.size() - i; j > 0; j--) {
            if (matches(S.substr(i, j), E)) {
                M.flag = true;
                M.begin = i;
                M.end = i + j - 1;
                return M;
            }
        }
    }
    return M;
}

string replaceFirst(string S1, string E, string S2)
{
    Match M = firstMatch(S1, E);
    if (M.flag)
        return S1.replace(M.begin, M.end - M.begin + 1, S2);
    else
        return S1;
}

string replaceAll(string S1, string E, string S2)
{
    Match M = firstMatch(S1, E);
    if (M.flag)
        return S1.substr(0, M.begin) + S2 + replaceAll(S1.substr(M.end + 1), E, S2);
    else
        return S1;
}

string findFirst(string S, string E)
{
    Match M = firstMatch(S, E);
    if (M.flag)
        return S.substr(M.begin, M.end - M.begin + 1);
    else
        return string();
}

vector<string> findAll(string S, string E)
{
    vector<string> strs;
    Match M = firstMatch(S, E);
    if (M.flag && M.end < (signed) S.size()) {
        strs.push_back(S.substr(M.begin, M.end - M.begin + 1));
        vector<string> substrs = findAll(S.substr(M.end + 1), E);
        strs.insert(strs.end(), substrs.begin(), substrs.end());
        return strs;
    }
    else
        return strs;
}
