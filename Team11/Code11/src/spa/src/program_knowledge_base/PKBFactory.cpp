#include "PKBFactory.h"

std::pair<PKBSP, PKBQPS> PKBFactory::createPKB() {
    auto PKBPointer{ std::make_shared<PKB>() };
    return std::pair<PKBSP, PKBQPS>{ PKBPointer, PKBPointer };
}
