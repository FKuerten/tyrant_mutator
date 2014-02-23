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

            for(C::DeckTemplate::ConstPtr deck : r.decks) {
                std::cout << std::string{*deck} << std::endl;
            }
            return 0;
        }

        void
        RunCommand::abort()
        {
            this->mutator->abort();
        }

    }
}
