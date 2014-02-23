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
                //std::clog << "before constructing generator" << std::endl;
                //std::clog.flush();
                //std::clog << "CCM::mutate() commanders: " << this->allowedCommanders.size() << std::endl;
                Ptr thisPtr = shared_from_this();
                CardChangeMutatorGenerator<SCDeckSet::const_iterator> generator(thisPtr, task, input.cbegin(), input.cend());
                //std::clog << "after constructing generator" << std::endl;
                //std::clog.flush();
                CDeckSet mutations;
                while(generator.hasNext()) {
                    //std::clog << "get item ...";
                    //std::clog.flush();
                    Core::DeckTemplate::ConstPtr mutation = generator();
                    //std::clog << " got item ... ";
                    //std::clog << std::string(*mutation);
                    //std::clog << " inserting ...";
                    //std::clog.flush();
                    mutations.insert(mutation);
                    //std::clog << " done.";
                    //std::clog << std::endl;
                    //std::clog.flush();
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
            throw LogicError("Not implemented.");
        }

    }
}

