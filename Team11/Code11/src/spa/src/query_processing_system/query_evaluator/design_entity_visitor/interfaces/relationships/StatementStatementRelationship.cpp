#include "StatementStatementRelationship.h"

std::unordered_map<int, std::unordered_set<int> > StatementStatementRelationship::getStatementOneRelationships() {
    std::unordered_map<int, std::unordered_set<int> > res;
    for (auto relationship : statementStatementRelationships) {
        if (res.find(relationship.first) == res.end()) {
            res.insert({relationship.first, {}});
        }
        res[relationship.first].insert(relationship.second);
    }
    return res;
}

std::unordered_map<int, std::unordered_set<int> > StatementStatementRelationship::getStatementTwoRelationships() {
    std::unordered_map<int, std::unordered_set<int> > res;
    for (auto relationship : statementStatementRelationships) {
        if (res.find(relationship.second) == res.end()) {
            res.insert({relationship.second, {}});
        }
        res[relationship.second].insert(relationship.first);
    }
    return res;
}
