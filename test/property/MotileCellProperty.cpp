#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractCellProperty.hpp"
#include "AbstractForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "NodesOnlyMesh.hpp"
#include "WildTypeCellMutationState.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "CellLabel.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

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
