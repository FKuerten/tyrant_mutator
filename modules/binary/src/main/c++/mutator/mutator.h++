#ifndef TYRANT_MUTATOR_MUTATOR_HPP
    #define TYRANT_MUTATOR_MUTATOR_HPP

    #include <memory>
    #include "mutationTask.h++"
    #include "mutationResult.h++"

    namespace Tyrant {
        namespace Mutator {

            class Mutator {
                public:
                    typedef std::shared_ptr<Mutator> Ptr;

                public:
                    virtual ~Mutator();
                    virtual MutationResult mutate(MutationTask const & task) = 0;
                    virtual void abort() = 0;
            };

        }
    }

#endif
