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

#include "PetscSetupAndFinalize.hpp"

#include "MotileCellProperty.hpp"
#include "CellMotilityWriter.hpp"

// template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
// template<class Archive>
// void CellMotilityWriter<ELEMENT_DIM, SPACE_DIM>::serialize(Archive & archive, const unsigned int version)
// {
//     archive & boost::serialization::base_object<AbstractCellWriter<ELEMENT_DIM, SPACE_DIM> >(*this);
// }

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellMotilityWriter<ELEMENT_DIM, SPACE_DIM>::CellMotilityWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("cellmotilityresults.dat")
{}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellMotilityWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    return pCell->HasCellProperty<MotileCellProperty>();
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellMotilityWriter<ELEMENT_DIM, SPACE_DIM>::VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    *this->mpOutStream << pCellPopulation->GetLocationIndexUsingCell(pCell) << " ";

    c_vector<double, SPACE_DIM> cell_location = pCellPopulation->GetLocationOfCellCentre(pCell);
    for (unsigned i=0; i<SPACE_DIM; i++)
    {
        *this->mpOutStream << cell_location[i] << " ";
    }

    *this->mpOutStream << pCell->HasCellProperty<MotileCellProperty>() << " ";
}

// #include "SerializationExportWrapper.hpp"
// EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellMotilityWriter)
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellMotilityWriter)
