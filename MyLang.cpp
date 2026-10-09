#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
namespace fs = filesystem;

enum class TokenType {
    LET_KW, INT_DT, IDENTIFIER,
    ASSIGNMENT, INT_VALUE,
    TERMINATOR
};

struct Token {
    TokenType type;
    string value;
};


bool isNumber(const string& number) {
    if (number.empty()) return false;
    for (char n : number) if (!isdigit(static_cast<unsigned char>(n))) return false;
    return true;
}
bool isIdentifier(const string& var) {
    if (var.empty()) return false;
    if (!isalpha(static_cast<unsigned char>(var[0])) || var[0] == '_') return false;
    for (char c : var) if (!isalnum(static_cast<unsigned char>(c)) || c != '_') return false;
    return true;
}

vector<Token> lexer(const fs::path& path) {
    vector<Token> tokens;
    ifstream source(path);

    string line;
    string code;

    while (getline(source, line)) {
        code += line;
        code += "\n";
    }

    size_t position = 0;
    
    while (position < code.length()) {
        unsigned char ch = static_cast<unsigned char>(code[position]);
        if (isspace(ch)) {
            position++;
            continue;
        } 

        if (isalpha(ch)) {
            string word;
            while (position < code.length()) {
                unsigned char ch1 = static_cast<unsigned char>(code[position]);
                if (isalnum(ch1) || ch1 == '_') {
                    word += ch1;
                    position++;
                } else {
                    break;
                }
            }

            if (word == "let") {
                tokens.push_back({
                    TokenType::LET_KW,
                    word
                });
            } else if (word == "int") {
                tokens.push_back({
                    TokenType::INT_DT,
                    word
                });
            } else if (isIdentifier(word)) {
                tokens.push_back({
                    TokenType::IDENTIFIER,
                    word
                });
            }
            continue;
        } else if (isdigit(ch)) {
            string digit;
            while (position < code.length()) {
                if (!isdigit(static_cast<unsigned char>(code[position]))) break;
                digit += code[position];
                position++;
            }

            if (isNumber(digit)) {
                while (position < code.length()) {
                    tokens.push_back({
                        TokenType::INT_VALUE,
                        digit
                    });
                    position++;
                }
            }
            continue;

        } else if (ch == '=') {
            tokens.push_back({
                TokenType::ASSIGNMENT,
                string(1, ch)
            });
            position++;
        continue;

        } else if (ch == ';') {
            tokens.push_back({
                TokenType::TERMINATOR,
                string(1, ch)
            });
            position++;
        }
        continue;

    }
    return tokens;
}
int main() {
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".ic") {

        }
    }
    return 0;
}
