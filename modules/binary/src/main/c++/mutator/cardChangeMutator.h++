#ifndef TYRANT_MUTATOR_MUTATOR_CARDCHANGEMUTATOR_HPP
#define TYRANT_MUTATOR_MUTATOR_CARDCHANGEMUTATOR_HPP

    #include <memory>
    #include "mutator.h++"

    namespace Tyrant {
        namespace Mutator {

            class CardChangeMutator : public Mutator {
                public:
                    typedef std::shared_ptr<CardChangeMutator> Ptr;

                public:
                    virtual ~CardChangeMutator();
                    virtual MutationResult mutate(MutationTask const & task);
                    virtual void abort();
            };

        }
    }

#endif

