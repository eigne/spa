#include "SimpleProgramTokenizer.h"

SimpleProgramTokenizer::SimpleProgramTokenizer(std::istream* stream)
    : Tokenizer(stream,
                { "&", "|", ">", "<", "=", "!" }, // first tokens
                { "&&", "||", ">=", "<=", "==", "!=" }) {
}                                                       // joined tokens
