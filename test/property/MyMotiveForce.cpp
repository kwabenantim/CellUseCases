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
#include "MyMotiveForce.hpp"

template<class Archive>
void MyMotiveForce::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractForce<2> >(*this);
    archive & mStrength;
}

MyMotiveForce::MyMotiveForce(double strength)
    : AbstractForce<2>(),
        mStrength(strength)
{
    assert(mStrength > 0.0);
}

void MyMotiveForce::AddForceContribution(AbstractCellPopulation<2>& rCellPopulation)
{
    for (AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
            cell_iter != rCellPopulation.End();
            ++cell_iter)
    {
        if (cell_iter->HasCellProperty<MotileCellProperty>())
        {
            unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);

            c_vector<double, 2> location;
            location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            c_vector<double, 2> force = -1.0 * mStrength * location;
            rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
        }
    }
}

void MyMotiveForce::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<Strength>" << mStrength << "</Strength>\n";
    AbstractForce<2>::OutputForceParameters(rParamsFile);
}
