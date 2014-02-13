#ifndef TYRANT_CACHE_CLI_SIMPLEORDEREDDECKTEMPLATE_HPP
    #define TYRANT_CACHE_CLI_SIMPLEORDEREDDECKTEMPLATE_HPP

    #include <memory>
    #include <list>
    #include <core/staticDeckTemplate.h++>
    #include <visitor/acyclicVisitor.h++>

    namespace C = Tyrant::Core;
    namespace TyrantCache {
        namespace CLI {

            class SimpleOrderedDeckTemplate : public C::StaticDeckTemplate {
                public:
                    typedef std::shared_ptr<SimpleOrderedDeckTemplate> Ptr;
                    CREATE_VISITOR_INTERFACE(SimpleOrderedDeckTemplate);
                public:
                    std::list<unsigned int> cards;
                public:
                    SimpleOrderedDeckTemplate(std::list<unsigned int> const & ids);
                    virtual operator std::string() const;

                    virtual void accept(Praetorian::Basics::Visitor::AcyclicVisitor & visitor);

                    virtual unsigned int getNumberOfNonCommanderCards() const;
                    virtual unsigned int getCardIdAtIndex(unsigned int index) const;
                    virtual StaticDeckTemplate::Ptr withCommander(unsigned int commanderId) const;
                    virtual StaticDeckTemplate::Ptr withoutCardAtIndex(unsigned int index) const;
                    virtual StaticDeckTemplate::Ptr withSwappedCards(size_t i, size_t j) const;
                    virtual StaticDeckTemplate::Ptr withCard(unsigned int cardId) const;
                    virtual StaticDeckTemplate::Ptr withCardAtIndex(unsigned int cardId , size_t index) const;
                    virtual StaticDeckTemplate::Ptr withReplacedCardAtIndex(unsigned int cardId, size_t index) const;
            };
        }
    }

#endif
