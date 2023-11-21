#pragma once

#include <vector>
#include <string>
#include <utility>
#include <unordered_set>
#include <unordered_map>

class EntityEntityRelationship {
protected:
    std::vector<std::pair<std::string, std::string> > entityEntityRelationships;
public:
    std::unordered_map<std::string, std::unordered_set<std::string> > getEntityOneRelationships();
    std::unordered_map<std::string, std::unordered_set<std::string> > getEntityTwoRelationships();
};
