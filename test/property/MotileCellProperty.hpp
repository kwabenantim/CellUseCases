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

class MotileCellProperty : public AbstractCellProperty
{
private:

    unsigned mColour;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellProperty>(*this);
        archive & mColour;
    }

public:

    MotileCellProperty(unsigned colour=5)
        : AbstractCellProperty(),
          mColour(colour)
    {
    }

    ~MotileCellProperty()
    {}

    unsigned GetColour() const
    {
        return mColour;
    }
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(MotileCellProperty)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(MotileCellProperty)
