#ifndef TYRANT_MUTATOR_MUTATOR_MUTATIONTASK_HPP
#define TYRANT_MUTATOR_MUTATOR_MUTATIONTASK_HPP

    #include <core/deckTemplate.h++>

    namespace Tyrant {
        namespace Mutator {

            struct MutationTask {
                Tyrant::Core::DeckTemplate::Ptr baseDeck;
            };

        }
    }

#endif

