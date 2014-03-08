#include "mutationResult.h++"

namespace Tyrant {
    namespace Mutator {

        MutationResult::MutationResult(DeckIterator & begin
                                      ,DeckIterator & end
                                      )
        : begin(begin)
        , end(end)
        {
        }

    }
}
