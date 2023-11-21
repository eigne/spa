#pragma once

#include <memory>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include "query_processing_system/query/query_selection/ISelectable.h"

class QuerySelection {
private:
    std::vector<std::shared_ptr<ISelectable>> selectedValues;
    std::list<std::string> resultList;
    void generateResultList(const std::vector<std::string>& otherValues);

public:
    QuerySelection();
    explicit QuerySelection(const std::shared_ptr<ISelectable>& selectedSynonym);
    explicit QuerySelection(const std::vector<std::shared_ptr<ISelectable>>& selectedSynonyms);

    std::list<std::string> getAnswer(bool isTruthy);
    bool operator==(const QuerySelection& other) const;
    std::vector<std::shared_ptr<ISynonym>> getSelectedSynonyms();
};
