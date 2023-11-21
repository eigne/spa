#pragma once

#include "ISynonymReader.h"
#include "IUsesStatementReader.h"
#include "ICallsReader.h"
#include "IUsesProcedureReader.h"
#include "IModifiesStatementReader.h"
#include "IModifiesProcedureReader.h"
#include "IParentReader.h"
#include "IFollowsReader.h"
#include "IAssignPatternReader.h"
#include "IIfPatternReader.h"
#include "IWhilePatternReader.h"
#include "INextReader.h"
#include "IAffectsReader.h"

class IStorageReader : public virtual ISynonymReader, public virtual IUsesStatementReader, public IUsesProcedureReader,
    public virtual IModifiesStatementReader, public IModifiesProcedureReader, public IParentReader,
    public IFollowsReader, public IAssignPatternReader, public ICallsReader,
    public IIfPatternReader, public IWhilePatternReader, public virtual INextReader, public IAffectsReader {
};
