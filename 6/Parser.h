#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

enum InstructionType { A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION };

class Parser {
public:
  Parser(std::filesystem::path path);

  bool hasMoreLines();
  void advance();
  InstructionType getInstructionType();
  std::string dest();
  std::string comp();
  std::string jump();
  std::string symbol();
  int getCurrentLineNumber();
  void reset();
  ~Parser();

private:
  struct Instruction {
    std::string dest;
    std::string comp;
    std::string jump;
    std::string symbol;
    Instruction();
    Instruction(std::string dest, std::string comp, std::string jump,
                std::string symbol);
  };
  std::ifstream file;
  std::string line;
  std::string buffer;
  bool hasBuffered;
  int lineNumber;
  Instruction currentInstruction;
  std::regex aRegexPattern = std::regex("^\\s*@([A-Za-z0-9.$_]+)$");
  std::regex lRegexPattern = std::regex("^\\s*\\(([A-Za-z0-9.$_]+)\\)$");
  std::regex cRegexPattern = std::regex(
      "^\\s*([ADM]*)=?([ADM+-10!|&]+);?(?:(JGT|JEQ|JGE|JLT|JNE|JLE|JMP))?$");
  int isCommentOrWhiteSpace();
  void parseInstruction(InstructionType type);
  void parseAInstruction();
  void parseLInstruction();
  void parseCInstruction();
};