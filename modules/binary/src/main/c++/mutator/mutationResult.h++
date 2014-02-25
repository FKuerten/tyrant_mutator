#ifndef TYRANT_MUTATOR_MUTATOR_MUTATIONRESULT_HPP
#define TYRANT_MUTATOR_MUTATOR_MUTATIONRESULT_HPP

    #include "mutatingIterator.h++"
    #include "iterator/uniqueIterator.h++"

    namespace Tyrant {
        namespace Mutator {

            typedef Praetorian::Basics::Iterator::UniqueIterator<MutatingIterator> DeckIterator;

            struct MutationResult {
                //CDeckSet decks;
                DeckIterator begin;
                DeckIterator end;


                MutationResult(DeckIterator & begin, DeckIterator & end);
            };

        }
    }

#endif


