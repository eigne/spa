#pragma once

#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>

class StatementStatementRelationship {
protected:
    std::vector<std::pair<int, int> > statementStatementRelationships;
public:
    std::unordered_map<int, std::unordered_set<int> > getStatementOneRelationships();
    std::unordered_map<int, std::unordered_set<int> > getStatementTwoRelationships();
};
