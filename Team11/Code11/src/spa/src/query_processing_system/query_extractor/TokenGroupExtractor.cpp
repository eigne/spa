#include "TokenGroupExtractor.h"

std::shared_ptr<ISynonym> TokenGroupExtractor::createSynonym(const std::string& type, const std::string& name) {
    if (type == "stmt") {
        return std::make_shared<StatementSynonym>(name);
    } else if (type == "read") {
        return std::make_shared<ReadSynonym>(name);
    } else if (type == "print") {
        return std::make_shared<PrintSynonym>(name);
    } else if (type == "call") {
        return std::make_shared<CallSynonym>(name);
    } else if (type == "while") {
        return std::make_shared<WhileSynonym>(name);
    } else if (type == "if") {
        return std::make_shared<IfSynonym>(name);
    } else if (type == "assign") {
        return std::make_shared<AssignSynonym>(name);
    } else if (type == "variable") {
        return std::make_shared<VariableSynonym>(name);
    } else if (type == "constant") {
        return std::make_shared<ConstantSynonym>(name);
    } else if (type == "procedure") {
        return std::make_shared<ProcedureSynonym>(name);
    }
    return nullptr;
}

std::shared_ptr<ISelectable> TokenGroupExtractor::createSelectable(const std::shared_ptr<ISynonym>& synonym,
                                                                   const std::string& selectionType) {
    if (selectionType.empty()) {
        return synonym;
    } else if (selectionType == "varName") {
        if (synonym->getSynonymType() == "statement") {
            return std::make_shared<StatementVariableNameAttribute>(
                    std::dynamic_pointer_cast<StatementWithVariableName>(synonym));
        } else {
            return std::make_shared<EntityVariableNameAttribute>(
                    std::dynamic_pointer_cast<EntityWithVariableName>(synonym));
        }
    } else if (selectionType == "procName") {
        if (synonym->getSynonymType() == "statement") {
            return std::make_shared<StatementProcedureNameAttribute>(
                    std::dynamic_pointer_cast<StatementWithProcedureName>(synonym));
        } else {
            return std::make_shared<EntityProcedureNameAttribute>(
                    std::dynamic_pointer_cast<EntityWithProcedureName>(synonym));
        }
    } else if (selectionType == "stmt#") {
        if (synonym->getSynonymType() == "statement") {
            return std::make_shared<StatementStatementNumberAttribute>(
                    std::dynamic_pointer_cast<StatementTypeSynonym>(synonym));
        } else {
            return std::make_shared<EntityStatementNumberAttribute>(
                    std::dynamic_pointer_cast<EntityWithStatementNumber>(synonym));
        }
    } else if (selectionType == "value") {
        if (synonym->getSynonymType() == "statement") {
            return std::make_shared<StatementValueAttribute>(
                    std::dynamic_pointer_cast<StatementWithValue>(synonym));
        } else {
            return std::make_shared<EntityValueAttribute>(
                    std::dynamic_pointer_cast<EntityWithValue>(synonym));
        }
    }
    return nullptr;
}

std::shared_ptr<WithRef>
TokenGroupExtractor::createWithRef(const std::string &ref, const std::string &attrRef, bool refIsInteger) {
    if (attrRef.empty()) {
        return std::make_shared<ConstantWithRef>(refIsInteger ? ref : ref.substr(1, ref.size() - 2));
    }

    auto synonym = synonymMap.at(ref);
    auto attribute = createSelectable(synonym, attrRef);
    if (synonym->getSynonymType() == "statement") {
        return std::make_shared<StatementAttributeRef>(std::dynamic_pointer_cast<IStatementAttribute>(attribute));
    } else {
        return std::make_shared<EntityAttributeRef>(std::dynamic_pointer_cast<IEntityAttribute>(attribute));
    }
}

void TokenGroupExtractor::visit(const std::shared_ptr<DeclarationTokenGroup> &declarationTokenGroup, Query& q) {
    auto tokens = declarationTokenGroup->getStringifiedTokens();
    auto type = tokens[0];

    // every other token skips the commas
    for (size_t i = 1; i < tokens.size(); i = i + 2) {
        auto name = tokens[i];
        auto syn = createSynonym(type, name);
        synonymMap.insert({name, syn});
        q.addSynonym(syn);
    }
}

