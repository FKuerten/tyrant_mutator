#include "mutationTask.h++"

namespace Tyrant {
    namespace Mutator {

        static unsigned int const ONE_ROUND = 7;

        MutationTask::MutationTask()
        : mutationLevel{ONE_ROUND}
        , onlyAutoDecks{false}
        , restrictToOwnedCards(true)
        , allowExtraCards(0)
        {
        }

        bool MutationTask::mutateUnorder() const
        {
            return this->mutationLevel >= 1;
        }

        bool MutationTask::mutateChangeCommander() const
        {
            return this->mutationLevel >= 2;
        }

        bool MutationTask::mutateRemoveCard() const
        {
            return this->mutationLevel >= 3;
        }

        bool MutationTask::mutateAddCard() const
        {
            return this->mutationLevel >= 4;
        }

        bool MutationTask::mutateSwapCard() const
        {
            return this->mutationLevel >= 5;
        }

        bool MutationTask::mutateReplaceCard() const
        {
            return this->mutationLevel >= 6;
        }

        bool MutationTask::mutateOrder() const
        {
            return this->mutationLevel >= 7;
        }

        bool operator==(MutationTask const & lhs, MutationTask const & rhs)
        {
            return lhs.baseDeck             == rhs.baseDeck
                && lhs.mutationLevel        == rhs.mutationLevel
                && lhs.onlyAutoDecks        == rhs.onlyAutoDecks
                && lhs.restrictToOwnedCards == rhs.restrictToOwnedCards
                && lhs.allowExtraCards      == rhs.allowExtraCards;
        }

        bool operator!=(MutationTask const & lhs, MutationTask const & rhs)
        {
            return !(lhs == rhs);
        }

    }
}
