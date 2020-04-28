//
// Created by zhaoyiping on 2020/4/6.
//

#include "test.h"
#include "json.hpp"
#include "parse.h"

using json = nlohmann::json;

#define RESOURCESPARSE (RESOURCES+(std::string)"parse/")
#define RESOURCESPARSEJSON (RESOURCESPARSE+(std::string)"json/")
#define RESOURCESPARSETOKEN (RESOURCESPARSE+(std::string)"token/")
#define JSONFILENAME(x) (x+(std::string)".json")
#define JSONLOCATION(x) (RESOURCESPARSEJSON+(std::string)(JSONFILENAME(x)))
#define TOKENFILENAME(x) (x+(std::string)".token")
#define TOKENLOCATION(x) (RESOURCESPARSETOKEN+(std::string)(TOKENFILENAME(x)))
#define SYMBOLTABLELOCATION(x) (RESOURCESPARSE+(std::string)"SymbolTable/"+x)

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
  Token getToken(std::istream &input) {
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
  Token getToken() {
    return getToken(this->input);
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

#define PARSESINGLETEST(filename, read)\
TEST_F(ParseTest,filename){\
  setTokenizer(#filename);\
  json j = getSTDJSON(#filename);\
  json j2 = toJSON(read(),true);\
  ASSERT_EQ(j.dump(), j2.dump()) << j.dump(1) << std::endl << j2.dump(1) << std::endl;\
  ASSERT_EQ(j, j2) << j << std::endl << j2 << std::endl;\
}

#define PARSEMULTITEST(filename, read)\
TEST_F(ParseTest,filename){\
  setTokenizer(#filename);\
  json jstd = getSTDJSON(#filename);\
  json j2;\
  auto ret = std::shared_ptr<AST>(nullptr);\
  auto tail = ret;\
  while (getToken().tokentype != EOFTOKEN) {\
    if (ret) {tail = getTail(tail); tail->next = read();}\
    else tail = ret = read();\
  }\
  j2 = toJSON(ret, true);\
  ASSERT_EQ(jstd.dump(), j2.dump()) << jstd.dump(1) << std::endl << j2.dump(1) << std::endl;\
  ASSERT_EQ(jstd, j2) << jstd << std::endl << j2 << std::endl;\
}

PARSESINGLETEST (aplusb, readExpression)
PARSESINGLETEST (constDeclare, readConstDeclare)
PARSESINGLETEST (compoundStatements1, readCompound_statements)
PARSESINGLETEST (compoundStatements2, readCompound_statements)
PARSESINGLETEST (compoundStatements3, readCompound_statements)
PARSESINGLETEST (compoundStatements4, readCompound_statements)
PARSESINGLETEST (compoundStatements5, readCompound_statements)
PARSESINGLETEST (main, readMain)
PARSESINGLETEST (funOrMain, readFunOrMain)
PARSESINGLETEST (statements, readStatements)
PARSESINGLETEST (program1, readProgram)

PARSEMULTITEST (valueDeclare, readInFuncValueDeclare)
PARSEMULTITEST (expression, readExpression)
PARSEMULTITEST (assign, readAssign)
PARSEMULTITEST (boolean, readBoolean)
PARSEMULTITEST (condition, readCondition)
PARSEMULTITEST (loop, readLoop)
PARSEMULTITEST (switch, readSwitch)
PARSEMULTITEST (callfun, readCall_fun)
PARSEMULTITEST (read, readRead)
PARSEMULTITEST (write, readWrite)
PARSEMULTITEST (return, readReturn)
PARSEMULTITEST (fun, readFun)
PARSEMULTITEST (statement, readStatement)

TEST_F(ParseTest, integer) {
  setTokenizer("integer");
  ASSERT_EQ("1", readInteger());
  ASSERT_EQ("-1", readInteger());
  ASSERT_EQ("1", readInteger());
  ASSERT_EQ(getToken().tokentype, EOFTOKEN);
}

TEST_F(ParseTest, cmpOp) {
  setTokenizer("cmpOp");
  ASSERT_EQ(LT, readCmpOp()->valueType);
  ASSERT_EQ(GT, readCmpOp()->valueType);
  ASSERT_EQ(LE, readCmpOp()->valueType);
  ASSERT_EQ(GE, readCmpOp()->valueType);
  ASSERT_EQ(NE, readCmpOp()->valueType);
  ASSERT_EQ(EQ, readCmpOp()->valueType);
  ASSERT_EQ(getToken().tokentype, EOFTOKEN);
}

TEST_F(ParseTest, addOp) {
  setTokenizer("addOp");
  ASSERT_EQ(ADD, readAddOp()->valueType);
  ASSERT_EQ(MINUS, readAddOp()->valueType);
  ASSERT_EQ(getToken().tokentype, EOFTOKEN);
}

TEST_F(ParseTest, mulOp) {
  setTokenizer("mulOp");
  ASSERT_EQ(MUL, readMulOp()->valueType);
  ASSERT_EQ(DIV, readMulOp()->valueType);
  ASSERT_EQ(getToken().tokentype, EOFTOKEN);
}

class ParseSymbolTest : public Parse, public ::testing::Test {
 protected:
  virtual void SetUp() {
    for (auto &x:AST::symbolTables)
      x.clear();
  }
  virtual void TearDown() {
    while (AST::symbolTables.size() > 1)AST::symbolTables.pop_back();
  }
  json getSTDJSON(std::string filename) {
    filename = JSONLOCATION(filename);
    std::ifstream input(filename);
    std::string s;
    getline(input, s);
    return json::parse(s);
  }
  std::shared_ptr<SymbolTable> global;
  std::shared_ptr<SymbolTable> getSymbolTable(std::string filename) {
    if (global)
      global->clear();
    filename = SYMBOLTABLELOCATION(filename);
    std::ifstream input(filename);
    std::string s;
    std::shared_ptr<SymbolTable> now;
    std::string nowName;
    while (input >> s) {
      if (s == "function" || s == "global") {
        if (s == "function") {
          input >> s;
        }
        if (nowName == "global")global = now;
        else if (now) {
          auto symbolFunction = std::static_pointer_cast<SymbolFunction>(global->get(nowName));
          symbolFunction->funSymbolTable = now;
        }
        nowName = s;
        now = std::make_shared<SymbolTable>(nowName);
      } else {
        std::string valueId = s;
        std::string type;
        bool isConst;
        input >> type;
        std::string tmp;
        input >> tmp;
        if (tmp == "true") isConst = true;
        else if (tmp == "false") isConst = false;
        else if (tmp == "INT" || tmp == "CHAR" || tmp == "VOID" || tmp == "end") {
          // function declare
          std::vector<Tokentype> args;
          while (tmp != "end") {
            args.emplace_back(toTokenType(tmp));
            input >> tmp;
          }
          now->insert<SymbolFunction>(valueId, args, toTokenType(type));
          continue;
        } else {
          int length = atoi(tmp.c_str());
          now->insert<SymbolArray>(valueId, toTokenType(type), length);
          continue;
        }
        int value;
        input >> value;
        now->insert<SymbolValue>(valueId, toTokenType(type), isConst, value);
      }
    }
    if (now) {
      if (nowName == "global")global = now;
      else {
        auto symbolFunction = std::static_pointer_cast<SymbolFunction>(global->get(nowName));
        symbolFunction->funSymbolTable = now;
      }
    }
    return global;
  }
};

TEST_F(ParseSymbolTest, init) {
  root = toAST(AST)(getSTDJSON("initSymbolTable"));
  initSymbolTable();
  auto stdSymbolTable = getSymbolTable("init");
  ASSERT_TRUE(AST::symbolTables[0] == *(stdSymbolTable.get()));
}

#define SymbolTestError(x) TEST_F(ParseSymbolTest,x){\
root=toAST(AST)(getSTDJSON(#x));\
ASSERT_THROW(initSymbolTable(),ParseError);\
}

#define SymbolTestNoError(x) TEST_F(ParseSymbolTest,x){\
root=toAST(AST)(getSTDJSON(#x));\
initSymbolTable();\
}

SymbolTestError(functionInFunction)
SymbolTestError(redefinition)
SymbolTestError(readConst)
SymbolTestError(assignConst)
SymbolTestError(callLessArg)
SymbolTestError(callMoreArg)
SymbolTestError(boolErrorMinus)
SymbolTestError(boolErrorAdd)
SymbolTestError(boolErrorMul)
SymbolTestError(boolErrorAssign)
SymbolTestError(boolErrorArray)
SymbolTestError(assignExpression)
SymbolTestError(notArray)
SymbolTestError(leafFunction)
SymbolTestError(readArray)
SymbolTestError(addString)
SymbolTestError(voidRet)
SymbolTestError(boolRet)
SymbolTestError(switchBool)

SymbolTestNoError(trueCondition)
SymbolTestNoError(trueLoop)
SymbolTestNoError(trueCall)
SymbolTestNoError(trueExpression)
SymbolTestNoError(trueLeaf)
SymbolTestNoError(trueRead)
SymbolTestNoError(trueWrite)
SymbolTestNoError(trueRet)
SymbolTestNoError(trueSwitch)

#undef PARSEMULTITEST
#undef PARSESINGLETEST
#undef TOKENLOCATION
#undef TOKENFILENAME
#undef JSONFILENAME
#undef JSONLOCATION
#undef RESOURCESPARSEJSON
#undef RESOURCESPARSE
