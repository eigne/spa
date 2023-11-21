#pragma once

#include <unordered_map>
#include <unordered_set>
#include "IModifiesStatementReader.h"
#include "IUsesStatementReader.h"
#include "INextReader.h"
#include "ISynonymReader.h"

class IAffectsReader : public virtual IModifiesStatementReader, public virtual IUsesStatementReader, public virtual INextReader,
        public virtual ISynonymReader {
private:
    std::unordered_map<int, std::unordered_set<int>> affectsForwardCache;
    std::unordered_map<int, std::unordered_set<int>> affectsBackwardCache;
    bool isReadOrCalls(int neighbour);

public:
    std::unordered_set<int> queryAllAffects(int statementNumber, bool queryForAffects);
};
