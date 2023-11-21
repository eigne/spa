#pragma once

#include "query_processing_system/query/design_entities/with_ref/attributes/IEntityAttribute.h"
#include "query_processing_system/query/design_entities/with_ref/attributes/procedure_name/EntityWithProcedureName.h"

class EntityProcedureNameAttribute : public IEntityAttribute {
private:
    std::shared_ptr<EntityWithProcedureName> synonym;
public:
    explicit EntityProcedureNameAttribute(std::shared_ptr<EntityWithProcedureName> synonym);
    std::shared_ptr<ISynonym> getSynonym() override;
    std::unordered_set<std::string> getRelationships(const std::shared_ptr<ISelectable> &other,
                                                     const std::string &thisValue) override;
    std::unordered_set<std::string> stringifyPossibleValues() override;
    std::string getSelectedValue(const std::string& synonymValue) override;
    std::unordered_map<std::string, std::unordered_set<std::string>> getAttributeAndSynonymValues() override;
};
