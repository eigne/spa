#pragma once

#include <memory>
#include "program_knowledge_base/facades/PKBSP.h"
#include "program_knowledge_base/facades/PKBQPS.h"

class PKBFactory {
public:
    static std::pair<PKBSP, PKBQPS> createPKB();
};
