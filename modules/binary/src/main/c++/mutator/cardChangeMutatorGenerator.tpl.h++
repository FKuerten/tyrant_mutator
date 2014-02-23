//
//  Copyright 2014 Fabian "Praetorian" Kürten
//
//  This file is part of tyrantMutator.
//  The tyrantMutator program is free software: you can redistribute it
//  and/or modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation, exactly version 3 of
//  the License.
//
//  The tyrantMutator program is distributed in the hope that it will be
//  useful but WITHOUT ANY WARRANTY; without even the implied warranty
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not,
//  see <http://www.gnu.org/licenses/>.
//

/*
 * This is not a source file, do not compile it directly!
 */
/*
 * This is not a simple header file, do not include it directly!
 * (unless you are cardChangeMutatorGenerator.h++ in the same folder)
 */
namespace Tyrant {
    namespace Mutator {

        static size_t const DEFAULT_DECK_SIZE{10};

        template <class Iterator>
        CardChangeMutatorGenerator<Iterator>::CardChangeMutatorGenerator
                            (CardChangeMutator::ConstPtr mutator
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
        CardChangeMutatorGenerator<Iterator>::findNext()
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
        typename CardChangeMutatorGenerator<Iterator>::result_type
        CardChangeMutatorGenerator<Iterator>::operator()()
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
        CardChangeMutatorGenerator<Iterator>::hasNext() const
        {
            return this->_hasNext;
        }
    }
}
