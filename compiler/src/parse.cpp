//
// Created by zhaoyiping on 2020/3/31.
//

#include "parse.h"
void parseError(const char *errorMessage) {
  std::string em = "Parse Error: " + (std::string)errorMessage;
  error(em.c_str());
}
Parse::Parse(std::shared_ptr<Tokenizer> tokenlizer) : tokenizer(tokenlizer) {
  nextToken = tokenlizer->getToken();
}
void Parse::init() {}
void Parse::parse() {}
Token Parse::getToken() {
  if (nextToken.tokentype == UNDEFINED)nextToken = tokenizer->getToken();
  return nextToken;
}
Token Parse::popToken() {
  auto ret = nextToken;
  nextToken = tokenizer->getToken();
  return ret;
}
void Parse::parseErrorUnexpectedToken(const char *s) {
  parseError(((std::string)"expected " + s + " read " + toString(getToken().tokentype)).c_str());
}
// DONE
std::shared_ptr<ASTLeaf> Parse::readAddOp() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case ADD:
    case MINUS:popToken();
      return std::make_shared<ASTLeaf>(nextToken.s, nextToken.tokentype);
    default:parseErrorUnexpectedToken("AddOp");
  }
}
// DONE
std::shared_ptr<ASTLeaf> Parse::readMulOp() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case MUL:
    case DIV:popToken();
      return std::make_shared<ASTLeaf>(nextToken.s, nextToken.tokentype);
    default:parseErrorUnexpectedToken("MulOp");
  }
}
// DONE
std::shared_ptr<ASTLeaf> Parse::readCmpOp() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case LT:
    case LE:
    case GT:
    case GE:
    case NE:
    case EQ:popToken();
      return std::make_shared<ASTLeaf>(nextToken.s, nextToken.tokentype);
    default:parseErrorUnexpectedToken("CmpOp");
  }
}
// DONE
std::shared_ptr<AST> Parse::readProgram() {
  std::shared_ptr<AST> ret(nullptr);
  // read all const declare
  ret = readConstDeclare();
  // value declare and funcs and main
  bool valuedeclare = true;
  bool main = true;
  auto tail = ret;
  while (main) {
    auto nextToken = getToken();
    if (nextToken.tokentype == EOFTOKEN) {
      parseError("expect main function");
    }
    tail = getTail(tail);
    if (nextToken.tokentype == VOID) {
      auto now = readFunOrMain();
      if (now->valueId == "main")main = false;
      tail->next = now;
      valuedeclare = false;
    } else if (nextToken.tokentype == CHAR || nextToken.tokentype == INT) {
      // func or value declare
      auto head = readDeclareHead();
      auto nextToken = getToken();
      std::shared_ptr<ASTDeclare> value;
      std::shared_ptr<ASTDeclareFun> func;
      switch (nextToken.tokentype) {
        case SEMICOLON:
        case COMMA:
        case LMBRACKETS:
          // value declare
          if (!valuedeclare)
            parseError("value must be declared before function declare");
          value = readValueDefine(head);
          tail->next = value;
          break;
        case LSBRACKETS:func = readFun();
          func->valueId = head.second;
          func->returnType = head.first;
          tail->next = func;
          valuedeclare = false;
          break;
        default:parseErrorUnexpectedToken(4, SEMICOLON, COMMA, LMBRACKETS, LSBRACKETS);
      }
    }
  }
  match(EOFTOKEN);
  return ret;
}
// DONE
std::shared_ptr<ASTDeclareFun> Parse::readFunOrMain() {
  auto Type = popToken().tokentype;
  auto nextToken = popToken();
  std::shared_ptr<ASTDeclareFun> ret;
  switch (nextToken.tokentype) {
    case MAIN:
      ret = readMain();
      break;
    case ID:
      ret = readFun();
      break;
    default:parseErrorUnexpectedToken(2, MAIN, ID);
  }
  ret->valueId = nextToken.s;
  ret->returnType = VOID;
  return ret;
}
// DONE
std::shared_ptr<ASTDeclareValue> Parse::readConstDeclare() {
  auto ret = std::shared_ptr<ASTDeclareValue>(nullptr);
  std::shared_ptr<AST> now = ret;
  while (getToken().tokentype == CONST) {
    now = getTail(now);
    popToken();
    if (ret) {
      now->next = readConstDefine();
      now = now->next;
    } else {
      ret = readConstDefine();
      now = ret;
    }
    match(SEMICOLON);
  }
  return ret;
}
// DONE
std::shared_ptr<ASTDeclareValue> Parse::readConstDefine() {
  auto ret = std::shared_ptr<ASTDeclareValue>(nullptr);
  std::shared_ptr<AST> now;
  auto nextToken = getToken();
  if (nextToken.tokentype == CHAR) {
    auto f = [this]() {
      auto ret = std::make_shared<ASTDeclareValue>();
      ret->isConst = true;
      ret->valueId = match(ID).s; // ID
      match(ASSIGN); // ASSIGN
      ret->value = match(CHARACTER).s; // CHARACTER
      ret->valueType = CHAR;
      return ret;
    };
    popToken(); // the CHAR token
    ret = f();
    now = ret;
    while (getToken().tokentype == COMMA) {
      popToken();
      now = getTail(now);
      now->next = f();
    }
  } else if (nextToken.tokentype == INT) {
    auto f = [this]() {
      auto ret = std::make_shared<ASTDeclareValue>();
      ret->isConst = true;
      ret->valueId = match(ID).s;
      match(ASSIGN);
      ret->value = readInteger();
      ret->valueType = INT;
      return ret;
    };
    popToken(); // the INT token
    ret = f();
    now = ret;
    while (getToken().tokentype == COMMA) {
      popToken();
      now = getTail(now);
      now->next = f();
    }
  } else {
    parseErrorUnexpectedToken(2, INT, CHAR);
  }
  return ret;
}
// DONE
std::string Parse::readInteger() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case ADD:
    case MINUS:popToken();
      return nextToken.s + match(UNSIGNED).s;
    case UNSIGNED:return popToken().s;
    default:parseErrorUnexpectedToken(3, ADD, MINUS, UNSIGNED);
  }
}
// DONE
std::pair<Tokentype, std::string> Parse::readDeclareHead() {
  auto nextToken = getToken();
  auto ret = std::pair<Tokentype, std::string>();
  switch (nextToken.tokentype){
    case INT:
    case CHAR:
      ret.first = nextToken.tokentype;
      popToken();
      break;
    default:
      parseErrorUnexpectedToken(2,INT,CHAR);
  }
  ret.second = match(ID).s;
  return ret;
}
// DONE
std::shared_ptr<ASTDeclare> Parse::readValueDefine(std::pair<Tokentype, std::string> head) {
  std::shared_ptr<ASTDeclare> ret(nullptr);
  if (getToken().tokentype == LMBRACKETS) {
    match(LMBRACKETS);
    ret = std::make_shared<ASTDeclareArray>(std::atoi(match(UNSIGNED).s.c_str()), head.first, head.second);
    match(RMBRACKETS);
  } else {
    ret = std::make_shared<ASTDeclareValue>(false, head.first, head.second, "");
  }
  std::shared_ptr<AST> tail = ret;
  while (getToken().tokentype == COMMA) {
    popToken();
    std::string id = match(ID).s;
    tail = getTail(tail);
    if (getToken().tokentype == LMBRACKETS) {
      match(LMBRACKETS);
      tail->next = std::make_shared<ASTDeclareArray>(std::atoi(match(UNSIGNED).s.c_str()), head.first, id);
      match(RMBRACKETS);
    } else {
      tail->next = std::make_shared<ASTDeclareValue>(false, head.first, id, "");
    }
  }
  match(SEMICOLON);
  return ret;
}
// DONE
std::shared_ptr<ASTLeaf> Parse::readConstant() {
  switch (getToken().tokentype) {
    case ADD:
    case MINUS:
    case UNSIGNED:return std::make_shared<ASTLeaf>(readInteger(), INT);
    case CHARACTER:return std::make_shared<ASTLeaf>(readCharacter(), CHARACTER);
    default:parseErrorUnexpectedToken(4, ADD, MINUS, UNSIGNED, CHARACTER);
  }
}
// DONE
Tokentype Parse::readType() {
  switch (getToken().tokentype) {
    case INT:
    case CHAR:return popToken().tokentype;
    default:parseErrorUnexpectedToken(2, INT, CHAR);
  }
}
// DONE
std::shared_ptr<ASTDeclareFun> Parse::readFun() {
  std::shared_ptr<ASTDeclareFun> ret;
  match(LSBRACKETS);
  ret->args = readArg();
  match(RSBRACKETS);
  match(LBBRACKETS);
  ret->body = readCompound_statements();
  match(RBBRACKETS);
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readCompound_statements() {
  std::shared_ptr<AST> ret, tail;
  ret = readConstDeclare();
  auto nextToken = getToken();
  while (nextToken.tokentype == INT || nextToken.tokentype == CHAR) {
    if (ret) {
      tail = getTail(tail);
      tail->next = readInFuncValueDeclare();
    } else {
      ret = tail = readInFuncValueDeclare();
    }
  }
  switch (nextToken.tokentype) {
    case SWITCH:
    case SEMICOLON:
    case PRINTF:
    case RETURN:
    case LBBRACKETS:
    case WHILE:
    case SCANF:
    case ID:
    case IF:
      if (ret) {
        tail = getTail(tail);
        tail->next = readStatements();
      } else {
        ret = tail = readStatements();
      }
  }
  return ret;
}
// DONE
std::shared_ptr<ASTDeclare> Parse::readInFuncValueDeclare() {
  auto head = readDeclareHead();
  return readValueDefine(head);
}
// DONE
std::vector<std::pair<Tokentype, std::string>> Parse::readArg() {
  std::vector<std::pair<Tokentype, std::string>> ret;
  auto nextToken = getToken();
  if (nextToken.tokentype == RSBRACKETS)return ret;
  do {
    auto type = readType();
    auto id = match(ID).s;
    ret.emplace_back(type, id);
  } while (getToken().tokentype == COMMA);
  return ret;
}
// DONE
std::shared_ptr<ASTDeclareFun> Parse::readMain() {
  std::shared_ptr<ASTDeclareFun> ret = std::make_shared<ASTDeclareFun>();
  match(LSBRACKETS);
  match(RSBRACKETS);
  match(LBBRACKETS);
  ret->valueId = "main";
  ret->body = readCompound_statements();
  ret->args = std::vector<std::pair<Tokentype, std::string>>();
  ret->returnType = VOID;
  match(RBBRACKETS);
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readExpression() {
  auto nextToken = getToken();
  std::shared_ptr<AST> ret;
  switch (nextToken.tokentype) {
    case ADD:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER:
      ret = readItem();
      break;
    default:parseErrorUnexpectedToken(6, ADD, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
  nextToken = getToken();
  while (nextToken.tokentype == ADD || nextToken.tokentype == MINUS) { // AddOp
    auto addOp = readAddOp();
    ret = std::make_shared<ASTStatement>(addOp->valueType, ret, readItem());
    nextToken = getToken();
  }
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readItem() {
  auto nextToken = getToken();
  std::shared_ptr<AST> ret;
  switch (nextToken.tokentype) {
    case ADD:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER:
      ret = readFactor();
      break;
    default:parseErrorUnexpectedToken(6, ADD, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
  nextToken = getToken();
  while (nextToken.tokentype == DIV || nextToken.tokentype == MUL) { // MulOp
    auto mulOp = readMulOp();
    ret = std::make_shared<ASTStatement>(mulOp->valueType, ret, readFactor());
    nextToken = getToken();
  }
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readFactor() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case MINUS:popToken();
      return std::make_shared<ASTStatement>(MINUS, readFactor1(), std::shared_ptr<AST>(nullptr));
    case ADD:popToken();
    case LSBRACKETS:
    case UNSIGNED:
    case CHARACTER:
    case ID:return readFactor1();
    default:parseErrorUnexpectedToken(6, MINUS, ADD, LSBRACKETS, UNSIGNED, CHARACTER, ID);
  }
}
// DONE
std::shared_ptr<AST> Parse::readFactor1() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case ID: {
      std::shared_ptr<ASTLeaf> tmp = std::make_shared<ASTLeaf>(nextToken.s, nextToken.tokentype);
      popToken();
      nextToken = getToken();
      if (nextToken.tokentype == LMBRACKETS) {
        // array
        match(LMBRACKETS);
        auto ret = std::make_shared<ASTStatement>(ARRAY, tmp, readStatement());
        match(RMBRACKETS);
        return ret;
      } else if (nextToken.tokentype == LSBRACKETS) {
        // callFunc
        match(LSBRACKETS);
        auto ret = std::make_shared<ASTCall>(tmp->value, readArgValue());
        match(RSBRACKETS);
        return ret;
      } else {
        // id
        return tmp;
      }
    }
    case UNSIGNED:
    case CHARACTER:nextToken = popToken();
      return std::make_shared<ASTLeaf>(nextToken.s, nextToken.tokentype);
    case LSBRACKETS: {
      std::shared_ptr<AST> ret(nullptr);
      popToken();
      ret = readStatement();
      match(RSBRACKETS);
      return ret;
    }
    default:parseErrorUnexpectedToken(4, ID, UNSIGNED, CHARACTER, LSBRACKETS);
  }
}
// DONE
std::shared_ptr<AST> Parse::readStatement() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case IF:return readCondition();
    case WHILE:return readLoop();
    case LBBRACKETS: {
      popToken();
      std::shared_ptr<AST> ret = readStatements();
      match(RBBRACKETS);
      return ret;
    }
    case SCANF:return readRead();
    case PRINTF:return readWrite();
    case SEMICOLON:
      return std::make_shared<ASTStatement>(UNDEFINED,
                                            std::shared_ptr<AST>(nullptr),
                                            std::shared_ptr<AST>(nullptr));
    case SWITCH:return readSwitch();
    case RETURN:return readReturn();
    case ID: {
      std::string id = popToken().s;
      nextToken = getToken();
      if (nextToken.tokentype == LSBRACKETS) {
        return readCall_fun(id);
      } else if (nextToken.tokentype == ASSIGN) {
        return readAssign(id);
      }
    }
    default:parseErrorUnexpectedToken(9, ID, WHILE, LBBRACKETS, SCANF, PRINTF, SEMICOLON, SWITCH, RETURN, ID);
  }
}
// DONE
std::shared_ptr<AST> Parse::readAssign(std::string id) {
  if (id == "")id = match(ID).s;
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case ASSIGN: {
      popToken();
      auto ret = std::make_shared<ASTStatement>();
      ret->operatorType = ASSIGN;
      ret->statement1 = std::make_shared<ASTLeaf>(id, ID);
      ret->statement2 = readExpression();
      return ret;
    }
    case LMBRACKETS: {
      popToken();
      auto ret = std::make_shared<ASTStatement>();
      ret->operatorType = ASSIGN;
      ret->statement1 = std::make_shared<ASTStatement>(ARRAY, std::make_shared<ASTLeaf>(id, ID), readExpression());
      match(RBBRACKETS);
      match(ASSIGN);
      ret->statement2 = readExpression();
      return ret;
    }
    default:parseErrorUnexpectedToken(2, ASSIGN, LMBRACKETS);
  }
}
// DONE
std::shared_ptr<ASTCondition> Parse::readCondition() {
  std::shared_ptr<ASTCondition> ret;
  match(IF);
  match(LSBRACKETS);
  ret->cmp = readBoolean();
  match(RBBRACKETS);
  ret->thenStatements = readStatement();
  ret->elseStatements = readElse();
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readElse() {
  auto nextToken = getToken();
  if (nextToken.tokentype == ELSE) {
    popToken();
    return readStatement();
  } else {
    return std::shared_ptr<ASTStatement>(nullptr);
  }
}
// DONE
std::shared_ptr<ASTStatement> Parse::readBoolean() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case ADD:
    case NOT:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER:return readOr();
    default:parseErrorUnexpectedToken(7, ADD, NOT, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
}
// DONE
std::shared_ptr<ASTStatement> Parse::readOr() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case ADD:
    case NOT:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER: {
      std::shared_ptr<ASTStatement> ret = readAnd();
      while (getToken().tokentype == OR) {
        popToken();
        ret = std::make_shared<ASTStatement>(OR, ret, readAnd());
      }
      return ret;
    }
    default:parseErrorUnexpectedToken(7, ADD, NOT, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
}
// DONE
std::shared_ptr<ASTStatement> Parse::readAnd() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case AND:
    case NOT:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER: {
      std::shared_ptr<ASTStatement> ret = readNot();
      while (getToken().tokentype == AND) {
        popToken();
        ret = std::make_shared<ASTStatement>(AND, ret, readNot());
      }
      return ret;
    }
    default:parseErrorUnexpectedToken(7, AND, NOT, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
}
// DONE
std::shared_ptr<ASTStatement> Parse::readNot() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case NOT:popToken();
      return std::make_shared<ASTStatement>(NOT, readCond(), std::shared_ptr<AST>(nullptr));
    case ADD:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER:return readCond();
    default:parseErrorUnexpectedToken(7, NOT, ADD, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
}
// DONE
std::shared_ptr<ASTStatement> Parse::readCond() {
  auto nextToken = getToken();
  switch (nextToken.tokentype) {
    case ADD:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER: {
      auto tmp = readExpression();
      std::shared_ptr<ASTStatement> ret(nullptr);
      nextToken = getToken();
      switch (nextToken.tokentype) {
        case GT:
        case LE:
        case GE:
        case NE:
        case EQ:
        case LT:ret = std::make_shared<ASTStatement>(readCmpOp()->valueType, tmp, readExpression());
          break;
        default:ret = std::make_shared<ASTStatement>(NE, tmp, std::make_shared<ASTLeaf>("0", INT));
      }
      return ret;
    }
    default:parseErrorUnexpectedToken(6, ADD, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
}
// DONE
std::shared_ptr<AST> Parse::readLoop() {
  auto ret = std::make_shared<ASTLoop>();
  match(WHILE);
  match(LSBRACKETS);
  ret->cmp = readCond();
  match(RSBRACKETS);
  ret->body = readStatement();
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readSwitch() {
  auto ret = std::make_shared<ASTSwitch>();
  match(SWITCH);
  match(LSBRACKETS);
  ret->expression = readExpression();
  match(RSBRACKETS);
  match(LBBRACKETS);
  ret->cases = readSwitch1();
  match(RBBRACKETS);
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readSwitch1() {
  auto a = readCases();
  auto b = readDefault();
  if (b)b->next = a;
  else return a;
}
// DONE
std::shared_ptr<AST> Parse::readDefault() {
  auto nextToken = getToken();
  if (nextToken.tokentype == DEFAULT) {
    popToken();
    match(COLON);
    return readStatement();
  }
  return nullptr;
}
// DONE
std::shared_ptr<AST> Parse::readCases() {
  auto nextToken = getToken();
  auto ret = std::shared_ptr<AST>(nullptr);
  auto tail = std::shared_ptr<AST>(nullptr);
  while (nextToken.tokentype == CASE) {
    if (ret) {
      tail = getTail(tail);
      tail->next = readCase();
    } else {
      ret = tail = readCase();
    }
  }
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readCase() {
  match(CASE);
  auto Case = readConstant();
  match(COLON);
  Case->next = readStatement();
  return Case;
}
// DONE
std::shared_ptr<ASTCall> Parse::readCall_fun(std::string id) {
  std::shared_ptr<ASTCall> ret(nullptr);
  if (id == "")id = match(ID).s;
  ret->funName = id;
  match(LSBRACKETS);
  ret->args = readArgValue();
  match(RSBRACKETS);
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readArgValue() {
  auto nextToken = getToken();
  std::shared_ptr<AST> ret(nullptr);
  switch (nextToken.tokentype) {
    case ADD:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER:ret = readExpression();
  }
  if (ret) {
    nextToken = getToken();
    auto tail = ret;
    while (nextToken.tokentype == COMMA) {
      popToken();
      tail = getTail(tail);
      tail->next = readExpression();
    }
  }
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readStatements() {
  std::shared_ptr<AST> ret(nullptr);
  std::shared_ptr<AST> tail(nullptr);
  while (true) {
    auto nextToken = getToken();
    switch (nextToken.tokentype) {
      case SEMICOLON:
      case PRINTF:
      case LBBRACKETS:
      case WHILE:
      case SCANF:
      case ID:
      case SWITCH:
      case IF:
      case RETURN:
        if (ret) {
          tail = getTail(tail);
          tail->next = readStatement();
        } else {
          ret = tail = readStatement();
        }
        continue;
    }
    break;
  }
  return ret;
}
// DONE
std::shared_ptr<ASTRead> Parse::readRead() {
  std::shared_ptr<ASTRead> ret = std::make_shared<ASTRead>();
  match(SCANF);
  match(LSBRACKETS);
  auto id = match(ID);
  ret->args = std::make_shared<ASTLeaf>(id.s, id.tokentype);
  std::shared_ptr<AST> tail = ret->args;
  auto nextToken = getToken();
  while (nextToken.tokentype == COMMA) {
    popToken();
    auto id = match(ID);
    tail = getTail(tail);
    tail->next = std::make_shared<ASTLeaf>(id.s, id.tokentype);
  }
  match(RSBRACKETS);
  return ret;
}
// DONE
std::shared_ptr<ASTWrite> Parse::readWrite() {
  std::shared_ptr<ASTWrite> ret = std::make_shared<ASTWrite>();
  match(PRINTF);
  match(LSBRACKETS);
  ret->args = readWrite2();
  std::shared_ptr<AST> tail = ret->args;
  while (getToken().tokentype == COMMA) {
    popToken();
    tail = getTail(tail);
    tail->next = readWrite2();
  }
  match(RSBRACKETS);
  return ret;
}
// DONE
std::shared_ptr<AST> Parse::readWrite2() {
  switch (getToken().tokentype) {
    case STRING: {
      auto s = match(STRING);
      return std::make_shared<ASTLeaf>(s.s, s.tokentype);
    }
    case ADD:
    case UNSIGNED:
    case LSBRACKETS:
    case ID:
    case MINUS:
    case CHARACTER:return readExpression();
    default:parseErrorUnexpectedToken(7, STRING, ADD, UNSIGNED, LSBRACKETS, ID, MINUS, CHARACTER);
  }
}
// DONE
std::shared_ptr<ASTRet> Parse::readReturn() {
  match(RETURN);
  return std::make_shared<ASTRet>(readExpression());
}
// DONE
std::string Parse::readCharacter() {
  return match(CHARACTER).s;
}
// DONE
std::string Parse::readId() {
  return match(ID).s;
}
// DONE
std::string Parse::readUnsigned() {
  return match(UNSIGNED).s;
}
// DONE
std::string Parse::readString() {
  return match(STRING).s;
}
Token Parse::match(Tokentype tokentype) {
  if (getToken().tokentype != tokentype)parseErrorUnexpectedToken(1, tokentype);
  return popToken();
}
std::shared_ptr<AST> Parse::getTail(std::shared_ptr<AST> now) {
  while (now && now->next)now = now->next;
  return now;
}
void Parse::parseErrorUnexpectedToken(int num, ...) {
  va_list va_l;
  std::string s;
  Tokentype tokentype;
  va_start(va_l, num);
  tokentype = (Tokentype)va_arg(va_l, int);
  s = toString(tokentype);
  for (int i = 1; i < num; i++)s += " or " + toString((Tokentype)va_arg(va_l, int));
  va_end(va_l);
  parseErrorUnexpectedToken(s.c_str());
}
Parse::Parse() {}
