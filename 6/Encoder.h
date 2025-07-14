#pragma once
#include "Parser.h"
#include <string>
#include <unordered_map>
class Encoder {
public:
  std::string encodeDest(const std::string &dest);
  std::string encodeComp(const std::string &comp);
  std::string encodeJump(const std::string &jump);
  std::string encodeSymbol(const std::string &symbol);
  void registerLoop(const std::string &symbol, const int &line);
  Encoder();

private:
  int nextSymbolAddress;
  std::unordered_map<std::string, std::string> compCodes;
  std::unordered_map<std::string, std::string> jumpCodes;
  std::unordered_map<std::string, int> symbols;
  std::string toBinary(int val);
  bool isNumber(const std::string &val);
};
