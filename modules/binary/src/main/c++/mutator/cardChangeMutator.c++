#include "cardChangeMutator.h++"
#include <errorHandling/assert.h++>
#include <errorHandling/exceptions.h++>
#include <core/autoDeckTemplate.h++>
#include <core/cards/cardLoader.h++>
#include "../cli/simpleOrderedDeckTemplate.h++"
// for dynamic_pointer_cast
#include <memory>
#include <list>
#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include "derefCompareLT.h++"

namespace Tyrant {
    namespace Mutator {

        static size_t const DEFAULT_DECK_SIZE{10};

        std::multiset<unsigned int> loadOwnedCards(std::string const & fileName)
        {
            std::multiset<unsigned int> cards;
            //std::clog << "loading file " << fileName << std::endl;
            std::ifstream is;
            std::istream::iostate const oldExceptionFlags = is.exceptions();
            is.exceptions(std::istream::failbit | std::istream::badbit);
            try {
                is.open(fileName);
                //std::clog << "in loadOwnedCards:" << __LINE__ << std::endl;

                std::string sRegex = R"(^\[(\d*)\] (.*) \((\d*)\)$)";
                //std::clog << sRegex << std::endl;
                boost::regex regex(sRegex);

                std::string line;
                while (std::getline(is, line)) {
                    //std::clog << line << std::endl;
                    boost::smatch match;
                    if (boost::regex_match(line, match, regex)) {
                        unsigned int const id = boost::lexical_cast<unsigned int>(match.str(1));
                        std::string const name = match.str(2);
                        size_t const count = boost::lexical_cast<unsigned int>(match.str(3));
                        //std::clog << "Got card " << id << " x " << count << std::endl;
                        for(size_t i = 0; i < count ; i++) {
                            cards.insert(id);
                        }
                    } else {
                        std::stringstream ssMessage;
                        ssMessage << "Incorrect format for owned cards, should be ";
                        ssMessage << sRegex;
                        throw InvalidUserInputError(ssMessage.str());
                    }
                    is.peek();
                    if (is.eof()) {
                        break;
                    }
                }
            } catch (std::ios_base::failure & e) {
                std::stringstream ssMessage;
                ssMessage << "Got am ios_base::failure with error code ";
                 #if (__GNUC__ < 4) || ((__GNUC__ == 4) and (__GNUC_MINOR__ <= 8))
                    ssMessage << "(broken gcc does not support new ios_base::failure yet)";
                #else
                    ssMessage << e.code();
                #endif
                ssMessage << std::endl;
                if (!is.good()) {
                    ssMessage << "with flags ";
                    if (is.bad()) {
                        ssMessage << "'bad' ";
                    }
                    if (is.fail()) {
                        ssMessage << "'fail' ";
                    }
                    if (is.eof()) {
                        ssMessage << "'eof' ";
                    }
                }
                ssMessage << "and message: " << std::endl;
                ssMessage << e.what();
                throw Exception(ssMessage.str());
            }
            is.exceptions(oldExceptionFlags);
            return cards;
        }

        CardChangeMutator::CardChangeMutator()
        {
            // Get a card database
            Core::Cards::Cards cards = Core::Cards::loadFromXMLFile("cards.xml");
            this->initCardDB(cards);
            std::multiset<unsigned int> ownedCards = loadOwnedCards("ownedcards.txt");
            this->buildAllowedCards(ownedCards);
        }

        void
        CardChangeMutator::initCardDB(Core::Cards::Cards const & cards)
        {
            this->cardDB = cards;
        }

        void
        CardChangeMutator::buildAllowedCards(std::multiset<unsigned int> const & ownedCards)
        {
            for(unsigned int const cardId : ownedCards) {
                Core::Cards::Card const card = this->cardDB[cardId];
                if (card.type == Core::Cards::CardType::COMMANDER) {
                    this->allowedCommanders.insert(card.id);
                } else {
                    this->allowedNonCommanderCards.insert(card.id);
                }
            }
            //std::clog << this->allowedCommanders.size() << " "
            //          << this->allowedNonCommanderCards.size() << std::endl;
        }

        static bool
        isOrdered(Core::StaticDeckTemplate const & deck)
        {
            // dirty code
            if (dynamic_cast<Core::AutoDeckTemplate const *>(&deck) != NULL) {
                return false;
            } else if (dynamic_cast<TyrantCache::CLI::SimpleOrderedDeckTemplate const *>(&deck) != NULL) {
                return true;
            } else {
                assertX(false);
                return true;
            }
        }

        static Core::AutoDeckTemplate::Ptr
        asUnordered(Core::StaticDeckTemplate const & orig)
        {
            std::list<unsigned int> cards;
            cards.push_back(orig.getCommanderId());
            for(size_t i = 0; i < orig.getNumberOfNonCommanderCards(); i++) {
                cards.push_back(orig.getCardIdAtIndex(i));
            }
            return Core::AutoDeckTemplate::Ptr(new Core::AutoDeckTemplate(cards));
        }

