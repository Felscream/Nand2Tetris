#include "Encoder.h"
#include "Parser.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

std::filesystem::path getOutputDir(std::filesystem::path path) {
  if (!std::filesystem::exists(path)) {
    throw std::invalid_argument("The given path couldn't be found");
  };
  if (std::filesystem::is_directory(path)) {
    throw std::invalid_argument("The given path is not a file");
  }

  if (path.extension() != ".asm") {
    throw std::invalid_argument("Unsupported file extension");
  }
  return std::filesystem::absolute(path.parent_path());
}

int main(int argc, char *argv[]) {
  std::cout << argc << " arguments passed" << std::endl;

  if (argc < 2 || argc > 2) {
    std::cout << "Input only the path to the assembly file." << std::endl;
    return -1;
  }

  std::filesystem::path path = argv[1];
  std::filesystem::path outputDir = getOutputDir(path);
  std::filesystem::path outputFilePath = outputDir / path.stem();
  outputFilePath.replace_extension(".hack");

  std::ofstream outputFile;

  Parser parser(path);
  Encoder encoder;

  // first loop to detect L instructions
  while (parser.hasMoreLines()) {
    parser.advance();

    InstructionType type = parser.getInstructionType();
    if (type == L_INSTRUCTION) {
      encoder.registerLoop(parser.symbol(), parser.getCurrentLineNumber());
    }
  }
  parser.reset();

  outputFile.open(outputFilePath);
  if (!outputFile.is_open()) {
    std::cerr << "Error creating file " << outputFilePath << std::endl;
    return -1;
  }

  while (parser.hasMoreLines()) {
    parser.advance();
    InstructionType type = parser.getInstructionType();
    if (type == A_INSTRUCTION) {
      std::string code = encoder.encodeSymbol(parser.symbol());
      outputFile << code << std::endl;
    } else if (type == C_INSTRUCTION) {
      std::string code = "111";
      std::string comp = parser.comp();
      int fromA = comp.find('M');
      if (fromA != std::string::npos) {
        code.append("1");
      } else {
        code.append("0");
      }
      std::string encodedComp = encoder.encodeComp(comp);
      std::string encodedDest = encoder.encodeDest(parser.dest());
      std::string encodedJump = encoder.encodeJump(parser.jump());
      code.append(encodedComp).append(encodedDest).append(encodedJump);
      outputFile << code << std::endl;
    }
  }
  outputFile.close();

  return 0;
}