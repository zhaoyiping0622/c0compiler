#include "base.h"
#include "iostream"
#include "cstdlib"
void error(const char *errorMessage) {
  throw BaseError(errorMessage);
}
void BaseError::baseOperation() {}
BaseError::BaseError() {}
BaseError::BaseError(std::string errorMessage) : errorMessage(errorMessage) {

}
BaseError::~BaseError() {}
int realValue(std::string s) {
  if (s[0] == '\'') {
    // char
    if (s[1] == '\\') {
      switch (s[2]) {
        case '0':return 0;
        case 'a':return 7;
        case 'b':return 8;
        case 't':return 9;
        case 'n':return 10;
        case 'r':return 13;
        case '\\':return '\\';
        default:;
          throw BaseError("only 0 a b t n r can follow \\");
      }
    } else {
      return s[1];
    }
  } else {
    return std::atoi(s.c_str());
  }
};
