#include "PKB.h"
#include "commons/ExpressionParser.h"
#include "program_knowledge_base/utility/PKBStatics.h"

// SP

/**
 * Inserts a read entity.
 * @param lineNumber The line number of the read statement.
 * @param entityRead The name of the variable read.
 */
void PKB::insertRead(int lineNumber, const std::string& entityRead) {
    entities.insertRead(lineNumber, entityRead);
}

/**
 * Inserts a print entity.
 * @param lineNumber The line number of the print statement.
 * @param entityPrinted The name of the variable printed.
 */
void PKB::insertPrint(int lineNumber, const std::string& entityPrinted) {
    entities.insertPrint(lineNumber, entityPrinted);
}

/**
 * Inserts an assign entity.
 * @param lineNumber The line number of the assign statement.
 * @param entityAssigned The name of the variable assigned to.
 */
void PKB::insertAssign(int lineNumber, const std::string& entityAssigned) {
    entities.insertAssign(lineNumber, entityAssigned);
}

/**
 * Inserts a while entity.
 * @param lineNumber The line number of the while condition.
 * @param entitiesInWhile The names of the variables contained in the condition. Do not include constants.
 *                        If the condition contains no variables, pass in an empty vector.
 */
void PKB::insertWhile(int lineNumber, const std::vector<std::string>& entitiesInWhile) {
    entities.insertWhile(lineNumber, entitiesInWhile);
}

/**
 * Inserts an if entity.
 * @param lineNumber The line number of the if condition statement.
 * @param entitiesInIf The names of the variables contained in the condition. Do not include constants.
 *                     If the condition contains no variables, pass in an empty vector.
 */
void PKB::insertIf(int lineNumber, const std::vector<std::string>& entitiesInIf) {
    entities.insertIf(lineNumber, entitiesInIf);
}

/**
 * Inserts a procedure entity.
 * @param procedureName The name of the procedure.
 */
void PKB::insertProcedure(const std::string& procedureName) {
    entities.insertProcedure(procedureName);
}

/**
 * Inserts a variable entity.
 * @param variable The name of the variable.
 */
void PKB::insertVariable(const std::string& variable) {
    entities.insertVariable(variable);
}

/**
 * Inserts a call entity.
 * @param lineNumber The line number where the call occurs.
 * @param procedureCalled The name of the called procedure.
 */
void PKB::insertCall(int lineNumber, const std::string& procedureCalled) {
    entities.insertCall(lineNumber, procedureCalled);
}

/**
 * Inserts a Calls relationship.
 * @param caller The name of the caller procedure.
 * @param procedureCalled The name of the called procedure.
 */
void PKB::insertCalls(const std::string& caller, const std::string& procedureCalled) {
    abstractions.insertCalls(caller, procedureCalled);
}

/**
 * Inserts a Calls* relationship.
 * @param caller The name of the caller procedure.
 * @param procedureCalled The name of the called procedure.
 */
void PKB::insertCallsStar(const std::string& caller, const std::string& procedureCalled) {
    abstractions.insertCallsStar(caller, procedureCalled);
}

/**
 * Inserts a constant entity.
 * @param constant The value of the constant.
 */
void PKB::insertConstant(const std::string& constant) {
    entities.insertConstant(constant);
}

/**
 * Inserts a Uses relationship.
 * @param lineNumber The line number where the relationship is present.
 * @param variable The name of the used variable.
 */
void PKB::insertUses(int lineNumber, const std::string &variable) {
    abstractions.insertUses(lineNumber, variable);
}

/**
 * Inserts a Uses relationship.
 * @param procedureName The procedure where the relationship is present.
 * @param variable The name of the used variable.
 */
void PKB::insertUses(const std::string& procedureName, const std::string& variable) {
    abstractions.insertUses(procedureName, variable);
}

/**
 * Inserts a Modifies relationship.
 * @param lineNumber The line number where the relationship is present.
 * @param variable The name of the modified variable.
 */
void PKB::insertModifies(int lineNumber, const std::string& variable) {
    abstractions.insertModifies(lineNumber, variable);
}
/**
 * Inserts a Modifies relationship.
 * @param procedureName The procedure where the relationship is present.
 * @param variable The name of the modified variable.
 */
