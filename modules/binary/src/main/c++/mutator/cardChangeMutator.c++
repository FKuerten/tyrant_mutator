#include "cardChangeMutator.h++"
#include <errorHandling/exceptions.h++>
// for dynamic_pointer_cast
#include <memory>
#include <core/staticDeckTemplate.h++>

namespace Tyrant {
    namespace Mutator {

        CardChangeMutator::~CardChangeMutator()
        {
        }

        MutationResult
        CardChangeMutator::mutate(MutationTask const & task)
        {
            // We only mutate certain deck types
            if (Core::StaticDeckTemplate::ConstPtr baseDeck = std::dynamic_pointer_cast<Core::StaticDeckTemplate const>(task.baseDeck)) {
                throw LogicError("Not implemented.");
            } else {
                throw InvalidUserInputError("Not supported.");
            }
        }

        void
        CardChangeMutator::abort()
        {
            throw LogicError("Not implemented.");
        }

    }
}

