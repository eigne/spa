#include "IAffectsReader.h"

bool IAffectsReader::isReadOrCalls(int neighbour) {
    return getAllReads().count(neighbour) || getAllCalls().count(neighbour);
}

std::unordered_set<int> IAffectsReader::queryAllAffects(int statementNumber, bool queryForAffects) {
    if (queryForAffects) {
        if (affectsForwardCache.count(statementNumber)) {
            return affectsForwardCache[statementNumber];
        }
    } else {
        if (affectsBackwardCache.count(statementNumber)) {
            return affectsBackwardCache[statementNumber];
        }
    }

    std::unordered_set<int> res = {};
    auto assignStmts = getAllAssigns();
    if (assignStmts.count(statementNumber) == 0) {
        return res;
    }

    std::unordered_set<std::string> vars = queryForAffects ?
        getVariablesModifiedInStatement(statementNumber) :
        getVariablesUsedInStatement(statementNumber);

    for (const auto& var : vars) {
        std::unordered_set<int> visited = {};
        std::vector<int> stack = { statementNumber };
        auto stmtsThatUseVar = getStatementsThatUseVariable(var);
        auto stmtsThatModifiesVar = getStatementsThatModifyVariable(var);
        while (!stack.empty()) {
            int curr = stack.back();
            stack.pop_back();

            if (visited.count(curr))
                continue;
            visited.insert(curr);

            auto neighbours = queryForAffects ?
                getNext(curr) :
                getPrevious(curr);

            for (auto neighbour : neighbours) {
                if (queryForAffects) {
                    if (assignStmts.count(neighbour) && stmtsThatUseVar.count(neighbour) &&
                        getVariablesUsedInStatement(neighbour).count(var)) {
                        res.insert(neighbour);
                    }
                } else {
                    if (assignStmts.count(neighbour) && stmtsThatModifiesVar.count(neighbour) &&
                        getVariablesModifiedInStatement(neighbour).count(var)) {
                        res.insert(neighbour);
                    }
                }

                // if neighbour modifies varModified, no need to check further
                // i.e. exclude control flow path after nighbour from DFS
                if (stmtsThatModifiesVar.count(neighbour) &&
                    (assignStmts.count(neighbour) || isReadOrCalls(neighbour))) {
                    continue;
                }

                if (!visited.count(neighbour)) {
                    stack.push_back(neighbour);
                }
            }
        }
    }

    if (queryForAffects) {
        affectsForwardCache[statementNumber].insert(res.begin(), res.end());
    } else {
        affectsBackwardCache[statementNumber].insert(res.begin(), res.end());
    }

    return res;
}