void TokenGroupExtractor::visit(const std::shared_ptr<SelectTokenGroup> &selectTokenGroup, Query& q) {
    auto tokens = selectTokenGroup->getStringifiedTokens();
    std::vector<std::shared_ptr<ISelectable>> syn;
    for (const auto& synonymAndAttr : selectTokenGroup->synonymsAndAttributes) {
        auto& synonym = synonymMap.at(synonymAndAttr.first);
        auto attributeName = synonymAndAttr.second;
        syn.push_back(createSelectable(synonym, attributeName));
    }
    auto sel = QuerySelection(syn);
    q.setSelection(sel);
}

void TokenGroupExtractor::visit(const std::shared_ptr<UsesTokenGroup>& usesTokenGroup, Query& q) {
    auto type = usesTokenGroup->type;
    auto arg1 = usesTokenGroup->arg1;
    auto arg2 = usesTokenGroup->arg2;
    bool isNegated = usesTokenGroup->isNegated;

    std::shared_ptr<ISynonym> arg1Synonym = usesTokenGroup->arg1IsSynonym ? synonymMap[arg1] : nullptr;
    bool arg1IsStatement = usesTokenGroup->arg1IsNumber || (usesTokenGroup->arg1IsSynonym &&
                           arg1Synonym->getSynonymType() == "statement");
    type += arg1IsStatement ? "S" : "P";

    std::shared_ptr<ISynonym> arg2Synonym = usesTokenGroup->arg2IsSynonym ? synonymMap[arg2] : nullptr;

    std::shared_ptr<SuchThat> st;
    auto s = std::make_shared<std::set<std::string>>();

    std::shared_ptr<IVariable> var;
    if (usesTokenGroup->arg2IsString) {
        var = std::make_shared<VariableName>(arg2.substr(1, arg2.size() - 2));
    } else if (usesTokenGroup->arg2IsWildcard) {
        var = std::make_shared<VariableWildcard>();
    } else {
        var = std::dynamic_pointer_cast<VariableSynonym>(arg2Synonym);
        s->insert(arg2);
    }

    if (type == "UsesS") {
        std::shared_ptr<IStatement> statement;
        if (usesTokenGroup->arg1IsNumber) {
            statement = std::make_shared<StatementLineNumber>(stoi(arg1));
        } else {
            statement = std::dynamic_pointer_cast<StatementTypeSynonym>(arg1Synonym);
            s->insert(arg1);
        }
        st = std::make_shared<UsesStatement>(statement, var, isNegated);
    } else {
        std::shared_ptr<IProcedure> procedure;
        if (usesTokenGroup->arg1IsString) {
            procedure = std::make_shared<ProcedureName>(arg1.substr(1, arg1.size() - 2));
        } else {
            procedure = std::dynamic_pointer_cast<ProcedureSynonym>(arg1Synonym);
            s->insert(arg1);
        }
        st = std::make_shared<UsesProcedure>(procedure, var, isNegated);
    }
    st->synonym = s;
    q.addClause(st);
}

void TokenGroupExtractor::visit(const std::shared_ptr<ModifiesTokenGroup> &modifiesTokenGroup, Query &q) {
    auto type = modifiesTokenGroup->type;
    auto arg1 = modifiesTokenGroup->arg1;
    auto arg2 = modifiesTokenGroup->arg2;
    bool isNegated = modifiesTokenGroup->isNegated;

    std::shared_ptr<ISynonym> arg1Synonym = modifiesTokenGroup->arg1IsSynonym ? synonymMap[arg1] : nullptr;
    bool arg1IsStatement = modifiesTokenGroup->arg1IsNumber || (modifiesTokenGroup->arg1IsSynonym &&
                           arg1Synonym->getSynonymType() == "statement");
    type += arg1IsStatement ? "S" : "P";

    std::shared_ptr<ISynonym> arg2Synonym = modifiesTokenGroup->arg2IsSynonym ? synonymMap[arg2] : nullptr;

    std::shared_ptr<SuchThat> st;
    auto s = std::make_shared<std::set<std::string>>();

    std::shared_ptr<IVariable> var;
    if (modifiesTokenGroup->arg2IsString) {
        var = std::make_shared<VariableName>(arg2.substr(1, arg2.size() - 2));
    } else if (modifiesTokenGroup->arg2IsWildcard) {
        var = std::make_shared<VariableWildcard>();
    } else {
        var = std::dynamic_pointer_cast<VariableSynonym>(arg2Synonym);
        s->insert(arg2);
    }

    if (type == "ModifiesS") {
        std::shared_ptr<IStatement> statement;
        if (modifiesTokenGroup->arg1IsNumber) {
            statement = std::make_shared<StatementLineNumber>(stoi(arg1));
        } else {
            statement = std::dynamic_pointer_cast<StatementTypeSynonym>(arg1Synonym);
            s->insert(arg1);
        }
        st = std::make_shared<ModifiesStatement>(statement, var, isNegated);
    } else {
        std::shared_ptr<IProcedure> procedure;
        if (modifiesTokenGroup->arg1IsString) {
            procedure = std::make_shared<ProcedureName>(arg1.substr(1, arg1.size() - 2));
        } else {
            procedure = std::dynamic_pointer_cast<ProcedureSynonym>(arg1Synonym);
            s->insert(arg1);
        }
        st = std::make_shared<ModifiesProcedure>(procedure, var, isNegated);
    }
    st->synonym = s;
    q.addClause(st);
}

