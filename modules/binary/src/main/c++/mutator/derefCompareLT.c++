#include "derefCompareLT.h++"
#include <core/autoDeckTemplate.h++>
#include "../cli/simpleOrderedDeckTemplate.h++"

namespace Tyrant {
    namespace Mutator {

        bool
        DerefCompareLT::operator() (Core::DeckTemplate::ConstPtr const & a, Core::DeckTemplate::ConstPtr const & b) const {
            return this->operator()(*a,*b);
        }

        bool
        DerefCompareLT::operator() (Core::DeckTemplate const & a, Core::DeckTemplate const & b) const {
            Core::AutoDeckTemplate const * const aa
                = dynamic_cast<Core::AutoDeckTemplate const *>(&a);
            if (aa) {
                Core::AutoDeckTemplate const * const ab
                    = dynamic_cast<Core::AutoDeckTemplate const *>(&b);
                if (ab != NULL) {
                    return (*aa) < (*ab);
                } else {
                    return true;
                }
            }
            TyrantCache::CLI::SimpleOrderedDeckTemplate const * const sa
                = dynamic_cast<TyrantCache::CLI::SimpleOrderedDeckTemplate const *>(&a);
            if (sa != NULL) {
                TyrantCache::CLI::SimpleOrderedDeckTemplate const * const sb
                    = dynamic_cast<TyrantCache::CLI::SimpleOrderedDeckTemplate const *>(&b);
                if (sb != NULL) {
                    return (*sa) < (*sb);
                } else {
                    return false;
                }
            } else {
                throw Exception("Can't compare them, sorry.");
            }
        }

    }
}
