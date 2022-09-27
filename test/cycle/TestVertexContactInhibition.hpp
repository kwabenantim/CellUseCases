#ifndef TESTVERTEXCONTACTINHIBITION_HPP_
#define TESTVERTEXCONTACTINHIBITION_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "SmartPointers.hpp"
#include "Exception.hpp"

#include "ContactInhibitionCellCycleModel.hpp"

#include "VolumeTrackingModifier.hpp"

#include "OffLatticeSimulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "UniformCellCycleModel.hpp"
#include "WildTypeCellMutationState.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "OutputFileHandler.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "SimulationTime.hpp"
#include "CellLabel.hpp"
#include "MutableMesh.hpp"
#include "MutableVertexMesh.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "FakePetscSetup.hpp"

class TestVertexContactInhibition : public AbstractCellBasedTestSuite
{
public:
    void TestContactInhibitionWithVertex()
    {
        HoneycombVertexMeshGenerator generator(2, 2);
        MutableVertexMesh<2,2>* p_mesh = generator.GetMesh();

        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        std::vector<CellPtr> cells;
        for (unsigned i=0; i<p_mesh->GetNumElements(); i++)
        {
            ContactInhibitionCellCycleModel* p_cycle_model = new ContactInhibitionCellCycleModel();
            p_cycle_model->SetDimension(2);
            p_cycle_model->SetBirthTime(-(double)i - 2.0); // So all out of M phase
            p_cycle_model->SetQuiescentVolumeFraction(0.9);
            p_cycle_model->SetEquilibriumVolume(1.0);

            CellPtr p_cell(new Cell(p_state, p_cycle_model));
            p_cell->SetCellProliferativeType(p_transit_type);
            p_cell->InitialiseCellCycleModel();
            cells.push_back(p_cell);
        }

        VertexBasedCellPopulation<2> cell_population(*p_mesh, cells);
        cell_population.AddCellPopulationCountWriter<CellMutationStatesCountWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestVertexContactInhibition");
        simulator.SetSamplingTimestepMultiple(50);
        simulator.SetEndTime(10.0);

        MAKE_PTR(VolumeTrackingModifier<2>, p_modifier);
        simulator.AddSimulationModifier(p_modifier);

        MAKE_PTR(NagaiHondaForce<2>, p_nagai_honda_force);
        simulator.AddForce(p_nagai_honda_force);

        MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
        simulator.AddSimulationModifier(p_growth_modifier);

        simulator.Solve();
    }
};

#endif // TESTVERTEXCONTACTINHIBITION_HPP_