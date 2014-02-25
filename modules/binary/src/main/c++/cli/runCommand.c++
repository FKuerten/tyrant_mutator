#include "runCommand.h++"

#include "configuration.h++"
#include <iomanip>
#include <errorHandling/exceptions.h++>
#include "../mutator/mutationResult.h++"

namespace C = Tyrant::Core;
namespace TyrantMutator {
    namespace CLI {

        RunCommand::RunCommand(Configuration configuration
                              )
        : Command(configuration)
        {
            this->mutator = configuration.constructMutator();
        }

        RunCommand::~RunCommand()
        {
        }

        int RunCommand::execute() {
            Mutator::MutationResult r = this->mutator->mutate(this->task);

            for(Tyrant::Mutator::DeckIterator iter = r.begin; iter != r.end; ++iter) {
                Core::DeckTemplate::ConstPtr deck = *iter;
                std::cout << std::string(*deck) << std::endl;
            }
            //std::clog << "done with execute" << std::endl;
            return 0;
        }

        void
        RunCommand::abort()
        {
            this->mutator->abort();
        }

    }
}
