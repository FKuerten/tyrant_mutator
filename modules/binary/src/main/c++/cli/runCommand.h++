#ifndef TYRANT_CACHE_CLI_RUNCOMMAND_HPP
    #define TYRANT_CACHE_CLI_RUNCOMMAND_HPP

    #include "commands.h++"
    #include <memory>

    namespace TyrantMutator {
        namespace CLI {

            class RunCommand : public Command {
                public:
                    typedef std::shared_ptr<RunCommand> Ptr;
                public:
                    C::SimulationTask task;
                private:
                    Cache::DiskBackedCache::Ptr simulator;
                public:
                    void setCacheWrite(bool cacheWrite);
                    void setCacheRead(bool cacheRead);
                public:
                    RunCommand(Configuration);
                    ~RunCommand();

                    int execute();
                    void abort();
            };
        }
    }

#endif
