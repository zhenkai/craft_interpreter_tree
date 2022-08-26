#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace {
  void run(const std::string& source) {
    std::cout << source << std::endl;
  }
  
  void runFile(const std::string& path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    run(buffer.str());
  }

  void runPrompt() {
    while(true) {
      std::cout << "> ";
      std::string line;
      std::getline(std::cin, line);
      run(line);
    }
  }
}

int main(int argc, char *argv[]) {
  switch(argc) {
    case 1:
      runPrompt(); break;
    case 2:
      runFile(argv[0]); break;
    default:
      std::cout << "Usage: lox [script]" << std::endl;
      return 64;
  }
  return 0;
}
