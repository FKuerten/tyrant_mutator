#include "mutatingIterator.h++"
#include <typeinfo>

namespace Tyrant {
    namespace Mutator {

        MutatingIterator::MutatingIterator(CDeckSet const source
                                          ,bool atEnd)
        : source(source)
        , sourceCurrent(atEnd ? this->source.cend() : this->source.cbegin())
        , sourceEnd(this->source.cend())
        {
        }

        MutatingIterator::~MutatingIterator()
        {
        }

        bool
        MutatingIterator::equals(MutatingIterator const & rhs) const
        {
            if (typeid(*this) == typeid(rhs)) {
                return this->equals2(rhs);
            }
            return false;
        }

        bool
        MutatingIterator::equals2(MutatingIterator const & rhs) const
        {
            return true;
        }

        bool
        operator==(MutatingIterator const & lhs, MutatingIterator const & rhs)
        {
            return lhs.equals(rhs);
        }

        bool
        operator!=(MutatingIterator const & lhs, MutatingIterator const & rhs)
        {
            return !(lhs == rhs);
        }
    }
}
