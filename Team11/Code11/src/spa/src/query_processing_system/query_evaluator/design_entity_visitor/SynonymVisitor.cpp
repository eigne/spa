#include "SynonymVisitor.h"

SynonymVisitor::SynonymVisitor(const std::shared_ptr<ISynonymReader> &synonymReader) {
    this->synonymReader = synonymReader;
    StatementWildcard::allPossibleStatements = synonymReader->getAllStatements();
    ProcedureWildcard::allPossibleProcedures = synonymReader->getAllProcedures();
    VariableWildcard::allPossibleVariables = synonymReader->getAllVariables();
}

void SynonymVisitor::visit(const std::shared_ptr<ConstantSynonym>& constantSynonym) {
    auto constants = synonymReader->getAllConstants();
    constantSynonym->setNames(constants);
    std::unordered_map<std::string, std::string> valueMap;
    for (const auto& constant : constants) {
        valueMap.insert({constant, constant});
    }
    constantSynonym->setValueAttributes(valueMap);
}

void SynonymVisitor::visit(const std::shared_ptr<ProcedureSynonym>& procedureSynonym) {
    auto procedures = synonymReader->getAllProcedures();
    procedureSynonym->setNames(procedures);
    std::unordered_map<std::string, std::string> procedureNameMap;
    for (const auto& procedure : procedures) {
        procedureNameMap.insert({procedure, procedure});
    }
    procedureSynonym->setProcedureNameAttributes(procedureNameMap);
}

void SynonymVisitor::visit(const std::shared_ptr<VariableSynonym>& variableSynonym) {
    auto variables = synonymReader->getAllVariables();
    variableSynonym->setNames(variables);
    std::unordered_map<std::string, std::string> variableNameMap;
    for (const auto& variable : variables) {
        variableNameMap.insert({variable, variable});
    }
    variableSynonym->setVariableNameAttributes(variableNameMap);
}

void SynonymVisitor::visit(const std::shared_ptr<AssignSynonym>& assignSynonym) {
    auto assigns = synonymReader->getAllAssigns();
    assignSynonym->setStatementNumbers(assigns);
}

void SynonymVisitor::visit(const std::shared_ptr<CallSynonym>& callSynonym) {
    auto calls = synonymReader->getAllCalls();
    callSynonym->setStatementNumbers(calls);
    std::unordered_map<int, std::string> procedureMap;
    for (auto statement : calls) {
        auto procedure = synonymReader->getProcedureCalledByCall(statement);
        procedureMap.insert({statement, procedure});
    }
    callSynonym->setProcedureNameAttributes(procedureMap);
}

void SynonymVisitor::visit(const std::shared_ptr<IfSynonym>& ifSynonym) {
    auto ifs = synonymReader->getAllIfs();
    ifSynonym->setStatementNumbers(ifs);
}

void SynonymVisitor::visit(const std::shared_ptr<PrintSynonym>& printSynonym) {
    auto prints = synonymReader->getAllPrints();
    printSynonym->setStatementNumbers(prints);
    std::unordered_map<int, std::string> variableMap;
    for (auto statement : prints) {
        auto variable = *(synonymReader->getVariablesUsedInStatement(statement).begin());
        variableMap.insert({statement, variable});
    }
    printSynonym->setVariableNameAttributes(variableMap);
}

void SynonymVisitor::visit(const std::shared_ptr<ReadSynonym>& readSynonym) {
    auto reads = synonymReader->getAllReads();
    readSynonym->setStatementNumbers(reads);
    std::unordered_map<int, std::string> variableMap;
    for (auto statement : reads) {
        auto variable = *(synonymReader->getVariablesModifiedInStatement(statement).begin());
        variableMap.insert({statement, variable});
    }
    readSynonym->setVariableNameAttributes(variableMap);
}

void SynonymVisitor::visit(const std::shared_ptr<StatementSynonym>& statementSynonym) {
    auto statements = synonymReader->getAllStatements();
    statementSynonym->setStatementNumbers(statements);
}

void SynonymVisitor::visit(const std::shared_ptr<WhileSynonym>& whileSynonym) {
    auto whiles = synonymReader->getAllWhiles();
    whileSynonym->setStatementNumbers(whiles);
}
