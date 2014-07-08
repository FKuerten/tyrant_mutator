#include "configuration.h++"
#include <mutator/cardChangeMutator.h++>

namespace TyrantMutator {
    namespace CLI {

        Tyrant::Mutator::Mutator::Ptr
        Configuration::constructMutator() const
        {
            return Tyrant::Mutator::Mutator::Ptr(
                new Tyrant::Mutator::CardChangeMutator()
            );
        }

    }
}
