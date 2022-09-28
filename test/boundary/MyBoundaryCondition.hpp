#ifndef MYBOUNDARYCONDITION_HPP_
#define MYBOUNDARYCONDITION_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractCellPopulationBoundaryCondition.hpp"
#include "OffLatticeSimulation.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "CellsGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "SmartPointers.hpp"
#include "FakePetscSetup.hpp"

class MyBoundaryCondition : public AbstractCellPopulationBoundaryCondition<2>
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    MyBoundaryCondition(AbstractCellPopulation<2>* pCellPopulation);

    void ImposeBoundaryCondition(const std::map<Node<2>*, c_vector<double, 2> >& rOldLocations);

    bool VerifyBoundaryCondition();

    void OutputCellPopulationBoundaryConditionParameters(out_stream& rParamsFile);
};

#endif // MYBOUNDARYCONDITION_HPP_