#pragma once

#include "commons/tokenizer/Tokenizer.h"

class SimpleProgramTokenizer : public Tokenizer {
public:
    explicit SimpleProgramTokenizer(std::istream* stream);
};
