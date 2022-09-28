#ifndef TESTNODEDELTANOTCH_HPP_
#define TESTNODEDELTANOTCH_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "WildTypeCellMutationState.hpp"
#include "CellAgesWriter.hpp"
#include "CellIdWriter.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "CellProliferativePhasesCountWriter.hpp"
#include "CellProliferativeTypesCountWriter.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "DeltaNotchSrnModel.hpp"
#include "DeltaNotchTrackingModifier.hpp"

#include "PetscSetupAndFinalize.hpp"

class TestNodeDeltaNotch : public AbstractCellBasedTestSuite
{
public:

    void TestNodeBasedMonolayerWithDeltaNotch()
    {
        EXIT_IF_PARALLEL;

        HoneycombMeshGenerator generator(5, 5);
        MutableMesh<2,2>* p_generating_mesh = generator.GetMesh();
        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, 1.5);

        std::vector<CellPtr> cells;
        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(DifferentiatedCellProliferativeType, p_diff_type);
        for (unsigned i=0; i<mesh.GetNumNodes(); i++)
        {
            UniformG1GenerationalCellCycleModel* p_cc_model = new UniformG1GenerationalCellCycleModel();
            p_cc_model->SetDimension(2);

            std::vector<double> initial_conditions;
            initial_conditions.push_back(RandomNumberGenerator::Instance()->ranf());
            initial_conditions.push_back(RandomNumberGenerator::Instance()->ranf());
            DeltaNotchSrnModel* p_srn_model = new DeltaNotchSrnModel();
            p_srn_model->SetInitialConditions(initial_conditions);

            CellPtr p_cell(new Cell(p_state, p_cc_model, p_srn_model));
            p_cell->SetCellProliferativeType(p_diff_type);
            double birth_time = -RandomNumberGenerator::Instance()->ranf()*12.0;
            p_cell->SetBirthTime(birth_time);
            cells.push_back(p_cell);
        }

        NodeBasedCellPopulation<2> cell_population(mesh, cells);
        cell_population.AddCellPopulationCountWriter<CellProliferativeTypesCountWriter>();
        cell_population.AddCellPopulationCountWriter<CellMutationStatesCountWriter>();
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellPopulationCountWriter<CellProliferativePhasesCountWriter>();
        cell_population.AddCellWriter<CellAgesWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestNodeBasedMonolayerWithDeltaNotch");
        simulator.SetSamplingTimestepMultiple(10);
        simulator.SetEndTime(5.0);

        MAKE_PTR(DeltaNotchTrackingModifier<2>, p_modifier);
        simulator.AddSimulationModifier(p_modifier);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
        p_force->SetCutOffLength(1.5);
        simulator.AddForce(p_force);

        simulator.Solve();
    }
};

#endif // TESTNODEDELTANOTCH_HPP_