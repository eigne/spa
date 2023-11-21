#pragma once

#include <string>
#include <fstream>
#include <memory>
#include "source_processor/tokenizer/SimpleProgramTokenizer.h"
#include "source_processor/parser/ParserManager.h"
#include "design_extractor/DesignExtractor.h"

class SourceProcessor {
public:
    void process(const std::string& file, std::shared_ptr<PKBSP> pkb);
};
