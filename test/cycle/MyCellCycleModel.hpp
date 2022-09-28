#ifndef MYCELLCYCLEMODEL_HPP_
#define MYCELLCYCLEMODEL_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "SmartPointers.hpp"
#include "Exception.hpp"

#include "AbstractSimpleGenerationalCellCycleModel.hpp"

#include "CheckReadyToDivideAndPhaseIsUpdated.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

class MyCellCycleModel : public AbstractSimpleGenerationalCellCycleModel
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

    void SetG1Duration();

public:

    MyCellCycleModel() {}

    AbstractCellCycleModel* CreateCellCycleModel();
};

#endif // MYCELLCYCLEMODEL_HPP_