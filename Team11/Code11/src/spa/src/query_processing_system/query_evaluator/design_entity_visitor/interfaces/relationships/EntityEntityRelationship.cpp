#include "EntityEntityRelationship.h"

std::unordered_map<std::string,
                   std::unordered_set<std::string> > EntityEntityRelationship::getEntityOneRelationships() {
    std::unordered_map<std::string, std::unordered_set<std::string> > res;
    for (const auto& relationship : entityEntityRelationships) {
        if (res.find(relationship.first) == res.end()) {
            res.insert({relationship.first, {}});
        }
        res[relationship.first].insert(relationship.second);
    }
    return res;
}

std::unordered_map<std::string,
                   std::unordered_set<std::string> > EntityEntityRelationship::getEntityTwoRelationships() {
    std::unordered_map<std::string, std::unordered_set<std::string> > res;
    for (const auto& relationship : entityEntityRelationships) {
        if (res.find(relationship.second) == res.end()) {
            res.insert({relationship.second, {}});
        }
        res[relationship.second].insert(relationship.first);
    }
    return res;
}
