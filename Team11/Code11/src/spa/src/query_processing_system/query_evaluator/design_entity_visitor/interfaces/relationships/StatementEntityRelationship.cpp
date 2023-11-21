#include "StatementEntityRelationship.h"

std::unordered_map<int, std::unordered_set<std::string> > StatementEntityRelationship::getStatementRelationships() {
    std::unordered_map<int, std::unordered_set<std::string> > res;
    for (const auto& relationship : statementEntityRelationships) {
        if (res.find(relationship.first) == res.end()) {
            res.insert({relationship.first, {}});
        }
        res[relationship.first].insert(relationship.second);
    }
    return res;
}

std::unordered_map<std::string, std::unordered_set<int> > StatementEntityRelationship::getEntityRelationships() {
    std::unordered_map<std::string, std::unordered_set<int> > res;
    for (const auto& relationship : statementEntityRelationships) {
        if (res.find(relationship.second) == res.end()) {
            res.insert({relationship.second, {}});
        }
        res[relationship.second].insert(relationship.first);
    }
    return res;
}
