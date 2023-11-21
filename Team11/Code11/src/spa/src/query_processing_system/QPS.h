#pragma once

#include <string>
#include <memory>
#include <list>
#include <sstream>
#include "program_knowledge_base/facades/PKBQPS.h"
#include "query_processing_system/query_evaluator/QueryEvaluator.h"
#include "query_processing_system/query_evaluator/storage_reader/QPSStorageReader.h"
#include "query_processing_system/query_extractor/QueryExtractor.h"
#include "query_processing_system/validator/QPSValidator.h"
#include "query_processing_system/tokenizer/QueryTokenizer.h"

class QPS {
private:
    QueryEvaluator evaluator;
public:
    void evaluateQuery(const std::string& string, std::list<std::string>& results, const std::shared_ptr<PKBQPS>& pkb);
};
