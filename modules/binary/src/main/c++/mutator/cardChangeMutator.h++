#ifndef TYRANT_MUTATOR_MUTATOR_CARDCHANGEMUTATOR_HPP
#define TYRANT_MUTATOR_MUTATOR_CARDCHANGEMUTATOR_HPP

    #include "abstractMutator.h++"
    //#include <memory>
    //#include <core/staticDeckTemplate.h++>
    //#include <set>
    //#include <core/cards/cardLoader.h++>

    namespace Tyrant {
        namespace Mutator {

            template<class Iterator>
            class Generator;

            class CardChangeMutator : public AbstractMutator
                                    , public std::enable_shared_from_this<CardChangeMutator> {
                public:
                    typedef std::shared_ptr<CardChangeMutator> Ptr;
                    typedef std::shared_ptr<CardChangeMutator const> ConstPtr;

                private:
                    bool aborted;

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

                template<class Iterator>
                friend class Generator;
            };

        }
    }

#endif

