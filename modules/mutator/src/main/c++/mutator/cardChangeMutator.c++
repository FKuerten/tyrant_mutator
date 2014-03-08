#include "cardChangeMutator.h++"
#include <errorHandling/assert.h++>
#include <errorHandling/exceptions.h++>
#include <core/cards/cardLoader.h++>
// for dynamic_pointer_cast
#include <memory>
#include <list>
#include <set>
#include <iostream>
#include <string>
#include "derefCompareLT.h++"
#include "cardChangeMutatorGenerator.h++"
#include "iterator/uniqueIterator.h++"

namespace Tyrant {
    namespace Mutator {

        CardChangeMutator::CardChangeMutator()
        {
            //std::clog << "CCM::CCM() commanders: " << this->allowedCommanders.size() << std::endl;
        }

        CardChangeMutator::~CardChangeMutator()
        {
        }

        MutationResult
        CardChangeMutator::mutate(MutationTask const & task)
        {
            // We only mutate certain deck types
            if (Core::StaticDeckTemplate::ConstPtr baseDeck = std::dynamic_pointer_cast<Core::StaticDeckTemplate const>(task.baseDeck)) {
                CardChangeMutatorGenerator::CDeckSet input;
                input.insert(baseDeck);
                Ptr thisPtr = shared_from_this();
                CardChangeMutatorGenerator::Ptr generator = std::make_shared<CardChangeMutatorGenerator>(thisPtr, task, input);
                CardChangeMutatorGenerator::Ptr end       = std::make_shared<CardChangeMutatorGenerator>(thisPtr, task, input, true);

                DeckIterator uniqueCurrent(generator, end);
                DeckIterator uniqueEnd(end, end);

                MutationResult result(uniqueCurrent, uniqueEnd);
                return result;
            } else {
                throw InvalidUserInputError("Not supported.");
            }
        }

        void
        CardChangeMutator::abort()
        {
            this->aborted = true;
        }

    }
}

