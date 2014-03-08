#ifndef TYRANT_MUTATOR_ABSTRACTMUTATOR_HPP
#define TYRANT_MUTATOR_ABSTRACTMUTATOR_HPP

    #include <memory>
    #include "mutator.h++"
    #include <core/staticDeckTemplate.h++>
    #include <set>
    #include <core/cards/cardLoader.h++>

    // for specific decks casting
    #include <core/autoDeckTemplate.h++>
    #include <core/simpleOrderedDeckTemplate.h++>

    namespace Tyrant {
        namespace Mutator {

            class AbstractMutator : public Mutator {
                public:
                    typedef std::shared_ptr<AbstractMutator> Ptr;

                protected:
                    Tyrant::Core::Cards::Cards cardDB;
                    std::set<unsigned int> allowedCommanders;
                    std::set<unsigned int> allowedNonCommanderCards;
                    std::multiset<unsigned int> allowedNonCommanderCardsWithCount;

                public:
                    AbstractMutator();
                    virtual ~AbstractMutator();
                    virtual MutationResult mutate(MutationTask const & task) = 0;
                    virtual void abort() = 0;

                private:
                    void initCardDB(Core::Cards::Cards const & cardDB);
                    void buildAllowedCards(std::multiset<unsigned int> const & ownedCards);

                public:
                    bool isValid(Core::StaticDeckTemplate const & deck) const;
                    bool canCompose(Core::StaticDeckTemplate const & deck) const;

                    static bool isOrdered(Core::StaticDeckTemplate const & deck);
                    static Core::SimpleOrderedDeckTemplate::Ptr asOrdered(Core::StaticDeckTemplate const & orig);
                    static Core::AutoDeckTemplate::Ptr asUnordered(Core::StaticDeckTemplate const & orig);

            };

        }
    }

#endif


