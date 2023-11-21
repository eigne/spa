#pragma once

#include <string>
#include <list>
#include <memory>
#include "source_processor/SourceProcessor.h"
#include "program_knowledge_base/PKBFactory.h"
#include "query_processing_system/QPS.h"

class StaticProgramAnalyser {
private:
    SourceProcessor sourceProcessor;
    QPS qps;
    std::shared_ptr<PKBSP> pkbSP;
    std::shared_ptr<PKBQPS> pkbQPS;

public:
    StaticProgramAnalyser();
    void processSourceFile(const std::string& sourceFile);
    void evaluateQuery(const std::string& query, std::list<std::string>& results);
};