void PKB::insertModifies(const std::string& procedureName, const std::string& variable) {
    abstractions.insertModifies(procedureName, variable);
}

/**
 * Inserts a Follows relationship.
 * @param before The preceding line number.
 * @param after The following line number.
 */
void PKB::insertFollows(int before, int after) {
    abstractions.insertFollows(before, after);
}

/**
 * Inserts a Follows* relationship.
 * @param before The previous line number.
 * @param after The following line number.
 */
void PKB::insertFollowsStar(int before, int after) {
    abstractions.insertFollowsStar(before, after);
}

/**
 * Inserts a Parents relationship.
 * @param parent The parent line number.
 * @param child The child line number.
 */
void PKB::insertParents(int parent, int child) {
    abstractions.insertParents(parent, child);
}

/**
 * Inserts a Parents* relationship.
 * @param parent The parent line number.
 * @param child The child line number.
 */
void PKB::insertParentsStar(int parent, int child) {
    abstractions.insertParentsStar(parent, child);
}

/**
 * Inserts a Next relationship.
 * @param previous The preceding line number.
 * @param next The succeeding line number
 */
void PKB::insertNext(int previous, int next) {
    abstractions.insertNext(previous, next);
}

/**
 * Inserts a pattern.
 * @param statementNumber The line number where the pattern is present.
 * @param postfixExpression The elements of the pattern in postfix format.
 */
void PKB::insertPattern(int statementNumber, const std::vector<std::string>& postfixExpression) {
    abstractions.insertPattern(statementNumber, postfixExpression);
}

/**
 * Queries if a procedure exists.
 * @param procedureName The name of the procedure.
 * @return true if the procedure exists,
 *         false if otherwise.
 */
bool PKB::containsProcedure(const std::string& procedureName) {
    return entities.containsProcedure(procedureName);
}

/**
 * Retrieves a copy of the Calls relationship table.
 * @return a map representing Calls.
 */
std::unordered_map<std::string, std::unordered_set<std::string> > PKB::getCallsMap() {
    return abstractions.getCallsMap();
}

/**
 * Retrieves a copy of the Calls* relationship table.
 * @return a map representing Calls*.
 */
std::unordered_map<std::string, std::unordered_set<std::string> > PKB::getCallsStarMap() {
    return abstractions.getCallsStarMap();
}

/**
 * Retrieves a copy of the inverse Calls relationship table.
 * @return a map representing inverse Calls.
 */
std::unordered_map<std::string, std::unordered_set<std::string> > PKB::getCallsReversedMap() {
    return abstractions.getCallsReversedMap();
}

/**
 * Retrieves a copy of the inverse Calls* relationship table.
 * @return a map representing inverse Calls*.
 */
std::unordered_map<std::string, std::unordered_set<std::string> > PKB::getCallsStarReversedMap() {
    return abstractions.getCallsStarReversedMap();
}

/**
 * Retrieves a copy of the Uses relationship table, filtered to only include procedures.
 * @return a map representing Uses (procedures only).
 */
std::unordered_map<std::string, std::unordered_set<std::string> > PKB::getUsesProcedureMap() {
    return abstractions.getUsesProcedureMap();
}

/**
 * Retrieves a copy of the Modifies relationship table, filtered to only include procedures.
 * @return a map representing Modifies (procedures only).
 */
std::unordered_map<std::string, std::unordered_set<std::string> > PKB::getModifiesProcedureMap() {
    return abstractions.getModifiesProcedureMap();
}

// QPS
/**
 * Retrieves all read entities.
 * @return A set containing all read line numbers.
 */
std::unordered_set<int> PKB::getAllReads() {
    return entities.getAllReads();
}

/**
 * Retrieves all print entities.
 * @return A set containing all print line numbers.
 */
std::unordered_set<int> PKB::getAllPrints() {
    return entities.getAllPrints();
}

/**
 * Retrieves all assign entities.
 * @return A set containing all assign line numbers.
 */
std::unordered_set<int> PKB::getAllAssigns() {
    return entities.getAllAssigns();
}

/**
 * Retrieves all while entities.
 * @return A set containing all while line numbers.
 */
