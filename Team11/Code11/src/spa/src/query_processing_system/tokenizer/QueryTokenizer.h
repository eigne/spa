#pragma once

#include "commons/tokenizer/Tokenizer.h"

class QueryTokenizer : public Tokenizer {
public:
    explicit QueryTokenizer(std::istream* stream);
};
