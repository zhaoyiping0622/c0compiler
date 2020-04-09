//
// Created by zhaoyiping on 2020/4/6.
//

#include "test.h"
#include "json.hpp"
#include "parse.h"

using json=nlohmann::json;

#define RESOURCESPARSE (RESOURCES+(std::string)"parse/")
#define RESOURCESPARSEJSON (RESOURCESPARSE+(std::string)"json/")
#define RESOURCESPARSETOKEN (RESOURCESPARSE+(std::string)"token/")
#define JSONFILENAME(x) (x+(std::string)".json")
#define JSONLOCATION(x) (RESOURCESPARSEJSON+(std::string)(JSONFILENAME(x)))
#define TOKENFILENAME(x) (x+(std::string)".token")
#define TOKENLOCATION(x) (RESOURCESPARSETOKEN+(std::string)(TOKENFILENAME(x)))

class FileTokenizer : public Tokenizer {
 public:
  std::ifstream input;
  std::unordered_map<std::string, Tokentype> ma;
  FileTokenizer(std::string filename) {
    input.open(TOKENLOCATION(filename));
    if (!input.is_open())std::cerr << TOKENLOCATION(filename) << " not found" << std::endl;
#define string2token(x) ma[#x]=x
    string2token(ID);
    string2token(ADD);
    string2token(MINUS);
    string2token(MUL);
    string2token(DIV);
    string2token(LT);
    string2token(LE);
    string2token(GT);
    string2token(GE);
    string2token(NE);
    string2token(EQ);
    string2token(NOT);
    string2token(AND);
    string2token(OR);
    string2token(SEMICOLON);
    string2token(LSBRACKETS);
    string2token(RSBRACKETS);
    string2token(LBBRACKETS);
    string2token(RBBRACKETS);
    string2token(LMBRACKETS);
    string2token(RMBRACKETS);
    string2token(ASSIGN);
    string2token(COMMA);
    string2token(COLON);
    string2token(CHARACTER);
    string2token(STRING);
    string2token(UNSIGNED);
    string2token(INT);
    string2token(CHAR);
    string2token(VOID);
    string2token(IF);
    string2token(WHILE);
    string2token(SWITCH);
    string2token(CASE);
    string2token(SCANF);
    string2token(PRINTF);
    string2token(RETURN);
    string2token(MAIN);
    string2token(DEFAULT);
    string2token(CONST);
    string2token(ELSE);
    string2token(EOFTOKEN);
#undef string2token
  }
  Token getToken() {
    auto f = [](std::string &s) { while (s.length() && isspace(s.back()))s.pop_back(); };
    std::string token;
    std::string s;
    if (input.eof())return Token(EOFTOKEN, "");
    getline(input, token);
    getline(input, s);
    f(s);
    f(token);
    if (s.length() == 0)return Token(EOFTOKEN, "");
    return Token(ma[token], s);
  }
};

class ParseTest : public Parse, public testing::Test {
 protected:
  void setTokenizer(std::string filename) {
    tokenizer = std::make_shared<FileTokenizer>(filename);
  }
  json getSTDJSON(std::string filename) {
    filename = JSONLOCATION(filename);
    std::ifstream input(filename);
    std::string s;
    getline(input, s);
    return json::parse(s);
  }
};

#define PARSETEST(filename, ...)\
TEST_F(ParseTest,filename){\
  setTokenizer(#filename);\
  json j = getSTDJSON(#filename);\
  __VA_ARGS__\
  ASSERT_EQ(j, j2) << j << std::endl << j2 << std::endl;\
}

PARSETEST(aplusb, json j2 = readExpression();)

PARSETEST(constDeclare, json j2 = readConstDeclare();)
PARSETEST
(valueDeclare, json j2;
    auto nextToken = getToken();
    auto ret = std::shared_ptr<AST>(nullptr);
    auto tail = ret;
    while (getToken().tokentype != EOFTOKEN) {
      if (ret) tail = getTail(tail), tail->next = readInFuncValueDeclare();
      else tail = ret = readInFuncValueDeclare();
    }
    j2 = ret;
)

#undef PARSETEST
#undef TOKENLOCATION
#undef TOKENFILENAME
#undef JSONFILENAME
#undef JSONLOCATION
#undef RESOURCESPARSEJSON
#undef RESOURCESPARSE