std::unordered_set<int> PKB::getAllWhiles() {
    return entities.getAllWhiles();
}

/**
 * Retrieves all if entities.
 * @return A set containing all if line numbers.
 */
std::unordered_set<int> PKB::getAllIfs() {
    return entities.getAllIfs();
}

/**
 * Retrieves all statements.
 * @return A set containing all statement line numbers.
 */
std::unordered_set<int> PKB::getAllStatements() {
    return entities.getAllStatements();
}

/**
 * Retrieves all call entities.
 * @return A set containing all call line numbers.
 */
std::unordered_set<int> PKB::getAllCalls() {
    return entities.getAllCalls();
}

/**
 * Queries the Follows/Follows* relationship.
 * @param lineNumber The previous line number.
 * @param onlyImmediate If false, queries Follows*. Otherwise, queries Follows.
 * @return A set containing all line numbers that Follow/Follow* from lineNumber.
 */
std::unordered_set<int> PKB::getFollows(int lineNumber, bool onlyImmediate) {
    return abstractions.getFollows(lineNumber, onlyImmediate);
}

/**
 * Retrieves all procedure entities.
 * @return A set containing all procedures.
 */
std::unordered_set<std::string> PKB::getAllProcedures() {
    return entities.getAllProcedures();
}

/**
 * Retrieves all variable entities.
 * @return A set containing all variables.
 */
std::unordered_set<std::string> PKB::getAllVariables() {
    return entities.getAllVariables();
}

/**
 * Retrieves all constant entities.
 * @return A set containing all constants.
 */
std::unordered_set<std::string> PKB::getAllConstants() {
    return entities.getAllConstants();
}

/**
 * Queries the Uses relationship.
 * @param lineNumber The line number where the relationship appears.
 * @return A set of all variables used at lineNumber.
 */
std::unordered_set<std::string> PKB::getVariablesUsedInStatement(int lineNumber) {
    return abstractions.getVariablesUsedInStatement(lineNumber);
}

/**
 * Queries the Uses relationship.
 * @param procedureName The procedure where the relationship appears.
 * @return A set of all variables used by procedureName.
 */
std::unordered_set<std::string> PKB::getVariablesUsedInProcedure(const std::string& procedureName) {
    return abstractions.getVariablesUsedInProcedure(procedureName);
}

/**
 * Queries the inverse Uses relationship.
 * @param variable The variable used.
 * @return A set of all line numbers that use variable.
 */
std::unordered_set<int> PKB::getStatementsThatUseVariable(const std::string& variable) {
    return abstractions.getStatementsThatUseVariable(variable);
}

/**
 * Queries the Uses relationship.
 * @return A set of all line numbers that use any variable.
 */
std::unordered_set<int> PKB::getStatementsThatUseAnyVariable() {
    return abstractions.getStatementsThatUseAnyVariable();
}

/**
 * Queries the Uses relationship.
 * @param variable The variable used.
 * @return A set of all procedures that use variable.
 */
std::unordered_set<std::string> PKB::getProceduresThatUseVariable(const std::string& variable) {
    return abstractions.getProceduresThatUseVariable(variable);
}

/**
 * Queries the Uses relationship.
 * @return A set of all procedures that use any variable.
 */
std::unordered_set<std::string> PKB::getProceduresThatUseAnyVariable() {
    return abstractions.getProceduresThatUseAnyVariable();
}

/**
 * Queries the Modifies relationship.
 * @param lineNumber The line number where the relationship appears.
 * @return A set of all variables used at lineNumber.
 */
std::unordered_set<std::string> PKB::getVariablesModifiedInStatement(int lineNumber) {
    return abstractions.getVariablesModifiedInStatement(lineNumber);
}

/**
 * Queries the Modifies relationship.
 * @param procedureName The procedure where the relationship appears.
 * @return A set of all variables modified by procedureName.
 */
std::unordered_set<std::string> PKB::getVariablesModifiedInProcedure(const std::string& procedureName) {
    return abstractions.getVariablesModifiedInProcedure(procedureName);
}

/**
 * Queries the Modifies relationship.
 * @param variable The modified variable.
 * @return A set of all line numbers that modify variable.
 */
