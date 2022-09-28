#include "AbstractCellMutationState.hpp"
#include "P53GainOfFunctionCellMutationState.hpp"

template<class Archive>
void P53GainOfFunctionCellMutationState::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractCellMutationState>(*this);
}

P53GainOfFunctionCellMutationState::P53GainOfFunctionCellMutationState()
    : AbstractCellMutationState(5)
{}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(P53GainOfFunctionCellMutationState)
