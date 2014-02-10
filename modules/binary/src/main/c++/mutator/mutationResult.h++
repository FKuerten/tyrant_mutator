#ifndef TYRANT_MUTATOR_MUTATOR_MUTATIONRESULT_HPP
#define TYRANT_MUTATOR_MUTATOR_MUTATIONRESULT_HPP

    #include <set>
    #include <core/deckTemplate.h++>

    namespace Tyrant {
        namespace Mutator {

            typedef std::set<TyrantCache::Core::DeckTemplate::Ptr> DeckSet;

            struct MutationResult {
                DeckSet decks;
            };

        }
    }

#endif