std::unordered_set<int> PKB::getStatementsThatModifyVariable(const std::string& variable) {
    return abstractions.getStatementsThatModifyVariable(variable);
}

/**
 * Queries the Modifies relationship.
 * @return A set of all line numbers that modify any variable.
 */
std::unordered_set<int> PKB::getStatementsThatModifyAnyVariable() {
    return abstractions.getStatementsThatModifyAnyVariable();
}

/**
 * Queries the Modifies relationship.
 * @param variable The modified variable.
 * @return A set of all procedures that modify variable.
 */
std::unordered_set<std::string> PKB::getProceduresThatModifyVariable(const std::string& variable) {
    return abstractions.getProceduresThatModifyVariable(variable);
}

/**
 * Queries the Modifies relationship.
 * @return A set of all procedures that modify any variable.
 */
std::unordered_set<std::string> PKB::getProceduresThatModifyAnyVariable() {
    return abstractions.getProceduresThatModifyAnyVariable();
}

/**
 * Queries the Follows relationship.
 * @return A set of all line numbers that have lines following them.
 */
std::unordered_set<int> PKB::getAllWithFollows() {
    return abstractions.getAllWithFollows();
}

/**
 * Queries the Follows relationship.
 * @return A set of all line numbers that follow any line.
 */
std::unordered_set<int> PKB::getAllWithBefore() {
    return abstractions.getAllWithBefore();
}

/**
 * Queries the Follows/Follows* relationship.
 * @param lineNumber The line number of the line in question.
 * @param onlyImmediate If false, queries Follows*. Otherwise, queries Follows.
 * @return A set of all line numbers that come before lineNumber.
 */
std::unordered_set<int> PKB::getBefore(int lineNumber, bool onlyImmediate) {
    return abstractions.getBefore(lineNumber, onlyImmediate);
}

/**
 * Queries the Parents/Parents* relationship.
 * @param lineNumber The line number of the parent.
 * @param onlyImmediate If false, queries Parents*. Otherwise, queries Parents.
 * @return A set of all line numbers that are children of lineNumber.
 */
std::unordered_set<int> PKB::getChild(int lineNumber, bool onlyImmediate) {
    return abstractions.getChild(lineNumber, onlyImmediate);
}

/**
 * Queries the Parents relationship.
 * @return A set of all line numbers with children.
 */
std::unordered_set<int> PKB::getAllWithChild() {
    return abstractions.getAllWithChild();
}

/**
 * Queries the Parents relationship.
 * @param lineNumber The line number of the child.
 * @param onlyImmediate If false, queries Parents*. Otherwise, queries Parents.
 * @return A set of all line numbers that are parents of lineNumber.
 */
std::unordered_set<int> PKB::getParent(int lineNumber, bool onlyImmediate) {
    return abstractions.getParent(lineNumber, onlyImmediate);
}

/**
 * Queries the Parents relationship.
 * @return A set of all line numbers that are children of any other line.
 */
std::unordered_set<int> PKB::getAllWithParent() {
    return abstractions.getAllWithParent();
}

/**
 * Queries the Calls/Calls* relationship.
 * @param procedure The caller procedure.
 * @param onlyImmediate If false, queries Calls*. Otherwise, queries Calls.
 * @return A set of all procedures called by procedure.
 */
std::unordered_set<std::string> PKB::getProceduresCalledBy(const std::string& procedure, bool onlyImmediate) {
    return abstractions.getProceduresCalledBy(procedure, onlyImmediate);
}

/**
 * Queries the Calls/Calls* relationship.
 * @param procedure The called procedure.
 * @param onlyImmediate If false, queries Calls*. Otherwise, queries Calls.
 * @return A set of all procedures that call procedure.
 */
std::unordered_set<std::string> PKB::getProceduresCalling(const std::string& procedure, bool onlyImmediate) {
    return abstractions.getProceduresCalling(procedure, onlyImmediate);
}

/**
 * Queries which call is executed at a specific line.
 * @param lineNumber The line number where the call occurs.
 * @return The name of the procedure called at lineNumber.
 */
std::string PKB::getProcedureCalledByCall(int lineNumber) {
    return entities.getProcedureCalledByCall(lineNumber);
}

