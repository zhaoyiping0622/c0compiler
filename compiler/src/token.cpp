//
// Created by zhaoyiping on 2020/3/31.
//

#include "token.h"
Token::Token() : tokentype(UNDEFINED), s("") {}
Token::Token(Tokentype tokentype, std::string s) : tokentype(tokentype), s(s) {}
