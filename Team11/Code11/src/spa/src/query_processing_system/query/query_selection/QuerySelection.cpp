#include "QuerySelection.h"

QuerySelection::QuerySelection() {
    this->selectedValues = {};
    this->resultList = {};
}

QuerySelection::QuerySelection(const std::shared_ptr<ISelectable> &selectedValue) {
    this->selectedValues = {selectedValue};
    this->resultList = {};
}

QuerySelection::QuerySelection(const std::vector<std::shared_ptr<ISelectable>> &selectedValues) {
    this->selectedValues = selectedValues;
    this->resultList = {};
}

void QuerySelection::generateResultList(const std::vector<std::string>& otherValues) {
    if (otherValues.size() == selectedValues.size()) {
        // all values set, add to the list
        std::string res;
        for (const auto& value : otherValues) {
            res += value;
            res += " ";
        }
        res.pop_back(); // remove last space
        resultList.push_back(res);
        return;
    }

    auto thisSelectionIndex = otherValues.size();
    auto thisSelection = selectedValues[thisSelectionIndex];
    auto thisSelectionValues = thisSelection->stringifyPossibleValues();

    // for each other synonym, get a list of relationships for this value
    // for each of current value, check relationship
    for (const auto& value : thisSelectionValues) {
        bool fitsOthers = true;
        for (int i = 0; i < otherValues.size(); i++) {
            auto correspondingSelection = selectedValues[i];
            const auto& correspondingValue = otherValues[i];

            auto relationships = thisSelection->getRelationships(correspondingSelection, value);
            if (relationships.find(correspondingValue) == relationships.end()) {
                fitsOthers = false;
            }
        }

        // if fits others, dig deeper
        if (fitsOthers) {
            auto nextValues = otherValues;
            nextValues.push_back(value);
            generateResultList(nextValues);
        }
    }
}

std::list<std::string> QuerySelection::getAnswer(bool isTruthy) {
    if (selectedValues.empty()) {
        return {isTruthy ? "TRUE" : "FALSE"};
    } else if (!isTruthy) {
        // has synonyms, but clauses are falsy -> empty result list
        return {};
    }

    resultList = {};
    generateResultList({});
    return resultList;
}

bool QuerySelection::operator==(const QuerySelection& other) const {
    return selectedValues == other.selectedValues;
}

std::vector<std::shared_ptr<ISynonym>> QuerySelection::getSelectedSynonyms() {
    std::vector<std::shared_ptr<ISynonym>> res;
    for (const auto& selection : selectedValues) {
        res.push_back(selection->getSynonym());
    }
    return res;
}
