//
// Created by zhaoyiping on 2020/3/31.
//

#include "parse.h"
void parseError(const char *errorMessage) {
  std::string em = "Parse Error: " + (std::string)errorMessage;
  error(em.c_str());
}
std::shared_ptr<Tokenizer>ParseState::tokenlizer;
Token ParseState::nextToken = Token();
Parse::Parse(std::shared_ptr<Tokenizer> tokenlizer) : tokenlizer(tokenlizer) {}
void Parse::init() {
  ParseState::tokenlizer = tokenlizer;
}
void Parse::parse() {
  std::stack<std::tuple<int, ParseStateVectorType, ParseStateFunctionType, ParseStateAST>> st;
  //  st.push(); FIXME: add the begin nonterminal type to st
  while (!st.empty()) {
    auto[pointer, stateVector, generator, ASTs]=st.top();
    st.pop();
    if (pointer == stateVector.size()) {
      std::shared_ptr<AST> result = generator(ASTs);
      if (st.empty())
        root = result;
      else {
        get<3>(st.top()).emplace_back(result);
      }
    } else {
      int nextPointer = 0;
      auto[nextStateVector, nextGenerator]=stateVector[pointer]->getNext();
      auto nextASTs = ParseStateAST();
      st.emplace(pointer + 1, stateVector, generator, ASTs);
      st.emplace(nextPointer, nextStateVector, nextGenerator, nextASTs);
    }
  }
}
Token ParseState::getToken() {
  if (nextToken.tokentype == UNDEFINED)
    nextToken = tokenlizer->getToken();
  return nextToken;
}
Token ParseState::popToken() {
  Token retToken = getToken();
  nextToken = tokenlizer->getToken();
  return retToken;
}
std::pair<ParseStateVectorType, ParseStateFunctionType > ParseState::getNext() {
  if (rules->find(getToken().tokentype) != rules->end())
    return rules->find(getToken().tokentype)->second;
  else if (rules->find(NULLStatement) != rules->end())
    return rules->find(NULLStatement)->second;
    //TODO: parseError
  else parseError("");
}
