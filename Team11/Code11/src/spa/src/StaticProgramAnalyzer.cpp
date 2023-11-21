#include "StaticProgramAnalyzer.h"

StaticProgramAnalyser::StaticProgramAnalyser() {
    std::pair<PKBSP, PKBQPS> stores = PKBFactory::createPKB();
    this->pkbSP = std::make_shared<PKBSP>(stores.first);
    this->pkbQPS = std::make_shared<PKBQPS>(stores.second);
}

void StaticProgramAnalyser::processSourceFile(const std::string& sourceFile) {
    this->sourceProcessor.process(sourceFile, this->pkbSP);
}

void StaticProgramAnalyser::evaluateQuery(const std::string& query, std::list<std::string>& results) {
    this->qps.evaluateQuery(query, results, this->pkbQPS);
}
