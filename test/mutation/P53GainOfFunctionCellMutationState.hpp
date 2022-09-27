#ifndef P53GAINOFFUNCTIONCELLMUTATIONSTATE_HPP_
#define P53GAINOFFUNCTIONCELLMUTATIONSTATE_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractCellMutationState.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "CellsGenerator.hpp"
#include "SmartPointers.hpp"
#include "FakePetscSetup.hpp"

class P53GainOfFunctionCellMutationState : public AbstractCellMutationState
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellMutationState>(*this);
    }

public:
    P53GainOfFunctionCellMutationState()
        : AbstractCellMutationState(5)
    {
    }
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(P53GainOfFunctionCellMutationState)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(P53GainOfFunctionCellMutationState)

#endif // P53GAINOFFUNCTIONCELLMUTATIONSTATE_HPP_
