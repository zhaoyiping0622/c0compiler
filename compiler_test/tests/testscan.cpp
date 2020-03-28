//
// Created by zhaoyiping on 2020/3/25.
//

#include "scan.h"
#include "gtest/gtest.h"
#define RESOURCES ((std::string)"resources/scan/")
#define FILELOCATION(x) (RESOURCES+(std::string)x)

class ScanTest : public testing::Test {
 protected:
  std::vector<Token> useroutput;
  std::unordered_map<Tokentype, std::string> ma;
  virtual void SetUp() {
#define Tokentype2string(x) ma[x]=#x
    Tokentype2string(ID);
    Tokentype2string(ADD);
    Tokentype2string(MINUS);
    Tokentype2string(MUL);
    Tokentype2string(DIV);
    Tokentype2string(LT);
    Tokentype2string(LE);
    Tokentype2string(GT);
    Tokentype2string(GE);
    Tokentype2string(NE);
    Tokentype2string(EQ);
    Tokentype2string(SEMICOLON);
    Tokentype2string(LSBRACKETS);
    Tokentype2string(RSBRACKETS);
    Tokentype2string(LBBRACKETS);
    Tokentype2string(RBBRACKETS);
    Tokentype2string(LMBRACKETS);
    Tokentype2string(RMBRACKETS);
    Tokentype2string(ASSIGN);
    Tokentype2string(COMMA);
    Tokentype2string(CHARACTER);
    Tokentype2string(STRING);
    Tokentype2string(UNSIGNED);
    Tokentype2string(INT);
    Tokentype2string(CHAR);
    Tokentype2string(VOID);
    Tokentype2string(IF);
    Tokentype2string(WHILE);
    Tokentype2string(SWITCH);
    Tokentype2string(CASE);
    Tokentype2string(SCANF);
    Tokentype2string(PRINTF);
    Tokentype2string(RETURN);
    Tokentype2string(MAIN);
    Tokentype2string(DEFAULT);
    Tokentype2string(CONST);
    Tokentype2string(ELSE);
    Tokentype2string(EOFTOKEN);
#undef Tokentype2string
  }
  void run(std::string inputfilename, std::string outputfilename) {
    std::string filestring;
    std::ifstream f;
    f.open(inputfilename);
    ASSERT_TRUE(f.is_open());
    run(f);
    f.close();
    f.open(outputfilename);
    ASSERT_TRUE(f.is_open());
    judge(f);
    f.close();
  }
  void run(std::istream &input) {
    Scan scan(input);
    while (true) {
      Token token = scan.getToken();
      if (token.tokentype == EOFTOKEN)
        break;
      useroutput.push_back(token);
    }
  }
  void run(std::string inputstring) {
    std::istringstream input(inputstring);
    run(input);
  }
  void judge(std::istream &input) {
    for (auto x:useroutput) {
      std::string s;
      getline(input, s);
      while (s.size() && isspace(s.back()))s.pop_back();
      ASSERT_EQ(s, ma[x.tokentype]);
      getline(input, s);
      while (s.size() && isspace(s.back()))s.pop_back();
      ASSERT_EQ(s, x.s);
    }
    std::string s;
    input >> s;
    ASSERT_TRUE(input.eof());
  }

};
class ScanDeathTest : public ScanTest {
 protected:
  void runDeath(std::string inputfilename, bool multi = false, std::string errorMessage = "") {
    std::string filestring;
    std::ifstream f;
    f.open(inputfilename);
    ASSERT_TRUE(f.is_open());
    if (multi) {
      std::string s;
      while (true) {
        getline(f, s);
        if (f.eof())
          break;
        std::istringstream input(s);
        while (s.size() && isspace(s.back()))s.pop_back();
        ASSERT_DEATH(run(input), errorMessage);
      }
    } else {
      ASSERT_DEATH(run(f), errorMessage);
    }
  }
};
#define FILETEST(file) \
TEST_F(ScanTest,file){\
  run(FILELOCATION(#file)+(std::string)".in",FILELOCATION(#file)+(std::string)".out");\
}
FILETEST(string);
FILETEST(single);
FILETEST(keywords);
FILETEST(cmpOp);
FILETEST(char);
FILETEST(number);
FILETEST(id);
#undef FILETEST
#define FILETEST(file, multi)\
TEST_F(ScanDeathTest, file) {\
runDeath(FILELOCATION(#file)+(std::string)"Death.in",multi);\
}
#define FILETESTWITHERROR(file, multi, errorMessage)\
TEST_F(ScanDeathTest, file) {\
runDeath(FILELOCATION(#file)+(std::string)"Death.in",multi,(std::string)errorMessage);\
}
FILETEST(not, false);
FILETEST(char, true);
FILETEST(string, true);
FILETESTWITHERROR(stringMultiLine, false, ".*\"asdf\\\\n.*");
#undef FILETEST
#undef RESOURCES
#undef FILELOCATION
#undef FILETESTWITHERROR
