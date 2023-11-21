#include "QPS.h"

void QPS::evaluateQuery(const std::string& string, std::list<std::string>& results,
                        const std::shared_ptr<PKBQPS>& pkb) {
    std::istringstream input;
    input.str(string);

    QueryTokenizer tokenizer(&input);
    std::vector<std::shared_ptr<Token> > tokens = tokenizer.tokenize();

    TokenSplitter splitter;
    auto tokenGroups = splitter.splitIntoTokenGroups(tokens);

    QPSValidator validator(tokenGroups);
    std::string error = validator.validate();
    if (!error.empty()) {
        results.push_back(error);
        return;
    }

    QueryExtractor extractor(tokenGroups);
    Query query = extractor.extractQuery();

    std::shared_ptr<IStorageReader> storageReader = std::make_shared<QPSStorageReader>(pkb);
    this->evaluator = QueryEvaluator(storageReader);
    std::list<std::string> evaluatorResult = evaluator.evaluateQuery(query);
    for (const auto& res : evaluatorResult) {
        results.push_back(res);
    }
}
