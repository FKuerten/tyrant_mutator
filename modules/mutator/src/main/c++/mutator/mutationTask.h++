#ifndef TYRANT_MUTATOR_MUTATOR_MUTATIONTASK_HPP
#define TYRANT_MUTATOR_MUTATOR_MUTATIONTASK_HPP

    #include <core/deckTemplate.h++>

    namespace Tyrant {
        namespace Mutator {

            class MutationTask {
                public:
                    Tyrant::Core::DeckTemplate::Ptr baseDeck;
                    unsigned int mutationLevel;
                    bool onlyAutoDecks;
                    bool restrictToOwnedCards;
                    unsigned int allowExtraCards;

                public:
                    MutationTask();

                    bool mutateUnorder() const;
                    bool mutateChangeCommander() const;
                    bool mutateRemoveCard() const;
                    bool mutateAddCard() const;
                    bool mutateSwapCard() const;
                    bool mutateReplaceCard() const;
                    bool mutateOrder() const;

                friend bool operator==(MutationTask const & lhs, MutationTask const & rhs);
            };

            bool operator==(MutationTask const & lhs, MutationTask const & rhs);
            bool operator!=(MutationTask const & lhs, MutationTask const & rhs);

        }
    }

#endif