void TokenGroupExtractor::visit(const std::shared_ptr<FollowsTokenGroup> &followsTokenGroup, Query &q) {
    auto type = followsTokenGroup->type;
    auto arg1 = followsTokenGroup->arg1;
    auto arg2 = followsTokenGroup->arg2;
    bool isNegated = followsTokenGroup->isNegated;

    std::shared_ptr<ISynonym> arg1Synonym = followsTokenGroup->arg1IsSynonym ? synonymMap[arg1] : nullptr;

    std::shared_ptr<ISynonym> arg2Synonym = followsTokenGroup->arg2IsSynonym ? synonymMap[arg2] : nullptr;

    std::shared_ptr<SuchThat> st;
    auto s = std::make_shared<std::set<std::string>>();

    std::shared_ptr<IStatement> statement1;
    if (followsTokenGroup->arg1IsNumber) {
        statement1 = std::make_shared<StatementLineNumber>(stoi(arg1));
    } else if (followsTokenGroup->arg1IsWildcard) {
        statement1 = std::make_shared<StatementWildcard>();
    } else {
        statement1 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg1Synonym);
        s->insert(arg1);
    }

    std::shared_ptr<IStatement> statement2;
    if (followsTokenGroup->arg2IsNumber) {
        statement2 = std::make_shared<StatementLineNumber>(stoi(arg2));
    } else if (followsTokenGroup->arg2IsWildcard) {
        statement2 = std::make_shared<StatementWildcard>();
    } else {
        statement2 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg2Synonym);
        s->insert(arg2);
    }

    st = std::make_shared<Follows>(statement1, statement2, type.back() == '*', isNegated);
    st->synonym = s;
    q.addClause(st);
}

void TokenGroupExtractor::visit(const std::shared_ptr<ParentTokenGroup> &parentTokenGroup, Query &q) {
    auto type = parentTokenGroup->type;
    auto arg1 = parentTokenGroup->arg1;
    auto arg2 = parentTokenGroup->arg2;
    auto isNegated = parentTokenGroup->isNegated;

    std::shared_ptr<ISynonym> arg1Synonym = parentTokenGroup->arg1IsSynonym ? synonymMap[arg1] : nullptr;

    std::shared_ptr<ISynonym> arg2Synonym = parentTokenGroup->arg2IsSynonym ? synonymMap[arg2] : nullptr;

    std::shared_ptr<SuchThat> st;
    auto s = std::make_shared<std::set<std::string>>();

    std::shared_ptr<IStatement> statement1;
    if (parentTokenGroup->arg1IsNumber) {
        statement1 = std::make_shared<StatementLineNumber>(stoi(arg1));
    } else if (parentTokenGroup->arg1IsWildcard) {
        statement1 = std::make_shared<StatementWildcard>();
    } else {
        statement1 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg1Synonym);
        s->insert(arg1);
    }

    std::shared_ptr<IStatement> statement2;
    if (parentTokenGroup->arg2IsNumber) {
        statement2 = std::make_shared<StatementLineNumber>(stoi(arg2));
    } else if (parentTokenGroup->arg2IsWildcard) {
        statement2 = std::make_shared<StatementWildcard>();
    } else {
        statement2 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg2Synonym);
        s->insert(arg2);
    }

    st = std::make_shared<Parent>(statement1, statement2, type.back() == '*', isNegated);
    st->synonym = s;
    q.addClause(st);
}

