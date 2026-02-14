#ifndef FLEECE_DEBUG_H_

#define FLEECE_DEBUG_H_

#include "lexer.hpp"
#include "parser.hpp"
#include <vector>

void print_lex_output(std::vector<TokenNode>&);
void print_parsed_node(const ParsedNode* node);

#endif
