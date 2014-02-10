#include "cardChangeMutator.h++"
#include <errorHandling/exceptions.h++>

namespace Tyrant {
    namespace Mutator {

        CardChangeMutator::~CardChangeMutator()
        {
        }

        MutationResult
        CardChangeMutator::mutate(MutationTask const & task)
        {
            throw LogicError("Not implemented.");
        }

        void
        CardChangeMutator::abort()
        {
            throw LogicError("Not implemented.");
        }

    }
}