void TokenGroupExtractor::visit(const std::shared_ptr<PatternTokenGroup> &patternTokenGroup, Query& q) {
    auto synonym = patternTokenGroup->statement;
    auto variable = patternTokenGroup->entity;
    auto expression = patternTokenGroup->expression;
    auto hasWildcard = patternTokenGroup->hasWildcard;
    auto isNegated = patternTokenGroup->isNegated;
    auto type = patternTokenGroup->type;

    std::shared_ptr<IVariable> variableEntity = nullptr;
    auto s = std::make_shared<std::set<std::string>>();
    s->insert(synonym);
    if (variable == "_") {
        variableEntity = std::make_shared<VariableWildcard>();
    } else if (variable[0] == '"') {
        variableEntity = std::make_shared<VariableName>(variable.substr(1, variable.size() - 2));
    } else {
        variableEntity = std::dynamic_pointer_cast<VariableSynonym>(synonymMap[variable]);
        s->insert(variable);
    }

    if (type == "assign") {
        std::shared_ptr<AssignSynonym> assignSynonym = std::dynamic_pointer_cast<AssignSynonym>(synonymMap[synonym]);
        std::shared_ptr<AssignPattern> pattern = std::make_shared<AssignPattern>(assignSynonym, variableEntity,
                                                                                 expression, hasWildcard, isNegated);
        pattern->synonym = s;
        q.addClause(pattern);
    } else if (type == "while") {
        std::shared_ptr<WhileSynonym> whileSynonym = std::dynamic_pointer_cast<WhileSynonym>(synonymMap[synonym]);
        std::shared_ptr<WhilePattern> pattern = std::make_shared<WhilePattern>(whileSynonym, variableEntity, isNegated);
        pattern->synonym = s;
        q.addClause(pattern);
    } else {
        std::shared_ptr<IfSynonym> ifSynonym = std::dynamic_pointer_cast<IfSynonym>(synonymMap[synonym]);
        std::shared_ptr<IfPattern> pattern = std::make_shared<IfPattern>(ifSynonym, variableEntity, isNegated);
        pattern->synonym = s;
        q.addClause(pattern);
    }
}

void TokenGroupExtractor::visit(const std::shared_ptr<CallsTokenGroup> &callsTokenGroup, Query &q) {
    auto type = callsTokenGroup->type;
    auto arg1 = callsTokenGroup->arg1;
    auto arg2 = callsTokenGroup->arg2;
    auto isNegated = callsTokenGroup->isNegated;

    std::shared_ptr<ISynonym> arg1Synonym = callsTokenGroup->arg1IsSynonym ? synonymMap[arg1] : nullptr;

    std::shared_ptr<ISynonym> arg2Synonym = callsTokenGroup->arg2IsSynonym ? synonymMap[arg2] : nullptr;

    std::shared_ptr<SuchThat> st;
    auto s = std::make_shared<std::set<std::string>>();

    std::shared_ptr<IProcedure> procedure1;
    if (callsTokenGroup->arg1IsString) {
        procedure1 = std::make_shared<ProcedureName>(arg1.substr(1, arg1.size() - 2));
    } else if (callsTokenGroup->arg1IsWildcard) {
        procedure1 = std::make_shared<ProcedureWildcard>();
    } else {
        procedure1 = std::dynamic_pointer_cast<ProcedureSynonym>(arg1Synonym);
        s->insert(arg1);
    }

    std::shared_ptr<IProcedure> procedure2;
    if (callsTokenGroup->arg2IsString) {
        procedure2 = std::make_shared<ProcedureName>(arg2.substr(1, arg2.size() - 2));
    } else if (callsTokenGroup->arg2IsWildcard) {
        procedure2 = std::make_shared<ProcedureWildcard>();
    } else {
        procedure2 = std::dynamic_pointer_cast<ProcedureSynonym>(arg2Synonym);
        s->insert(arg2);
    }

    st = std::make_shared<Calls>(procedure1, procedure2, type.back() == '*', isNegated);
    st->synonym = s;
    q.addClause(st);
}

