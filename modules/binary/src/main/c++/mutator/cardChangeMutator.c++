#include "cardChangeMutator.h++"
#include <errorHandling/assert.h++>
#include <errorHandling/exceptions.h++>
#include <core/cards/cardLoader.h++>
// for dynamic_pointer_cast
#include <memory>
#include <list>
#include <set>
#include <iostream>
#include <string>
#include "derefCompareLT.h++"

namespace Tyrant {
    namespace Mutator {

        static size_t const DEFAULT_DECK_SIZE{10};

        CardChangeMutator::CardChangeMutator()
        {
            //std::clog << "CCM::CCM() commanders: " << this->allowedCommanders.size() << std::endl;
        }

        CardChangeMutator::~CardChangeMutator()
        {
        }

        template <class Iterator>
        class Generator {
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
                Generator(CardChangeMutator::ConstPtr mutator
                         ,MutationTask const & task
                         ,Iterator const & sourceBegin
                         ,Iterator const & sourceEnd
                         );

                result_type operator()();
                bool hasNext() const;

        };

        template <class Iterator>
        Generator<Iterator>::Generator(CardChangeMutator::ConstPtr mutator
                            ,MutationTask const & task
                            ,Iterator const & sourceBegin
                            ,Iterator const & sourceEnd
                            )
        : mutator(mutator)
        , task(task)
        , sourceCurrent(sourceBegin)
        , sourceEnd(sourceEnd)
        , stage0(SETUP)
        {
            //std::clog << "commanders: " << mutator->allowedCommanders.size() << std::endl;
            this->findNext();
        }

