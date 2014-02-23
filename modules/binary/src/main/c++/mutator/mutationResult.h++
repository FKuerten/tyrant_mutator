#ifndef TYRANT_MUTATOR_MUTATOR_MUTATIONRESULT_HPP
#define TYRANT_MUTATOR_MUTATOR_MUTATIONRESULT_HPP

    #include <set>
    #include <core/deckTemplate.h++>
    #include "derefCompareLT.h++"

    namespace Tyrant {
        namespace Mutator {

            typedef std::set<::Tyrant::Core::DeckTemplate::Ptr, DerefCompareLT> DeckSet;
            typedef std::set<::Tyrant::Core::DeckTemplate::ConstPtr, DerefCompareLT> CDeckSet;

            struct MutationResult {
                CDeckSet decks;
            };

        }
    }

#endif