void TokenGroupExtractor::visit(const std::shared_ptr<NextTokenGroup> &nextTokenGroup, Query &q) {
    auto type = nextTokenGroup->type;
    auto arg1 = nextTokenGroup->arg1;
    auto arg2 = nextTokenGroup->arg2;
    bool isNegated = nextTokenGroup->isNegated;

    std::shared_ptr<ISynonym> arg1Synonym = nextTokenGroup->arg1IsSynonym ? synonymMap[arg1] : nullptr;

    std::shared_ptr<ISynonym> arg2Synonym = nextTokenGroup->arg2IsSynonym ? synonymMap[arg2] : nullptr;

    std::shared_ptr<SuchThat> st;
    auto s = std::make_shared<std::set<std::string>>();

    std::shared_ptr<IStatement> stmt1;

    std::shared_ptr<IStatement> stmt2;

    if (nextTokenGroup->arg1IsNumber) {
        stmt1 = std::make_shared<StatementLineNumber>(stoi(arg1));
    } else if (nextTokenGroup->arg1IsWildcard) {
        stmt1 = std::make_shared<StatementWildcard>();
    } else {
        stmt1 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg1Synonym);
        s->insert(arg1);
    }

    if (nextTokenGroup->arg2IsNumber) {
        stmt2 = std::make_shared<StatementLineNumber>(stoi(arg2));
    } else if (nextTokenGroup->arg2IsWildcard) {
        stmt2 = std::make_shared<StatementWildcard>();
    } else {
        stmt2 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg2Synonym);
        s->insert(arg2);
    }

    st = std::make_shared<Next>(stmt1, stmt2, type.back() == '*', isNegated);
    st->synonym = s;
    q.addClause(st);
}

void TokenGroupExtractor::visit(const std::shared_ptr<WithTokenGroup> &withTokenGroup, Query &q) {
    auto s = std::make_shared<std::set<std::string>>();
    if (withTokenGroup->firstRefAttrName.length() > 0) {
        s->insert(withTokenGroup->firstRef);
    }
    if (withTokenGroup->secondRefAttrName.length() > 0) {
        s->insert(withTokenGroup->secondRef);
    }
    auto firstRef = createWithRef(withTokenGroup->firstRef,
                                  withTokenGroup->firstRefAttrName,
                                  withTokenGroup->isFirstRefInteger);

    auto secondRef = createWithRef(withTokenGroup->secondRef,
                                   withTokenGroup->secondRefAttrName,
                                   withTokenGroup->isSecondRefInteger);

    auto with = std::make_shared<With>(firstRef, secondRef, withTokenGroup->isNegated);
    with->synonym = s;
    q.addClause(with);
}

void TokenGroupExtractor::visit(const std::shared_ptr<AffectsTokenGroup> &affectsTokenGroup, Query &q) {
    auto type = affectsTokenGroup->type;
    auto arg1 = affectsTokenGroup->arg1;
    auto arg2 = affectsTokenGroup->arg2;
    bool isNegated = affectsTokenGroup->isNegated;

    std::shared_ptr<ISynonym> arg1Synonym = affectsTokenGroup->arg1IsSynonym ? synonymMap[arg1] : nullptr;

    std::shared_ptr<ISynonym> arg2Synonym = affectsTokenGroup->arg2IsSynonym ? synonymMap[arg2] : nullptr;

    std::shared_ptr<SuchThat> st;
    auto s = std::make_shared<std::set<std::string>>();

    std::shared_ptr<IStatement> stmt1;

    std::shared_ptr<IStatement> stmt2;

    if (affectsTokenGroup->arg1IsNumber) {
        stmt1 = std::make_shared<StatementLineNumber>(stoi(arg1));
    } else if (affectsTokenGroup->arg1IsWildcard) {
        stmt1 = std::make_shared<StatementWildcard>();
    } else {
        stmt1 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg1Synonym);
        s->insert(arg1);
    }

    if (affectsTokenGroup->arg2IsNumber) {
        stmt2 = std::make_shared<StatementLineNumber>(stoi(arg2));
    } else if (affectsTokenGroup->arg2IsWildcard) {
        stmt2 = std::make_shared<StatementWildcard>();
    } else {
        stmt2 = std::dynamic_pointer_cast<StatementTypeSynonym>(arg2Synonym);
        s->insert(arg2);
    }

    st = std::make_shared<Affects>(stmt1, stmt2, isNegated);
    st->synonym = s;
    q.addClause(st);
}

void TokenGroupExtractor::visit(const std::shared_ptr<InvalidClauseTokenGroup> &declarationTokenGroup, Query& q) {
    // should never happen
}
