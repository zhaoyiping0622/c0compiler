#include "base.h"
#include "iostream"
#include "cstdlib"
void error(const char *errorMessage) {
  throw BaseError(errorMessage);
}
void unreachable() {
  error("unreachable");
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
        case 'f':return 0x0c;
        case 'n':return 10;
        case 'r':return 13;
        case 't':return 9;
        case 'v':return 0x0b;
        case '\'':return '\'';
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
