#include "token.h"

std::string Token::str() const {
  return type_ + " " + lexeme_ + " " + literal_;
}
