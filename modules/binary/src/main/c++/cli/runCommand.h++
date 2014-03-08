#ifndef TYRANT_MUTATOR_CLI_RUNCOMMAND_HPP
    #define TYRANT_MUTATOR_CLI_RUNCOMMAND_HPP

    #include "commands.h++"
    #include <memory>
    #include <mutator/mutator.h++>
    #include <mutator/mutationTask.h++>

    using namespace Tyrant;
    namespace TyrantMutator {
        namespace CLI {

            class RunCommand : public Command {
                public:
                    typedef std::shared_ptr<RunCommand> Ptr;
                public:
                    Mutator::MutationTask task;
                private:
                    Mutator::Mutator::Ptr mutator;
                public:
                    RunCommand(Configuration);
                    ~RunCommand();

                    int execute();
                    void abort();
            };
        }
    }

#endif
