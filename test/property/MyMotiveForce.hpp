#ifndef MYMOTIVEFORCE_HPP_
#define MYMOTIVEFORCE_HPP_

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

class MyMotiveForce : public AbstractForce<2>
{
private:

    double mStrength;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    MyMotiveForce(double strength=2.0);

    void AddForceContribution(AbstractCellPopulation<2>& rCellPopulation);

    void OutputForceParameters(out_stream& rParamsFile);
};

// #include "SerializationExportWrapper.hpp"
// CHASTE_CLASS_EXPORT(MyMotiveForce)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(MyMotiveForce)

#endif // MYMOTIVEFORCE_HPP_