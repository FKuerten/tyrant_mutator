#ifndef TYRANT_MUTATOR_MUTATING_ITERATOR_HPP
    #define TYRANT_MUTATOR_MUTATING_ITERATOR_HPP

    #include <set>
    #include "core/deckTemplate.h++"
    #include "derefCompareLT.h++"

    namespace Tyrant {
        namespace Mutator {

            class MutatingIterator {
                // types
                public:
                    typedef ::Tyrant::Core::DeckTemplate::ConstPtr value_type;
                    typedef std::set<value_type, DerefCompareLT> CDeckSet;
                    typedef CDeckSet::const_iterator BaseIterator;
                    typedef std::shared_ptr<MutatingIterator> Ptr;
                    typedef std::shared_ptr<MutatingIterator const> ConstPtr;

                // variables
                protected:
                    CDeckSet const source;
                    BaseIterator sourceCurrent;
                    BaseIterator const sourceEnd;

                // functions
                protected:
                    bool equals(MutatingIterator const & rhs) const;
                    virtual bool equals2(MutatingIterator const & rhs) const = 0;

                public:
                    MutatingIterator(CDeckSet const source, bool atEnd);
                    virtual ~MutatingIterator();

                    virtual MutatingIterator & operator++() = 0;
                    //virtual MutatingIterator const & operator++(int) = 0;
                    virtual value_type operator*() const = 0;

                friend bool operator==(MutatingIterator const &, MutatingIterator const &);
            };

            bool operator==(MutatingIterator const &, MutatingIterator const &);
            bool operator!=(MutatingIterator const &, MutatingIterator const &);

        }
    }
#endif
