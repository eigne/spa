#pragma once

#define SP_LOGGING_DISABLED
#define QPS_LOGGING_DISABLED

#ifdef SP_LOGGING_ENABLED
#warning "SP logging enabled. This prints all PKB calls to stderr, slowing execution."
#endif

#ifdef QPS_LOGGING_ENABLED
#warning "QPS logging enabled. This prints all PKB calls to stderr, slowing execution."
#endif
