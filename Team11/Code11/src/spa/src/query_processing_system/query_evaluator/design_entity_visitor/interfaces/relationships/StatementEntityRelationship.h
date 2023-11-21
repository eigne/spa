#pragma once

#include <vector>
#include <string>
#include <utility>
#include <unordered_set>
#include <unordered_map>

class StatementEntityRelationship {
protected:
    std::vector<std::pair<int, std::string> > statementEntityRelationships;
public:
    std::unordered_map<int, std::unordered_set<std::string> > getStatementRelationships();
    std::unordered_map<std::string, std::unordered_set<int> > getEntityRelationships();
};
