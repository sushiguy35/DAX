#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

// read arguments
string program_filepath = "";

int main(int argc, char* argv[]) {
    if (argc > 1) {
        program_filepath = argv[1];
    } else {
        cout << "Please provide a program file path." << endl;
        return 1;
    }

    //     Tokenize Program

    // read file lines
    vector<string> program_lines;
    ifstream program_file(program_filepath);
    string line;
    while (getline(program_file, line)) {
        program_lines.push_back(line);
    }

    vector<string> program;
    int token_counter = 0;
    unordered_map<string, int> label_tracker;
    for (string line : program_lines) {
        vector<string> parts;
        string part;
        for (char c : line) {
            if (c == ' ') {
                parts.push_back(part);
                part = "";
            } else {
                part += c;
            }
        }
        parts.push_back(part);
        string opcode = parts[0];

        // check for empty line
        if (opcode == "") {
            continue;
        }

        // check if its a label
        if (opcode.back() == ':') {
            label_tracker[opcode.substr(0, opcode.size()-1)] = token_counter;
            continue;
        }

        // store opcode token
        program.push_back(opcode);
        token_counter++;

        // handle each opcode
        if (opcode == "PUSH") {
            // expecting a number
            int number = stoi(parts[1]);
            program.push_back(to_string(number));
            token_counter++;
        } else if (opcode == "PRINT") {
            // parse string literal
            string string_literal = "";
            for (int i = 1; i < parts.size(); i++) {
                string_literal += parts[i];
                if (i != parts.size()-1) {
                    string_literal += " ";
                }
            }
            string_literal = string_literal.substr(1, string_literal.size()-2);
            program.push_back(string_literal);
            token_counter++;
        } else if (opcode == "JUMP.EQ.0") {
            // read label
            string label = parts[1];
            program.push_back(label);
            token_counter++;
        } else if (opcode == "JUMP.GT.0") {
            // read label
            string label = parts[1];
            program.push_back(label);
            token_counter++;
        }
    }

    //     Interpret Program

    class Stack {
    private:
        vector<int> buf;
        int sp;
    public:
        Stack(int size) {
            buf = vector<int>(size, 0);
            sp = -1;
        }

        void push(int number) {
            sp++;
            buf[sp] = number;
        }

        int pop() {
            int number = buf[sp];
            sp--;
            return number;
        }

        int top() {
            return buf[sp];
        }
    };

    int pc = 0;
    Stack stack(256);

    while (pc < program.size() && program[pc] != "HALT") {
        string opcode = program[pc];
        pc++;

        if (opcode == "PUSH") {
            int number = stoi(program[pc]);
            pc++;

            stack.push(number);
        } else if (opcode == "POP") {
            stack.pop();
        } else if (opcode == "ADD") {
            int a = stack.pop();
            int b = stack.pop();
            stack.push(a+b);
        } else if (opcode == "SUB") {
            int a = stack.pop();
            int b = stack.pop();
            stack.push(b-a);
        } else if (opcode == "PRINT") {
            string string_literal = program[pc];
            pc++;
            cout << string_literal << endl;
        } else if (opcode == "READ") {
            int number;
            cin >> number;
            stack.push(number);
        } else if (opcode == "JUMP.EQ.0") {
            int number = stack.top();
            if (number == 0) {
                pc = label_tracker[program[pc]];
            } else {
                pc++;
            }
        } else if (opcode == "JUMP.GT.0") {
            int number = stack.top();
            if (number > 0) {
                pc = label_tracker[program[pc]];
            } else {
                pc++;
            }
        }
    }

    return 0;
}