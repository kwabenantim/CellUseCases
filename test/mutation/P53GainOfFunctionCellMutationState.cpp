#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractCellMutationState.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "CellsGenerator.hpp"
#include "SmartPointers.hpp"

#include "FakePetscSetup.hpp"

#include "MotileCellProperty.hpp"
#include "P53GainOfFunctionCellMutationState.hpp"

template<class Archive>
void P53GainOfFunctionCellMutationState::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractCellMutationState>(*this);
}

P53GainOfFunctionCellMutationState::P53GainOfFunctionCellMutationState()
    : AbstractCellMutationState(5)
{}

// #include "SerializationExportWrapper.hpp"
// CHASTE_CLASS_EXPORT(P53GainOfFunctionCellMutationState)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(P53GainOfFunctionCellMutationState)
