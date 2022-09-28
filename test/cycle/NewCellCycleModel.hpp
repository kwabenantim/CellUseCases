#ifndef NEWCELLCYCLEMODEL_HPP_
#define NEWCELLCYCLEMODEL_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "SmartPointers.hpp"
#include "Exception.hpp"

#include "AbstractSimpleGenerationalCellCycleModel.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

class NewCellCycleModel : public AbstractSimpleGenerationalCellCycleModel
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

    void SetG1Duration();

public:

    NewCellCycleModel() {}

    AbstractCellCycleModel* CreateCellCycleModel();
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(NewCellCycleModel)

#endif // NEWCELLCYCLEMODEL_HPP_
