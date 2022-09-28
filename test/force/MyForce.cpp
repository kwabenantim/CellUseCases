#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellsGenerator.hpp"
#include "TransitCellProliferativeType.hpp"
#include "SmartPointers.hpp"

#include "FakePetscSetup.hpp"

#include "MyForce.hpp"

template<class Archive>
void MyForce::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractForce<2> >(*this);
    archive & mStrength;
}

MyForce::MyForce(double strength)
    : AbstractForce<2>(),
        mStrength(strength)
{
    assert(mStrength > 0.0);
}

void MyForce::AddForceContribution(AbstractCellPopulation<2>& rCellPopulation)
{
    c_vector<double, 2> force = zero_vector<double>(2);
    force(1) = -mStrength;

    for (unsigned node_index=0; node_index<rCellPopulation.GetNumNodes(); node_index++)
    {
        rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
    }
}

double MyForce::GetStrength()
{
    return mStrength;
}

void MyForce::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<Strength>" << mStrength << "</Strength>\n";
    AbstractForce<2>::OutputForceParameters(rParamsFile);
}

// #include "SerializationExportWrapper.hpp"
// CHASTE_CLASS_EXPORT(MyForce)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(MyForce)

#endif // MYFORCE_HPP_