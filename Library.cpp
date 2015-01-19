#include "Library.h"

const string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const string digit = "0123456789";
const string ordi_chars = alpha + digit + ";<=>@_`{}~!#%&',-/: \t\n\"";
const string spec_chars = "\\.[]^$*+?()|";

bool isOrdinary(char character)
{
    return ordi_chars.find(character) != string::npos;
}

bool isSpecial(char character)
{
    return spec_chars.find(character) != string ::npos;
}

set<char> getAlpha()
{
    set<char> chars;
    for (unsigned int i = 0; i < alpha.length(); i++)
        chars.insert(alpha[i]);
    return chars;
}

set<char> getDigit()
{
    set<char> chars;
    for (unsigned int i = 0; i < digit.length(); i++)
        chars.insert(digit[i]);
    return chars;
}

set<char> getAlnum()
{
    set<char> chars;
    chars.insert(getAlpha().begin(), getAlpha().end());
    chars.insert(getDigit().begin(), getDigit().end());
    return chars;
}

set<char> getUniverse()
{
    set<char> chars;
    string universe = ordi_chars + spec_chars;
    for (unsigned int i = 0; i < universe.length(); i++)
        chars.insert(universe[i]);
    return chars;
}

set<char> getComplement(set<char> chars)
{
    set<char> complement;
    set<char> universe = getUniverse();
    for (set<char>::iterator i = universe.begin(); i != universe.end(); i++) {
        char character = *i;
        if (chars.find(character) == chars.end())
            complement.insert(character);
    }
    return complement;
}

set<char> getCharSet(string charSet)
{
    int index = 1;
    set<char> chars;
    bool isNegative = false;
    while (index < (signed) charSet.length()) {
        switch (charSet[index]) {
            case '^':
                isNegative = true;
                break;

            case '.':
                chars = getUniverse();
                break;

            case '\\':
            {
                set<char> subChars;
                switch (charSet[index + 1]) {
                    case 'a':
                        subChars = getAlpha();
                        break;

                    case 'A':
                        subChars = getComplement(getAlpha());
                        break;

                    case 'd':
                        subChars = getDigit();
                        break;

                    case 'D':
                        subChars = getComplement(getDigit());
                        break;

                    case 'w':
                        subChars = getAlnum();
                        subChars.insert('_');
                        break;

                    case 'W':
                        subChars = getAlnum();
                        subChars.insert('_');
                        subChars = getComplement(subChars);
                        break;

                    case 's':
                        subChars.insert(' ');
                        subChars.insert('\t');
                        subChars.insert('\n');
                        break;

                    case 'S':
                        subChars.insert(' ');
                        subChars.insert('\t');
                        subChars.insert('\n');
                        subChars = getComplement(subChars);
                        break;

                    case 't':
                        subChars.insert('\t');
                        break;

                    case 'n':
                        subChars.insert('\n');
                        break;

                    default:
                        subChars.insert(charSet[index + 1]);
                }
                chars.insert(subChars.begin(), subChars.end());
                index++;
                break;
            }

            case '[':
            {
                int end;
                int count = 1;
                for (unsigned int i = index + 1; i < charSet.length(); i++) {
                    switch (charSet[i]) {
                        case '[':
                            count++;
                            break;
                        case ']':
                            count--;
                            break;
                        case '\\':
                            i++;
                            break;
                    }
                    if (count == 0)
                        end = i;
                }
                set<char> subChars = getCharSet(charSet.substr(index, end - index + 1));
                chars.insert(subChars.begin(), subChars.end());
                index = end;
                break;
            }

            case ']':
                if (!isNegative)
                    return chars;
                else
                    return getComplement(chars);

            default:
                chars.insert(charSet[index]);
        }
        index++;
    }
    if (!isNegative)
        return chars;
    else
        return getComplement(chars);
}
