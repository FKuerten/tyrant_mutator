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
                typedef std::set<Core::StaticDeckTemplate::ConstPtr> SCDeckSet;
                SCDeckSet input;
                input.insert(baseDeck);
                Ptr thisPtr = shared_from_this();
                CardChangeMutatorGenerator<SCDeckSet::const_iterator> generator(thisPtr, task, input.cbegin(), input.cend());
                CardChangeMutatorGenerator<SCDeckSet::const_iterator> end(thisPtr, task, input.cend(), input.cend());

                typedef Praetorian::Basics::Iterator::UniqueIterator<CardChangeMutatorGenerator<SCDeckSet::const_iterator>> UniqueIterator;
                UniqueIterator uniqueCurrent(generator, end);
                UniqueIterator uniqueEnd(end, end);

                CDeckSet mutations;
                while(uniqueCurrent != uniqueEnd && !(this->aborted)) {
                    Core::DeckTemplate::ConstPtr mutation = *uniqueCurrent;
                    //std::clog << "got one: " << std::string(*mutation) << std::endl;
                    mutations.insert(mutation);
                    ++uniqueCurrent;
                }
                MutationResult result;
                result.decks = mutations;
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