/**
 * Queries if calls exist.
 * @return true if calls have been inserted, false otherwise.
 */
bool PKB::doCallsExist() {
    return entities.doCallsExist();
}

/**
 * Queries which variables are used in an if statement's condition.
 * @param lineNumber The line number of the condition.
 * @return A set of variables (not constants) used in the condition.
 */
std::unordered_set<std::string> PKB::getVariablesUsedInIf(int lineNumber) {
    return entities.getVariablesUsedInIf(lineNumber);
}

/**
 * Queries which if statements use a specific variable in their conditions.
 * @param variable The variable in question.
 * @return A set of all if statements that use variable in their conditions.
 */
std::unordered_set<int> PKB::getIfsThatUseVariable(const std::string& variable) {
    return entities.getIfsThatUseVariable(variable);
}

/**
 * Queries which if statements use variables in their conditions.
 * @return A set of all if statements that use any variable in their condition.
 */
std::unordered_set<int> PKB::getIfsThatUseAnyVariable() {
    return entities.getIfsThatUseAnyVariable();
}

/**
 * Queries which variables are used in an while statement's condition.
 * @param lineNumber The line number of the condition.
 * @return A set of variables (not constants) used in the condition.
 */
std::unordered_set<std::string> PKB::getVariablesUsedInWhile(int lineNumber) {
    return entities.getVariablesUsedInWhile(lineNumber);
}

/**
 * Queries which while statements use a specific variable in their conditions.
 * @param variable The variable in question.
 * @return A set of all while statements that use variable in their conditions.
 */
std::unordered_set<int> PKB::getWhilesThatUseVariable(const std::string& variable) {
    return entities.getWhilesThatUseVariable(variable);
}

/**
 * Queries which while statements use variables in their conditions.
 * @param variable The variable in question.
 * @return A set of all while statements that use any variable in their conditions.
 */
std::unordered_set<int> PKB::getWhilesThatUseAnyVariable() {
    return entities.getWhilesThatUseAnyVariable();
}

/**
 * Queries which procedures call other procedures.
 * @return A set of all procedures that call any procedure.
 */
std::unordered_set<std::string> PKB::getProceduresThatCall() {
    return abstractions.getProceduresThatCall();
}

/**
 * Queries which procedures are called by other procedures.
 * @return A set of all procedures called by any procedure.
 */
std::unordered_set<std::string> PKB::getAllCalledProcedures() {
    return abstractions.getAllCalledProcedures();
}

/**
 * Queries the Next relationship.
 * @param lineNumber The preceding line number.
 * @return A set of line numbers that execute after lineNumber.
 */
std::unordered_set<int> PKB::getNext(int lineNumber) {
    return abstractions.getNext(lineNumber);
}

/**
 * Queries the Next relationship.
 * @return A set of all line numbers that execute before any other line.
 */
std::unordered_set<int> PKB::getAllWithNext() {
    return abstractions.getAllWithNext();
}

/**
 * Queries the Next relationship.
 * @return A set of all line numbers that execute after any other line.
 */
std::unordered_set<int> PKB::getAllWithPrevious() {
    return abstractions.getAllWithPrevious();
}

/**
 * Queries the Next relationship.
 * @param lineNumber The succeeding line number.
 * @return A set of all line numbers that execute before lineNumber.
 */
std::unordered_set<int> PKB::getPrevious(int lineNumber) {
    return abstractions.getPrevious(lineNumber);
}

/**
 * Queries if a line has a particular pattern expression.
 * @param statementNumber The line number where the pattern is present.
 * @param postfixExpression A list containing the elements of the pattern to search for in postfix format.
 * @param exactMatch Searches for substrings and exact matches if false. Otherwise, only searches for exact matches.
 * @return true if the pattern exists, false otherwise.
 */
bool PKB::hasMatchingPattern(int statementNumber, const std::vector<std::string>& postfixExpression, bool exactMatch) {
    return abstractions.hasMatchingPattern(statementNumber, postfixExpression, exactMatch);
}

bool PKB::isSubarray(const std::vector<std::string> &array, const std::vector<std::string> &subarray, bool exactMatch) {
    return abstractions.isSubarray(array, subarray, exactMatch);
}
