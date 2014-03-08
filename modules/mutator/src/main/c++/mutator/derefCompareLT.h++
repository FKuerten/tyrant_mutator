#ifndef TYRANTMUTATOR_MUTATOR_DEREFCOMPARELT_HPP
    #define TYRANTMUTATOR_MUTATOR_DEREFCOMPARELT_HPP

    #include <core/staticDeckTemplate.h++>

    namespace Tyrant {
        namespace Mutator {

            class DerefCompareLT {
                public:
                    bool operator() (Core::DeckTemplate::ConstPtr const & a, Core::DeckTemplate::ConstPtr const & b) const;

                    bool operator() (Core::DeckTemplate const & a, Core::DeckTemplate const &b) const;
            };
        }
    }
#endif