        static TyrantCache::CLI::SimpleOrderedDeckTemplate::Ptr
        asOrdered(Core::StaticDeckTemplate const & orig)
        {
            std::list<unsigned int> cards;
            cards.push_back(orig.getCommanderId());
            for(size_t i = 0; i < orig.getNumberOfNonCommanderCards(); i++) {
                cards.push_back(orig.getCardIdAtIndex(i));
            }
            return TyrantCache::CLI::SimpleOrderedDeckTemplate::Ptr(
                new TyrantCache::CLI::SimpleOrderedDeckTemplate(cards)
            );
        }

        template <class T>
        bool isSubSet(std::multiset<T> const & sub
                     ,std::multiset<T> copyOfSuper
                     ,unsigned int allowUpToExtra = 0
                     )
        {
            for(auto iter = sub.begin()
               ;iter != sub.end()
               ;iter++)
            {
                auto superIter = copyOfSuper.find(*iter);
                if(superIter == copyOfSuper.end()) {
                    if(allowUpToExtra > 0) {
                        allowUpToExtra--;
                    } else {
                        return false;
                    }
                } else {
                    copyOfSuper.erase(superIter);
                }
            }
            return true;
        }


        CardChangeMutator::~CardChangeMutator()
        {
        }

        MutationResult
        CardChangeMutator::mutate(MutationTask const & task)
        {
            class Generator {
                private: // variable
                    CardChangeMutator::Ptr mutator;


            };

            // We only mutate certain deck types
            if (Core::StaticDeckTemplate::ConstPtr baseDeck = std::dynamic_pointer_cast<Core::StaticDeckTemplate const>(task.baseDeck)) {
                DeckSet mutations;
                this->mutateOne(task, *baseDeck, mutations);
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

        void
        CardChangeMutator::mutateOne(MutationTask const & task, Core::StaticDeckTemplate const & baseDeck, DeckSet & mutations)
        {
            if (task.mutateUnorder()) {
                this->mutateUnorder(task, baseDeck, mutations);
            }
            if (task.mutateChangeCommander()) {
                this->mutateChangeCommander(task, baseDeck, mutations);
            }
            if (task.mutateRemoveCard()) {
                this->mutateRemoveCard(task, baseDeck, mutations);
            }
            if (task.mutateAddCard()) {
                this->mutateAddCard(task, baseDeck, mutations);
            }
            if (task.mutateSwapCard()) {
                this->mutateSwapCard(task, baseDeck, mutations);
            }
            if (task.mutateReplaceCard()) {
                this->mutateReplaceCard(task, baseDeck, mutations);
            }
            if (task.mutateOrder()) {
                this->mutateOrder(task, baseDeck, mutations);
            }
        }

        void
        CardChangeMutator::mutateUnorder(MutationTask const & task
                                        ,Core::StaticDeckTemplate const & baseDeck
                                        ,DeckSet & mutations
                                        )
        {
            if(isOrdered(baseDeck)) {
                Core::StaticDeckTemplate::Ptr mutation = asUnordered(baseDeck);
                mutations.insert(mutation);
            }
        }

        void
        CardChangeMutator::mutateChangeCommander(MutationTask const & task
                                                ,Core::StaticDeckTemplate const & baseDeck
                                                ,DeckSet & mutations
                                                )
        {
            for(unsigned int commanderId: this->allowedCommanders)
            {

                Core::StaticDeckTemplate::Ptr mutation = baseDeck.withCommander(commanderId);
                if(isValid(*mutation) && canCompose(*mutation)) {
                    mutations.insert(mutation);
                }
            }

        }

        void
        CardChangeMutator::mutateRemoveCard(MutationTask const & task
                                           ,Core::StaticDeckTemplate const & baseDeck
                                           ,DeckSet & mutations
                                           )
        {
            size_t const numberOfCards = baseDeck.getNumberOfNonCommanderCards();
            for(size_t i = 0; i < numberOfCards; i++) {
                Core::StaticDeckTemplate::Ptr mutation = baseDeck.withoutCardAtIndex(i);
                mutations.insert(mutation);
            }

        }

        void
        CardChangeMutator::mutateAddCard(MutationTask const & task
                                        ,Core::StaticDeckTemplate const & baseDeck
                                        ,DeckSet & mutations
                                        )
        {
            size_t const numberOfCards = baseDeck.getNumberOfNonCommanderCards();
            if (numberOfCards < DEFAULT_DECK_SIZE) {
                // consider all possible cards
                for(unsigned int cardId: this->allowedNonCommanderCards)
                {
                    if(!isOrdered(baseDeck)) {
                        Core::StaticDeckTemplate::Ptr mutation = baseDeck.withCard(cardId);
                        // check for validity and can compose
                        if(this->isValid(*mutation) && this->canCompose(*mutation)) {
                            mutations.insert(mutation);
                        }
                    } else {
                        for(unsigned int i = 0; i <= numberOfCards; i++) {
                            Core::StaticDeckTemplate::Ptr mutation = baseDeck.withCardAtIndex(cardId, i);
                            if(this->isValid(*mutation) && this->canCompose(*mutation)) {
                                mutations.insert(mutation);
                            }
                        }
                    } // (un)ordered
                }
            }
            //std::clog << "\t\tfound " << std::setw(5) << count << " add card mutations." << std::endl;

        }

        void
        CardChangeMutator::mutateSwapCard(MutationTask const & task
                                         ,Core::StaticDeckTemplate const & baseDeck
                                         ,DeckSet & mutations
                                         )
        {
            if (isOrdered(baseDeck) && !(task.onlyAutoDecks)) {
                size_t const numberOfCards = baseDeck.getNumberOfNonCommanderCards();
                for(unsigned int i = 0; i+1 < numberOfCards; i++) {
                    for(unsigned int j = i+1; j < numberOfCards; j++) {
                        Core::StaticDeckTemplate::Ptr mutation = baseDeck.withSwappedCards(i,j);
                        mutations.insert(mutation);
                    } // for j
                } // for i
            }

        }

        void
        CardChangeMutator::mutateReplaceCard(MutationTask const & task
                                            ,Core::StaticDeckTemplate const & baseDeck
                                            ,DeckSet & mutations
                                            )
        {
            size_t const numberOfCards = baseDeck.getNumberOfNonCommanderCards();
            for(size_t i = 0; i < numberOfCards; i++) {
                 // consider all possible cards
                for(unsigned int const cardId: this->allowedNonCommanderCards)
                {
                    // replace
                    Core::StaticDeckTemplate::Ptr mutation = baseDeck.withReplacedCardAtIndex(cardId, i);

                    // check for validity
                    if(!this->isValid(*mutation)) {
                        //std::clog << "Invalid" << std::endl;
                        //std::clog << mutation << std::endl;
                        //std::clog << "-------" << std::endl;
                        continue;
                    }
                    // can compose?
                    if(!this->canCompose(*mutation)) {
                        //std::clog << "Can not compose" << std::endl;
                        //std::clog << mutation << std::endl;
                        //std::clog << "-------" << std::endl;
                        continue;
                    }

                    //std::clog << mutation << std::endl;
                    mutations.insert(mutation);

                    if (this->aborted) {
                        throw AbortionException("During replace mutations.");
                    }
                } // for
            }

        }

        void
        CardChangeMutator::mutateOrder(MutationTask const & task
                                      ,Core::StaticDeckTemplate const & baseDeck
                                      ,DeckSet & mutations
                                      )
        {
            if(!isOrdered(baseDeck) && !(task.onlyAutoDecks)) {
                #if 0
                    //std::clog << "ordering..." << std::endl;
                    // compute all orders... thats gonna be a lot
                    PCardList const emptyList;
                    PCardMSet const cards = original.getMCards();
                    PCardListSet permutations;
                    generatePermutations(emptyList, cards, permutations);
                    for(PCardListSet::const_iterator iter = permutations.begin()
                       ;iter != permutations.end()
                       ;iter++)
                    {
                        PCardList permutation = *iter;
                        Deck mutation(original.getCommander(), permutation, true);
                        //std::clog << mutation << std::endl;
                        mutations.insert(mutation);
                    }
                #else
                    Core::StaticDeckTemplate::Ptr mutation = asOrdered(baseDeck);
                    mutations.insert(mutation);
                #endif
            }

        }

        bool
        CardChangeMutator::isValid(Core::StaticDeckTemplate const & deck)
        {
            bool hasLegendary = false;
            std::set<Core::Cards::Card> uniques;
            if (this->cardDB[deck.getCommanderId()].rarity == Core::Cards::CardRarity::LEGENDARY) {
                hasLegendary = true;
            }
            for(size_t i = 0; i < deck.getNumberOfNonCommanderCards(); i++) {
                unsigned int const cardId = deck.getCardIdAtIndex(i);
                Core::Cards::Card card = this->cardDB[cardId];
                if (card.rarity == Core::Cards::CardRarity::LEGENDARY) {
                    if (hasLegendary) {
                        return false;
                    } else {
                        hasLegendary = true;
                    }
                } else if (card.rarity == Core::Cards::CardRarity::UNIQUE) {
                    if (uniques.find(card) != uniques.end()) {
                        return false;
                    } else {
                        uniques.insert(card);
                    }
                }
            } // for
            return true;
        }

        bool
        CardChangeMutator::canCompose(Core::StaticDeckTemplate const & deck)
        {
            if (this->allowedCommanders.find(deck.getCommanderId()) == this->allowedCommanders.end()) {
                return false;
            }
            std::multiset<unsigned int> cards;
            for(size_t i = 0; i < deck.getNumberOfNonCommanderCards(); i++) {
                unsigned int const cardId = deck.getCardIdAtIndex(i);
                cards.insert(cardId);
            }
            return isSubSet(cards, this->allowedNonCommanderCards);
        }

    }
}