        template <class Iterator>
        void
        Generator<Iterator>::findNext()
        {
            while (true) {
                Core::StaticDeckTemplate const & currentBaseDeck = **(this->sourceCurrent);
                switch(this->stage0) {
                    case SETUP: // setup
                        //std::clog << "stage0 == SETUP" << std::endl;
                        if (this->sourceCurrent == this->sourceEnd) {
                            this->_hasNext = false;
                            return;
                        } else {
                            this->stage0 = UNORDER;
                        }
                    case UNORDER: // unorder
                        //std::clog << "stage0 == UNORDER" << std::endl;
                        if (this->task.mutateUnorder() && this->mutator->isOrdered(currentBaseDeck)) {
                            Core::StaticDeckTemplate::Ptr mutation = this->mutator->asUnordered(currentBaseDeck);
                            this->stage0 = CHANGE_COMMANDER_SETUP;
                            this->nextResult = mutation;
                            this->_hasNext = true;
                            return;
                        } else {
                            this->stage0 = CHANGE_COMMANDER_SETUP;
                        }
                    case CHANGE_COMMANDER_SETUP: // change commander: setup
                        //std::clog << "stage0 == CHANGE_COMMANDER_SETUP" << std::endl;
                        if (this->task.mutateChangeCommander()) {
                            //std::clog << "change commander setup" << std::endl;
                            this->stage1Iter = this->mutator->allowedCommanders.cbegin();
                            this->stage1IterEnd = this->mutator->allowedCommanders.cend();
                            //std::clog << "commanders: " << this->mutator->allowedCommanders.size() << std::endl;
                            this->stage0 = CHANGE_COMMANDER_EXECUTION;
                        } else {
                            this->stage0 = REMOVE_CARD_SETUP;
                            break;
                        }
                    case CHANGE_COMMANDER_EXECUTION: // change commander: execution
                        //std::clog << "stage0 == CHANGE_COMMANDER_EXECUTION" << std::endl;
                        assertX(this->task.mutateChangeCommander());
                        if (this->stage1Iter != this->stage1IterEnd) {
                            unsigned int const commanderId = *(this->stage1Iter);
                            //std::clog << "change commander: " << commanderId << std::endl;
                            Core::StaticDeckTemplate::Ptr mutation = currentBaseDeck.withCommander(commanderId);
                            //std::clog << "mutation generated " << std::string(*mutation) << std::endl;
                            if(this->mutator->isValid(*mutation) && this->mutator->canCompose(*mutation)) {
                                this->stage1Iter++;
                                this->nextResult = mutation;
                                this->_hasNext = true;
                                return;
                            } else {
                                std::clog << "invalid" << std::endl;
                                this->stage1Iter++;
                                break;
                            }
                        } else {
                            this->stage0 = REMOVE_CARD_SETUP;
                        }
                    case REMOVE_CARD_SETUP: // remove card: setup
                        //std::clog << "stage0 == REMOVE_CARD_SETUP" << std::endl;
                        if (this->task.mutateRemoveCard()) {
                            this->stage1Number = 0;
                            this->stage1NumberEnd = currentBaseDeck.getNumberOfNonCommanderCards();
                            this->stage0 = REMOVE_CARD_EXECUTION;
                        } else {
                            this->stage0 = ADD_CARD_SETUP;
                            break;
                        }
                    case REMOVE_CARD_EXECUTION: // remove card: execution
                        assertX(this->task.mutateRemoveCard());
                        if (this->stage1Number != this->stage1NumberEnd) {
                            unsigned int const i = this->stage1Number;
                            Core::StaticDeckTemplate::Ptr mutation = currentBaseDeck.withoutCardAtIndex(i);
                            this->stage1Number++;
                            this->nextResult = mutation;
                            this->_hasNext = true;
                            return;
                        } else {
                            this->stage0 = ADD_CARD_SETUP;
                        }
                    case ADD_CARD_SETUP: // add card: setup
                        //std::clog << "stage0 == ADD_CARD_SETUP" << std::endl;
                        if (this->task.mutateAddCard() && currentBaseDeck.getNumberOfNonCommanderCards() < DEFAULT_DECK_SIZE) {
                            this->stage1Iter = this->mutator->allowedNonCommanderCards.cbegin();
                            this->stage1IterEnd = this->mutator->allowedNonCommanderCards.cend();
                            if(this->mutator->isOrdered(currentBaseDeck)) {
                                this->stage2NumberEnd = currentBaseDeck.getNumberOfNonCommanderCards() + 1;
                                this->stage0 = ADD_CARD_ORDERED_SETUP;
                                break;
                            } else {
                                this->stage0 = ADD_CARD_UNORDERED_EXECUTION;
                                break;
                            }
                        } else {
                            this->stage0 = SWAP_CARD_SETUP;
                            break;
                        }
                    case ADD_CARD_UNORDERED_EXECUTION: // add card: execution
                        assertX(this->task.mutateAddCard() && currentBaseDeck.getNumberOfNonCommanderCards() < DEFAULT_DECK_SIZE);
                        if (this->stage1Iter != this->stage1IterEnd) {
                            unsigned int const cardId = *this->stage1Iter;
                            Core::StaticDeckTemplate::Ptr mutation = currentBaseDeck.withCard(cardId);
                            if(this->mutator->isValid(*mutation) && this->mutator->canCompose(*mutation)) {
                                this->stage1Iter++;
                                this->nextResult = mutation;
                                this->_hasNext = true;
                                return;
                            } else {
                                this->stage1Iter++;
                                break;
                            }
                        } else {
                            this->stage0 = SWAP_CARD_SETUP;
                            break;
                        }
                    case ADD_CARD_ORDERED_SETUP:
                        assertX(this->task.mutateAddCard() && currentBaseDeck.getNumberOfNonCommanderCards() < DEFAULT_DECK_SIZE);
                        if (this->stage1Iter != this->stage1IterEnd) {
                            this->stage2Number = 0;
                            this->stage0 = ADD_CARD_ORDERED_EXECUTION;
                            break;
                        } else {
                            this->stage0 = SWAP_CARD_SETUP;
                            break;
                        }
                    case ADD_CARD_ORDERED_EXECUTION:
                        assertX(this->task.mutateAddCard() && currentBaseDeck.getNumberOfNonCommanderCards() < DEFAULT_DECK_SIZE);
                        if (this->stage1Iter != this->stage1IterEnd) {
                            if (this->stage2Number != this->stage2NumberEnd) {
                                unsigned int const cardId = *this->stage1Iter;
                                unsigned int i = this->stage2Number;
                                Core::StaticDeckTemplate::Ptr mutation = currentBaseDeck.withCardAtIndex(cardId, i);
                                if(this->mutator->isValid(*mutation) && this->mutator->canCompose(*mutation)) {
                                    this->stage2Number++;
                                    this->nextResult = mutation;
                                    this->_hasNext = true;
                                    return;
                                } else {
                                    this->stage2Number++;
                                    break;
                                }
                            } else {
                                this->stage1Iter++;
                                this->stage0 = ADD_CARD_ORDERED_SETUP;
                                break;
                            }
                        } else {
                            this->stage0 = SWAP_CARD_SETUP;
                        }
                    case SWAP_CARD_SETUP:
                        //std::clog << "stage0 == SWAP_CARD_SETUP" << std::endl;
                        if (this->task.mutateAddCard() && this->mutator->isOrdered(currentBaseDeck)) {
                            this->stage1Number = 0;
                            this->stage1NumberEnd = currentBaseDeck.getNumberOfNonCommanderCards() - 1;
                            this->stage2NumberEnd = currentBaseDeck.getNumberOfNonCommanderCards();
                            this->stage0 = SWAP_CARD_EXECUTION_1;
                        } else {
                            this->stage0 = REPLACE_CARD_SETUP;
                            break;
                        }
                    case SWAP_CARD_EXECUTION_1:
                        assertX(this->task.mutateAddCard() && this->mutator->isOrdered(currentBaseDeck));
                        if (this->stage1Number != this->stage1NumberEnd) {
                            this->stage2Number = this->stage1Number + 1;
                            this->stage0 = SWAP_CARD_EXECUTION_2;
                        } else {
                            this->stage0 = REPLACE_CARD_SETUP;
                            break;
                        }
                    case SWAP_CARD_EXECUTION_2:
                        assertX(this->task.mutateAddCard() && this->mutator->isOrdered(currentBaseDeck));
                        assertX(this->stage1Number != this->stage1NumberEnd);
                        if (this->stage2Number != this->stage2NumberEnd) {
                            unsigned int const i = this->stage1Number;
                            unsigned int const j = this->stage2Number;
                            assertLT(i,j);
                            Core::StaticDeckTemplate::Ptr mutation = currentBaseDeck.withSwappedCards(i,j);
                            this->stage2Number++;
                            this->nextResult = mutation;
                            this->_hasNext = true;
                            return;
                        } else {
                            this->stage1Number++;
                            this->stage0 = SWAP_CARD_EXECUTION_1;
                            break;
                        }
                    case REPLACE_CARD_SETUP:
                        //std::clog << "stage0 == REPLACE_CARD_SETUP" << std::endl;
                        if (this->task.mutateReplaceCard()) {
                            this->stage1Iter = this->mutator->allowedNonCommanderCards.cbegin();
                            this->stage1IterEnd = this->mutator->allowedNonCommanderCards.cend();
                            this->stage2NumberEnd = currentBaseDeck.getNumberOfNonCommanderCards();
                            this->stage0 = REPLACE_CARD_EXECUTION_1;
                        } else {
                            this->stage0 = ORDER;
                            break;
                        }
                    case REPLACE_CARD_EXECUTION_1:
                        //std::clog << "stage0 == REPLACE_CARD_EXECUTION_1 " << *(this->stage1Iter) << std::endl;
                        assertX(this->task.mutateReplaceCard());
                        if (this->stage1Iter != this->stage1IterEnd) {
                            this->stage2Number = 0;
                            this->stage0 = REPLACE_CARD_EXECUTION_2;
                        } else {
                            this->stage0 = ORDER;
                            break;
                        }
                    case REPLACE_CARD_EXECUTION_2:
                        //std::clog << "stage0 == REPLACE_CARD_EXECUTION_2 " << this->stage2Number << std::endl;
                        assertX(this->task.mutateReplaceCard());
                        assertX(this->stage1Iter != this->stage1IterEnd);
                        if (this->stage2Number != this->stage2NumberEnd) {
                            unsigned int const cardId = *this->stage1Iter;
                            unsigned int i = this->stage2Number;
                            Core::StaticDeckTemplate::Ptr mutation = currentBaseDeck.withReplacedCardAtIndex(cardId, i);
                            if(this->mutator->isValid(*mutation) && this->mutator->canCompose(*mutation)) {
                                this->stage2Number++;
                                this->nextResult = mutation;
                                this->_hasNext = true;
                                return;
                            } else {
                                this->stage2Number++;
                                break;
                            }
                        } else {
                            this->stage1Iter++;
                            this->stage0 = REPLACE_CARD_EXECUTION_1;
                            break;
                        }
                    case ORDER:
                        //std::clog << "stage0 == ORDER" << std::endl;
                        if (this->task.mutateUnorder() && !this->mutator->isOrdered(currentBaseDeck)) {
                            Core::StaticDeckTemplate::Ptr mutation = this->mutator->asOrdered(currentBaseDeck);
                            this->stage0 = STEP;
                            this->nextResult = mutation;
                            this->_hasNext = true;
                            return;
                        } else {
                            this->stage0 = STEP;
                        }
                    case STEP:
                        //std::clog << "stage0 == STEP" << std::endl;
                        this->sourceCurrent++;
                        this->stage0 = SETUP;
                        break;
                } // switch stage0
            } // while
        }

