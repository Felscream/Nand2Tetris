#include "Parser.h"

Parser::Instruction::Instruction() = default;
Parser::Instruction::Instruction(std::string dest, std::string comp,
                                 std::string jump, std::string symbol) {
  this->dest = dest;
  this->comp = comp;
  this->jump = jump;
  this->symbol = symbol;
}

Parser::Parser(std::filesystem::path path) {
  file.open(path);
  lineNumber = 0;
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << path << std::endl;
  }
}

bool Parser::hasMoreLines() {
  if (hasBuffered) {
    return true;
  }
  if (std::getline(file, buffer)) {
    hasBuffered = true;
    return true;
  }
  return false;
}

void Parser::advance() {
  do {
    if (hasBuffered) {
      line = buffer;
      hasBuffered = false;
    } else {
      std::getline(file, line);
    }
  } while (hasMoreLines() && this->isCommentOrWhiteSpace() >= 0);
  line.erase(line.find_last_not_of(" \t\r\n") + 1);
  InstructionType type = getInstructionType();
  if (type != L_INSTRUCTION) {
    lineNumber++;
  }
  parseInstruction(type);
}

InstructionType Parser::getInstructionType() {
  size_t pos = line.find_first_not_of(" \t\r\n");
  if (pos == std::string::npos) {
    throw std::invalid_argument("Cannot parse line");
  }

  if (line[pos] == '@') {
    return InstructionType::A_INSTRUCTION;
  }
  if (line[pos] == '(') {
    return InstructionType::L_INSTRUCTION;
  }
  return InstructionType::C_INSTRUCTION;
}

std::string Parser::dest() { return currentInstruction.dest; }
std::string Parser::comp() { return currentInstruction.comp; }
std::string Parser::jump() { return currentInstruction.jump; }
std::string Parser::symbol() { return currentInstruction.symbol; }

int Parser::isCommentOrWhiteSpace() {
  // returns the index of the first '/' if it exists
  // or 0 if the line is just whitespace
  // or -1 if the line starts with valid instructions
  size_t pos = line.find_first_not_of(" \t\r\n");
  if (pos == std::string::npos) {
    return 0;
  }

  if (line[pos] == '/' && line[pos + 1] == '/') {
    return pos; // comment line
  }

  return -1;
}

void Parser::parseInstruction(InstructionType type) {
  std::smatch matches;
  switch (type) {
  case A_INSTRUCTION:
    parseAInstruction();
    break;
  case L_INSTRUCTION:
    parseLInstruction();
    break;
  case C_INSTRUCTION:
    parseCInstruction();
    break;
  default:
    throw std::invalid_argument("Unsupported instruction type");
  }
}

void Parser::parseAInstruction() {
  std::smatch match;
  if (std::regex_match(line, match, aRegexPattern)) {
    currentInstruction = Instruction("", "", " ", match[1]);
  }
}

void Parser::parseLInstruction() {
  std::smatch match;
  if (std::regex_match(line, match, lRegexPattern)) {
    currentInstruction = Instruction("", "", " ", match[1]);
  }
}

void Parser::parseCInstruction() {
  std::smatch match;
  if (std::regex_match(line, match, cRegexPattern)) {
    currentInstruction = Instruction(match[1], match[2], match[3], "");
  }
}

void Parser::reset() {
  file.clear();
  file.seekg(0, std::ios::beg);
  lineNumber = 0;
}

int Parser::getCurrentLineNumber() { return lineNumber; }

Parser::~Parser() {
  if (file.is_open()) {
    file.close();
  }
}