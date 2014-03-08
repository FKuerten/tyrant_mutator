#ifndef TYRANT_MUTATOR_CLI_CONFIGURATION_HPP
    #define TYRANT_MUTATOR_CLI_CONFIGURATION_HPP

    #include <mutator/mutator.h++>

    namespace TyrantMutator {
        namespace CLI {

            class Configuration {
                public:
                    signed int verbosity = 0;

                    Tyrant::Mutator::Mutator::Ptr constructMutator() const;
            };
        }
    }
#endif
