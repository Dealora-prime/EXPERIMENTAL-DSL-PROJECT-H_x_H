#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;
namespace fs = filesystem;


enum class TokenType {
    LET_KEYWORD, INT_DATATYPE,
    IDENTIFIER, ASSIGNMNT, INT_VALUE,
    STATEMENT_TERMINATOR
};

struct Token {
    TokenType type;
    string value;
};

bool isIdentifier(const string& var) {
    if (var.empty()) return false;
    for (char c : var) if (!isalpha(c)) return false;
    return true;
}

bool isNumber(const string& number) {
    if (number.empty()) return false;
    for (char n : number) if (!isdigit(n)) return false;
    return true;
}

void lexer(const fs::path& path) {
    vector<Token> tokens;
    ifstream source(path);

    string line;
    string code;

    while (getline(source, line)) {
        code += line;
        code += "\n";
    }

    size_t position = 0;
    string word;
    string digit;

    while (position < code.length()) {
        if (isalpha(code[position])) {
            word += code[position];

        } else if (isdigit(code[position])) {
            digit += code[position];

        } else if (code[position] == ';') {
            if (!digit.empty()) {
                if (isNumber(digit)) {
                    tokens.push_back({
                        TokenType::INT_VALUE, 
                        digit
                    });
                }
            }
            tokens.push_back({
                TokenType::STATEMENT_TERMINATOR,
                string(1, code[position])
            });

        } else if (code[position] == ' ') {
            if (!word.empty()) {
                if (word == "let") {
                    tokens.push_back({
                        TokenType::LET_KEYWORD,
                        word
                    });

                } else if (word == "int") {
                    tokens.push_back({
                        TokenType::INT_DATATYPE,
                        word
                    });
                
                } else if (isIdentifier(word)) {
                    tokens.push_back({
                        TokenType::IDENTIFIER,
                        word
                    });
                }

                word.clear();
            } 
        } else if (code[position] == '=') {
            if (!word.empty()) {
                if (word =="let") {
                    tokens.push_back({
                        TokenType::LET_KEYWORD, 
                        word
                    });

                } else if (word == "int") {
                    tokens.push_back({
                        TokenType::INT_DATATYPE,
                        word
                    });
 
                } else if (isIdentifier(word)) {
                    tokens.push_back({
                        TokenType::IDENTIFIER,
                        word
                    });
                }

                word.clear();
            }

            tokens.push_back({
                TokenType::ASSIGNMNT,
                string(1, code[position])
            });
        } 

        position++;
    }
}
int main() {
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".ic") {
            lexer(entry.path());
        }
    }
    return 0;
}
