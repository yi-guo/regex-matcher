#include "Match.h"

string tolower(string str)
{
    for (unsigned int i = 0; i < str.size(); i++)
        str[i] = tolower(str[i]);
    return str;
}

string toupper(string str)
{
    for (unsigned int i = 0; i < str.size(); i++)
        str[i] = toupper(str[i]);
    return str;
}

void execute_through_command_prompt(int argc, char* argv[])
{
    cout << "\n==================================================" << endl;
    if (argc > 3) {
        string command = tolower(argv[1]);
        string S = argv[2];
        string E = argv[3];
        cout << toupper(command) << ": S";

        if (command.find("replace") != string::npos && argc > 4)
            cout << "1 = " << S << "  E = " << E << "  S2 = " << argv[4] << endl;
        else
            cout << " = " << S << "  E = " << E << endl;
        cout << "RESULT: ";

        if (command == "matches") {
            if (matches(S, E))
                cout << "true" << endl;
            else
                cout << "false" << endl;
        }
        else if (command == "firstmatch")
            cout << firstMatch(S, E).toString();
        else if (command == "replacefirst") {
            if (argc > 4)
                cout << replaceFirst(S, E, argv[4]) << endl;
            else
                cout << "Error - invalid command!" << endl;
        }
        else if (command == "replaceall") {
            if (argc > 4)
                cout << replaceAll(S, E, argv[4]) << endl;
            else
                cout << "Error - invalid command!" << endl;
        }
        else if (command == "findfirst")
            cout << findFirst(S, E) << endl;
        else if (command == "findall") {
            vector<string> strs = findAll(S, E);
            cout << "[";
            for (unsigned int i = 0; i < strs.size(); i++) {
                cout << "\"" << strs[i] << "\"";
                if (i != strs.size() - 1)
                    cout << ", ";
            }
            cout << "]" << endl;
        }
    }
    else
        cout << "Error - invalid command!" << endl;
    cout << "==================================================" << endl;
}

void execute_through_text_file(int argc, char* argv[])
{
    ifstream input(argv[1]);
    while (!input.eof()) {
        string line;
        getline(input, line);
        if (!line.empty()) {
            cout << "\n==================================================" << endl;

            string command = tolower(line.substr(0, line.find("\t")));
            line = line.substr(line.find("\""));
            cout << toupper(command) << ": ";

            string S = line.substr(1, line.find("\t") - 2);
            line = line.substr(line.find("\"", S.size() + 2));

            string E;
            if (command.find("replace") != string::npos) {
                E = line.substr(1, line.find("\t") - 2);
                line = line.substr(line.find("\"", E.size() + 2));
                cout << "S1 = " << S << "  E = " << E << "  S2 = " << line.substr(1, line.length() - 2) << endl;
            }
            else {
                E = line.substr(1, line.length() - 2);
                cout << "S = " << S << "  E = " << E << endl;
            }

            cout << "RESULT: ";
            if (command == "matches") {
                if (matches(S, E))
                    cout << "true" << endl;
                else
                    cout << "false" << endl;
            }
            else if (command == "firstmatch")
                cout << firstMatch(S, E).toString();
            else if (command == "replacefirst")
                cout << replaceFirst(S, E, line.substr(1, line.length() - 2)) << endl;
            else if (command == "replaceall")
                cout << replaceAll(S, E, line.substr(1, line.length() - 2)) << endl;
            else if (command == "findfirst")
                cout << findFirst(S, E) << endl;
            else if (command == "findall") {
                vector<string> strs = findAll(S, E);
                cout << "[";
                for (unsigned int i = 0; i < strs.size(); i++) {
                    cout << "\"" << strs[i] << "\"";
                    if (i != strs.size() - 1)
                        cout << ", ";
                }
                cout << "]" << endl;
            }
            cout << "==================================================\n" << endl;
        }
    }
}

int main(int argc, char* argv[])
{
//  execute_through_command_prompt(argc, argv);
    execute_through_text_file(argc, argv);
    return 0;
}
