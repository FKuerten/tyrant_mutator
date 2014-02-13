#include "simpleOrderedDeckTemplate.h++"
#include <errorHandling/assert.h++>

namespace TyrantCache {
    namespace CLI {

        SimpleOrderedDeckTemplate::SimpleOrderedDeckTemplate(std::list<unsigned int> const & ids)
        : StaticDeckTemplate()
        {
            std::list<unsigned int>::const_iterator iter = ids.begin();
            unsigned int const commanderId = *iter;
            this->commanderId = commanderId;
            for(iter++ ;iter != ids.end(); iter++) {
                unsigned int cardId = *iter;
                this->cards.push_back(cardId);
            }
        }

        SimpleOrderedDeckTemplate::operator std::string() const
        {
            std::stringstream ssString;
            ssString << "ORDERED_IDS:";
            ssString << this->commanderId;
            for(std::list<unsigned int>::const_iterator iter = this->cards.begin()
               ;iter != this->cards.end()
               ;iter++
               )
            {
                ssString << ",";
                ssString << *iter;
            }
            return ssString.str();
        }

        CREATE_VISITOR_METHOD(SimpleOrderedDeckTemplate)

        Tyrant::Core::StaticDeckTemplate::Ptr
        SimpleOrderedDeckTemplate::withCommander(unsigned int commanderId) const
        {
            SimpleOrderedDeckTemplate::Ptr copy(new SimpleOrderedDeckTemplate(*this));
            copy->commanderId = commanderId;
            return copy;
        }

        size_t
        SimpleOrderedDeckTemplate::getNumberOfNonCommanderCards() const
        {
            return this->cards.size();
        }

        unsigned int
        SimpleOrderedDeckTemplate::getCardIdAtIndex(size_t index) const
        {
            std::list<unsigned int>::const_iterator iter = this->cards.begin();
            for(size_t i = 0u; i < index; i++) {
                iter++;
            }
            return *iter;
        }

        Tyrant::Core::StaticDeckTemplate::Ptr
        SimpleOrderedDeckTemplate::withCard(unsigned int cardId) const
        {
            SimpleOrderedDeckTemplate::Ptr copy(new SimpleOrderedDeckTemplate(*this));
            copy->cards.push_back(cardId);
            return copy;
        }

        Tyrant::Core::StaticDeckTemplate::Ptr
        SimpleOrderedDeckTemplate::withCardAtIndex(unsigned int cardId, size_t index) const
        {
            SimpleOrderedDeckTemplate::Ptr copy(new SimpleOrderedDeckTemplate(*this));
            std::list<unsigned int>::iterator iter = copy->cards.begin();
            for(size_t i = 0u; i < index; i++) {
                iter++;
            }
            copy->cards.insert(iter,cardId);
            return copy;
        }

        Tyrant::Core::StaticDeckTemplate::Ptr
        SimpleOrderedDeckTemplate::withoutCardAtIndex(size_t index) const
        {
            SimpleOrderedDeckTemplate::Ptr copy(new SimpleOrderedDeckTemplate(*this));
            std::list<unsigned int>::iterator iter = copy->cards.begin();
            for(size_t i = 0u; i < index; i++) {
                iter++;
            }
            copy->cards.erase(iter);
            return copy;
        }

        Tyrant::Core::StaticDeckTemplate::Ptr
        SimpleOrderedDeckTemplate::withSwappedCards(size_t i, size_t j) const
        {
            SimpleOrderedDeckTemplate::Ptr copy(new SimpleOrderedDeckTemplate(*this));
            std::list<unsigned int>::iterator iterI = copy->cards.begin();
            for(size_t ii = 0u; ii < i; ii++) {
                iterI++;
            }
            std::list<unsigned int>::iterator iterJ = copy->cards.begin();
            for(size_t jj = 0u; jj < j; jj++) {
                iterJ++;
            }
            unsigned int cardAtI = *iterI;
            unsigned int cardAtJ = *iterJ;
            *iterI = cardAtJ;
            *iterJ = cardAtI;
            return copy;
        }

        Tyrant::Core::StaticDeckTemplate::Ptr
        SimpleOrderedDeckTemplate::withReplacedCardAtIndex(unsigned int cardId, size_t index) const
        {
            SimpleOrderedDeckTemplate::Ptr copy(new SimpleOrderedDeckTemplate(*this));
            std::list<unsigned int>::iterator iter = copy->cards.begin();
            for(size_t i = 0u; i < index; i++) {
                iter++;
            }
            *iter = cardId;
            return copy;
        }

        bool
        operator< (SimpleOrderedDeckTemplate const & a
                  ,SimpleOrderedDeckTemplate const & b
                  )
        {
            if (a.commanderId < b.commanderId) {
                return true;
            } else if (a.commanderId > b.commanderId) {
                return false;
            } else {
                assertEQ(a.commanderId, b.commanderId);
                return a.cards < b.cards;
            }
        }

    }
}
