#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
  switch(argc) {
    case 0:
      // runPrompt(); break;
    case 1:
      // runFile(argv[0]); break;
    default:
      std::cout << "Usage: lox [script]" << std::endl;
      return 64;
  }
  return 0;
}
