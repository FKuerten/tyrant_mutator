#ifndef TYRANT_MUTATOR_MUTATOR_CARDCHANGEMUTATOR_HPP
#define TYRANT_MUTATOR_MUTATOR_CARDCHANGEMUTATOR_HPP

    #include <memory>
    #include "mutator.h++"
    #include <core/staticDeckTemplate.h++>
    #include <set>
    #include <core/cards/cardLoader.h++>

    namespace Tyrant {
        namespace Mutator {

            class CardChangeMutator : public Mutator {
                public:
                    typedef std::shared_ptr<CardChangeMutator> Ptr;

                private:
                    bool aborted;
                    Tyrant::Core::Cards::Cards cardDB;
                    std::set<unsigned int> allowedCommanders;
                    std::multiset<unsigned int> allowedNonCommanderCards;

                public:
                    CardChangeMutator();
                    virtual ~CardChangeMutator();
                    virtual MutationResult mutate(MutationTask const & task);
                    virtual void abort();

                private:
                    void mutateOne(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);
                    void mutateUnorder(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);
                    void mutateChangeCommander(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);
                    void mutateRemoveCard(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);
                    void mutateAddCard(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);
                    void mutateSwapCard(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);
                    void mutateReplaceCard(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);
                    void mutateOrder(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations);

                    bool isValid(Core::StaticDeckTemplate const & deck);
                    bool canCompose(Core::StaticDeckTemplate const & deck);

            };

        }
    }

#endif

