#ifndef TYRANT_MUTATOR_CARDCHANGEMUTATORGENERATOR_HPP
    #define TYRANT_MUTATOR_CARDCHANGEMUTATORGENERATOR_HPP

    #include "cardChangeMutator.forward.h++"

    namespace Tyrant {
        namespace Mutator {

            template<class Iterator>
            class CardChangeMutatorGenerator {
            public: // types
                enum Stages {
                    SETUP,
                    UNORDER,
                    CHANGE_COMMANDER_SETUP,
                    CHANGE_COMMANDER_EXECUTION,
                    REMOVE_CARD_SETUP,
                    REMOVE_CARD_EXECUTION,
                    ADD_CARD_SETUP,
                    ADD_CARD_UNORDERED_EXECUTION,
                    ADD_CARD_ORDERED_SETUP,
                    ADD_CARD_ORDERED_EXECUTION,
                    SWAP_CARD_SETUP,
                    SWAP_CARD_EXECUTION_1,
                    SWAP_CARD_EXECUTION_2,
                    REPLACE_CARD_SETUP,
                    REPLACE_CARD_EXECUTION_1,
                    REPLACE_CARD_EXECUTION_2,
                    ORDER,
                    STEP,
                };

                typedef Core::DeckTemplate::Ptr result_type;
            private: // variable
                CardChangeMutator::ConstPtr mutator;
                MutationTask const task;
                Iterator sourceCurrent;
                Iterator const sourceEnd;

                Stages stage0;
                std::set<unsigned int>::const_iterator stage1Iter;
                std::set<unsigned int>::const_iterator stage1IterEnd;
                unsigned int stage1Number;
                unsigned int stage1NumberEnd;
                unsigned int stage2Number;
                unsigned int stage2NumberEnd;

                result_type nextResult;
                bool _hasNext;

            private: // methods
                void findNext();

            public: // methods
                CardChangeMutatorGenerator
                        (CardChangeMutator::ConstPtr mutator
                        ,MutationTask const & task
                        ,Iterator const & sourceBegin
                        ,Iterator const & sourceEnd
                        );

                result_type operator()();
                bool hasNext() const;

        };

        }
    }

    #include "cardChangeMutatorGenerator.tpl.h++"
#endif
