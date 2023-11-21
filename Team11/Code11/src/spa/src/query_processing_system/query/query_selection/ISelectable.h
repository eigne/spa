#pragma once

#include <unordered_set>
#include <string>
#include <memory>

class ISynonym;

class ISelectable {
public:
    virtual std::shared_ptr<ISynonym> getSynonym() = 0;
    virtual std::unordered_set<std::string> stringifyPossibleValues() = 0;
    virtual std::string getSelectedValue(const std::string& synonymValue) = 0;
    // if relationship doesn't exist, this should just return all possible values
    virtual std::unordered_set<std::string> getRelationships(const std::shared_ptr<ISelectable>& other,
                                                             const std::string& thisValue) = 0;
};
