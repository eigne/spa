#pragma once

#include "program_knowledge_base/facades/PKBSP.h"
#include "program_knowledge_base/facades/PKBQPS.h"
#include "program_knowledge_base/PKBFactory.h"

static std::pair<PKBSP, PKBQPS> createPKBPair() {
    auto PKBPair{ PKBFactory::createPKB() };
    auto sp{ PKBPair.first };
    auto qps{ PKBPair.second };
    return std::make_pair(sp, qps);
}