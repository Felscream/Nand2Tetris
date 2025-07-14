#include "Encoder.h"

Encoder::Encoder() {
  nextSymbolAddress = 16;
  compCodes = {
      {"0", "101010"},   {"1", "111111"},   {"-1", "111010"},
      {"D", "001100"},   {"A", "110000"},   {"M", "110000"},
      {"!D", "001101"},  {"!A", "110001"},  {"!M", "110001"},
      {"-D", "001111"},  {"-A", "110011"},  {"-M", "110011"},
      {"D+1", "011111"}, {"A+1", "110111"}, {"M+1", "110111"},
      {"D-1", "001110"}, {"A-1", "110010"}, {"M-1", "110010"},
      {"D+A", "000010"}, {"D+M", "000010"}, {"D-A", "010011"},
      {"D-M", "010011"}, {"A-D", "000111"}, {"M-D", "000111"},
      {"D&A", "000000"}, {"D&M", "000000"}, {"D|A", "010101"},
      {"D|M", "010101"}, {"", ""},
  };

  jumpCodes = {{"", "000"},    {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
               {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};

  symbols = {{"SP", 0},   {"LCL", 1},        {"ARG", 2},    {"THIS", 3},
             {"THAT", 4}, {"SCREEN", 16384}, {"KBD", 24576}};
}

std::string Encoder::encodeDest(const std::string &dest) {
  std::string res = "000";

  for (int i = 0; i < dest.size(); i++) {
    switch (dest[i]) {
    case 'A':
      res[0] = '1';
      break;
    case 'D':
      res[1] = '1';
      break;
    case 'M':
      res[2] = '1';
      break;
    }
  }
  return res;
}

std::string Encoder::encodeComp(const std::string &comp) {
  if (comp.empty()) {
    return "";
  }
  return compCodes[comp];
}

std::string Encoder::encodeJump(const std::string &jump) {
  return jumpCodes[jump];
}

void Encoder::registerLoop(const std::string &symbol, const int &lineNumber) {
  symbols[symbol] = lineNumber;
}

std::string Encoder::encodeSymbol(const std::string &symbol) {
  // handle @R0, @R1...@R15
  if (symbol[0] == 'R') {
    int length = symbol.size() - 1;
    std::string address = symbol.substr(1, length);
    if (isNumber(address)) {
      int val = std::stoi(address);
      if (val < 16) {
        return toBinary(val);
      }
    }
  }

  // handles @1, @2133
  if (isNumber(symbol)) {
    return toBinary(std::stoi(symbol));
  }

  // handles @SCREEN, @sum, @R16, @LOOP
  if (symbols.find(symbol) != symbols.end()) {
    return toBinary(symbols[symbol]);
  } else {
    // adds the new @symbol in the next available address
    symbols[symbol] = nextSymbolAddress;
    nextSymbolAddress++;
    return toBinary(symbols[symbol]);
  }
}

bool Encoder::isNumber(const std::string &val) {
  return !val.empty() &&
         std::find_if(val.begin(), val.end(), [](unsigned char c) {
           return !std::isdigit(c);
         }) == val.end();
}

std::string Encoder::toBinary(int val) {
  std::string bin = "0000000000000000";
  int i = 15;
  while (val > 0 && i >= 0) {
    bin[i] = (val % 2) + '0';
    val = val / 2;
    i--;
  }
  return bin;
}