#include "CheckpointArchiveTypes.hpp"

#include "AbstractCellProperty.hpp"
#include "MotileCellProperty.hpp"

template<class Archive>
void MotileCellProperty::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractCellProperty>(*this);
    archive & mColour;
}

MotileCellProperty::MotileCellProperty(unsigned colour)
    : AbstractCellProperty(),
        mColour(colour)
{}

unsigned MotileCellProperty::GetColour() const
{
    return mColour;
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(MotileCellProperty)
