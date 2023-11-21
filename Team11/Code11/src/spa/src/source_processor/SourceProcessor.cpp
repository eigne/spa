#include "SourceProcessor.h"

void SourceProcessor::process(const std::string& sourceFile, std::shared_ptr<PKBSP> pkb) {
    std::ifstream input(sourceFile);

    SimpleProgramTokenizer tokenizer = SimpleProgramTokenizer(&input);
    auto tokenList = tokenizer.tokenize();

    ParserManager parserManager;
    auto root = parserManager.generateAST(tokenList);

    std::shared_ptr<StorageWriter> storageWriter = std::make_shared<StorageWriter>(pkb);
    DesignExtractor designExtractor(storageWriter);
    designExtractor.extract(root);

    input.close();
}