        template<class Iterator>
        typename Generator<Iterator>::result_type
        Generator<Iterator>::operator()()
        {
            assertX(this->_hasNext);
            result_type result = this->nextResult;
            //std::clog << "before findNext() ";
            //std::clog.flush();
            this->findNext();
            //std::clog << "after findNext() ";
            //std::clog.flush();
            return result;
        }

        template<class Iterator>
        bool
        Generator<Iterator>::hasNext() const
        {
            return this->_hasNext;
        }


        MutationResult
        CardChangeMutator::mutate(MutationTask const & task)
        {
            // We only mutate certain deck types
            if (Core::StaticDeckTemplate::ConstPtr baseDeck = std::dynamic_pointer_cast<Core::StaticDeckTemplate const>(task.baseDeck)) {
                typedef std::set<Core::StaticDeckTemplate::ConstPtr> SCDeckSet;
                SCDeckSet input;
                input.insert(baseDeck);
                //std::clog << "before constructing generator" << std::endl;
                //std::clog.flush();
                //std::clog << "CCM::mutate() commanders: " << this->allowedCommanders.size() << std::endl;
                Ptr thisPtr = shared_from_this();
                Generator<SCDeckSet::const_iterator> generator(thisPtr, task, input.cbegin(), input.cend());
                //std::clog << "after constructing generator" << std::endl;
                //std::clog.flush();
                CDeckSet mutations;
                while(generator.hasNext()) {
                    //std::clog << "get item ...";
                    //std::clog.flush();
                    Core::DeckTemplate::ConstPtr mutation = generator();
                    //std::clog << " got item ... ";
                    //std::clog << std::string(*mutation);
                    //std::clog << " inserting ...";
                    //std::clog.flush();
                    mutations.insert(mutation);
                    //std::clog << " done.";
                    //std::clog << std::endl;
                    //std::clog.flush();
                }
                MutationResult result;
                result.decks = mutations;
                return result;
            } else {
                throw InvalidUserInputError("Not supported.");
            }
        }

        void
        CardChangeMutator::abort()
        {
            throw LogicError("Not implemented.");
        }

    }
}

