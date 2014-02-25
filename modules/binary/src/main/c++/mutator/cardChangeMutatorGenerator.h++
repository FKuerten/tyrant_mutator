#ifndef TYRANT_MUTATOR_CARDCHANGEMUTATORGENERATOR_HPP
    #define TYRANT_MUTATOR_CARDCHANGEMUTATORGENERATOR_HPP

    #include "mutatingIterator.h++"
    #include "cardChangeMutator.h++"

    namespace Tyrant {
        namespace Mutator {

            class CardChangeMutatorGenerator : public MutatingIterator {
                public: // types
                    enum Stages {
                        SETUP, // 0
                        UNORDER,
                        CHANGE_COMMANDER_SETUP,
                        CHANGE_COMMANDER_EXECUTION,
                        REMOVE_CARD_SETUP,
                        REMOVE_CARD_EXECUTION,
                        ADD_CARD_SETUP,
                        ADD_CARD_UNORDERED_EXECUTION,
                        ADD_CARD_ORDERED_SETUP,
                        ADD_CARD_ORDERED_EXECUTION,
                        SWAP_CARD_SETUP, // 10
                        SWAP_CARD_EXECUTION_1,
                        SWAP_CARD_EXECUTION_2,
                        REPLACE_CARD_SETUP,
                        REPLACE_CARD_EXECUTION_1,
                        REPLACE_CARD_EXECUTION_2, // 15
                        ORDER,
                        STEP, // 17
                    };

                private: // variable
                    CardChangeMutator::ConstPtr mutator;
                    MutationTask const task;

                    Stages stage0;
                    std::set<unsigned int>::const_iterator stage1Iter;
                    std::set<unsigned int>::const_iterator stage1IterEnd;
                    unsigned int stage1Number;
                    unsigned int stage1NumberEnd;
                    unsigned int stage2Number;
                    unsigned int stage2NumberEnd;

                    value_type nextResult;
                    bool _hasNext;

                private: // methods
                    void findNext();

                protected:
                    virtual bool equals2(MutatingIterator const & rhs) const;
                    virtual bool equals2(CardChangeMutatorGenerator const & rhs) const;

                public: // methods
                    CardChangeMutatorGenerator
                            (CardChangeMutator::ConstPtr mutator
                            ,MutationTask const & task
                            ,CDeckSet const source
                            ,bool atEnd = false
                            );

                public:
                    CardChangeMutatorGenerator & operator++();
                    //CardChangeMutatorGenerator const & operator++(int);
                    value_type operator*() const;

                //friend bool operator==(CardChangeMutatorGenerator const & lhs, CardChangeMutatorGenerator const & rhs);
                //friend bool operator!=(CardChangeMutatorGenerator const & lhs, CardChangeMutatorGenerator const & rhs);
            };

            //bool operator==(CardChangeMutatorGenerator const & lhs, CardChangeMutatorGenerator const & rhs);
            //bool operator!=(CardChangeMutatorGenerator const & lhs, CardChangeMutatorGenerator const & rhs);

        }
    }

    #include "cardChangeMutatorGenerator.tpl.h++"
#endif
