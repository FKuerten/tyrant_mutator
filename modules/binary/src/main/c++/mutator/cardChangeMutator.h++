#ifndef TYRANT_MUTATOR_CARDCHANGEMUTATOR_HPP
    #define TYRANT_MUTATOR_CARDCHANGEMUTATOR_HPP

    #include "abstractMutator.h++"
    #include "cardChangeMutatorGenerator.forward.h++"

    namespace Tyrant {
        namespace Mutator {

            class CardChangeMutator : public AbstractMutator
                                    , public std::enable_shared_from_this<CardChangeMutator> {
                public:
                    typedef std::shared_ptr<CardChangeMutator> Ptr;
                    typedef std::shared_ptr<CardChangeMutator const> ConstPtr;

                private:
                    bool aborted;

                public:
                    CardChangeMutator();
                    virtual ~CardChangeMutator();
                    virtual MutationResult mutate(MutationTask const & task);

                    virtual void abort();

                friend class CardChangeMutatorGenerator;
            };

        }
    }

#endif

