#ifndef MOTILECELLPROPERTY_HPP_
#define MOTILECELLPROPERTY_HPP_

#include "AbstractCellProperty.hpp"

class MotileCellProperty : public AbstractCellProperty
{
private:

    unsigned mColour;

    friend class boost::serialization::access;
    
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    MotileCellProperty(unsigned colour=5);

    ~MotileCellProperty() {}

    unsigned GetColour() const;
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(MotileCellProperty)

#endif // MOTILECELLPROPERTY_HPP_
