#ifndef CELLMOTILITYWRITER_HPP_
#define CELLMOTILITYWRITER_HPP_

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
#include "AbstractCellWriter.hpp"

// #include "MotileCellProperty.hpp"

#include "PetscSetupAndFinalize.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class CellMotilityWriter : public AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>
{
private:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    CellMotilityWriter();

    double GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);

    void VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);

};

// #include "SerializationExportWrapper.hpp"
// EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellMotilityWriter)
// #include "SerializationExportWrapperForCpp.hpp"
// EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellMotilityWriter)

#endif // CELLMOTILITYWRITER_HPP_