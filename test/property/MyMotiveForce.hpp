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
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractForce<2> >(*this);
        archive & mStrength;
    }

public:
    MyMotiveForce(double strength=2.0)
        : AbstractForce<2>(),
          mStrength(strength)
    {
        assert(mStrength > 0.0);
    }

    void AddForceContribution(AbstractCellPopulation<2>& rCellPopulation)
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

    void OutputForceParameters(out_stream& rParamsFile)
    {
        *rParamsFile << "\t\t\t<Strength>" << mStrength << "</Strength>\n";
        AbstractForce<2>::OutputForceParameters(rParamsFile);
    }
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(MyMotiveForce)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(MyMotiveForce)
