#ifndef MYFORCE_HPP_
#define MYFORCE_HPP_

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

class MyForce : public AbstractForce<2>
{
private:

    double mStrength;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:
    MyForce(double strength=1.0);

    void AddForceContribution(AbstractCellPopulation<2>& rCellPopulation);

    double GetStrength();

    void OutputForceParameters(out_stream& rParamsFile);
};

#endif // MYFORCE_HPP_