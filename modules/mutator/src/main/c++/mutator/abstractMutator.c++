#include "abstractMutator.h++"

// std::ifstream
#include <fstream>

// for boost::regex and boost::lexical_cast
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

// for assertions
#include <errorHandling/assert.h++>

namespace Tyrant {
    namespace Mutator {

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

        AbstractMutator::AbstractMutator()
        {
            // Get a card database
            Core::Cards::Cards cards = Core::Cards::loadFromXMLFile("cards.xml");
            this->initCardDB(cards);
            std::multiset<unsigned int> ownedCards = loadOwnedCards("ownedcards.txt");
            this->buildAllowedCards(ownedCards);
            //std::clog << "AM::AM() commanders: " << this->allowedCommanders.size() << std::endl;
        }

        AbstractMutator::~AbstractMutator()
        {
        }

        void
        AbstractMutator::initCardDB(Core::Cards::Cards const & cards)
        {
            this->cardDB = cards;
        }

        void
        AbstractMutator::buildAllowedCards(std::multiset<unsigned int> const & ownedCards)
        {
            for(unsigned int const cardId : ownedCards) {
                Core::Cards::Card const card = this->cardDB[cardId];
                if (card.type == Core::Cards::CardType::COMMANDER) {
                    this->allowedCommanders.insert(card.id);
                } else {
                    this->allowedNonCommanderCards.insert(card.id);
                    if (this->allowedNonCommanderCardsWithCount.count(card.id) < 10) {
                        this->allowedNonCommanderCardsWithCount.insert(card.id);
                    }
                }
            }
            //std::clog << this->allowedCommanders.size() << " "
            //          << this->allowedNonCommanderCards.size() << std::endl;
        }

        bool
        AbstractMutator::isValid(Core::StaticDeckTemplate const & deck) const
        {
            bool hasLegendary = false;
            std::set<Core::Cards::Card> uniques;
            if (this->cardDB.at(deck.getCommanderId()).rarity == Core::Cards::CardRarity::LEGENDARY) {
                hasLegendary = true;
            }
            for(size_t i = 0; i < deck.getNumberOfNonCommanderCards(); i++) {
                unsigned int const cardId = deck.getCardIdAtIndex(i);
                Core::Cards::Card card = this->cardDB.at(cardId);
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

        template <class T>
        bool isSubSet(std::multiset<T> const & sub
                     ,std::multiset<T> const & super
                     ,unsigned int allowUpToExtra = 0
                     )
        {
            auto subIter = sub.cbegin();
            auto subEnd = sub.cend();
            auto supIter = super.cbegin();
            auto supEnd = super.cend();
            // iterate over subset
            while(subIter != subEnd)
            {
                bool failed;
                if(supIter == supEnd) {
                    // none left in super? fail!
                    failed = true;
                } else {
                    T const & subItem = *subIter;
                    T const & supItem = *supIter;
                    if (subItem < supItem) {
                        // element in super already larger? fail!
                        failed = true;
                    } else if (subItem > supItem) {
                        // element in super smaller, keep looking
                        ++supIter;
                        failed = false;
                    } else {
                        // elements equal
                        ++subIter;
                        ++supIter;
                        failed = false;
                    }
                }

                if(failed) {
                    if(allowUpToExtra > 0) {
                        allowUpToExtra--;
                    } else {
                        return false;
                    }
                }
            }
            return true;
        }

        bool
        AbstractMutator::canCompose(Core::StaticDeckTemplate const & deck) const
        {
            if (this->allowedCommanders.find(deck.getCommanderId()) == this->allowedCommanders.end()) {
                return false;
            }
            std::multiset<unsigned int> cards;
            for(size_t i = 0; i < deck.getNumberOfNonCommanderCards(); i++) {
                unsigned int const cardId = deck.getCardIdAtIndex(i);
                cards.insert(cardId);
            }
            return isSubSet(cards, this->allowedNonCommanderCardsWithCount);
        }

        bool
        AbstractMutator::isOrdered(Core::StaticDeckTemplate const & deck)
        {
            // dirty code
            if (dynamic_cast<Core::AutoDeckTemplate const *>(&deck) != NULL) {
                return false;
            } else if (dynamic_cast<Core::SimpleOrderedDeckTemplate const *>(&deck) != NULL) {
                return true;
            } else {
                assertX(false);
                return true;
            }
        }

        Core::AutoDeckTemplate::Ptr
        AbstractMutator::asUnordered(Core::StaticDeckTemplate const & orig)
        {
            std::list<unsigned int> cards;
            cards.push_back(orig.getCommanderId());
            for(size_t i = 0; i < orig.getNumberOfNonCommanderCards(); i++) {
                cards.push_back(orig.getCardIdAtIndex(i));
            }
            return Core::AutoDeckTemplate::Ptr(new Core::AutoDeckTemplate(cards));
        }

        Core::SimpleOrderedDeckTemplate::Ptr
        AbstractMutator::asOrdered(Core::StaticDeckTemplate const & orig)
        {
            std::list<unsigned int> cards;
            cards.push_back(orig.getCommanderId());
            for(size_t i = 0; i < orig.getNumberOfNonCommanderCards(); i++) {
                cards.push_back(orig.getCardIdAtIndex(i));
            }
            return Core::SimpleOrderedDeckTemplate::Ptr(
                new Core::SimpleOrderedDeckTemplate(cards)
            );
        }

    }
}
