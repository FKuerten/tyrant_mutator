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

    }
}
