#include "QueryTokenizer.h"

QueryTokenizer::QueryTokenizer(std::istream* stream)
    : Tokenizer(stream,
                { "Parent", "Follows", "Calls", "Next", "stmt" },     // first tokens
                { "Parent*", "Follows*", "Calls*", "Next*", "stmt#" }) {
}                                                                              // joined tokens
