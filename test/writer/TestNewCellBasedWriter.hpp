#ifndef TESTNEWCELLBASEDWRITER_HPP_
#define TESTNEWCELLBASEDWRITER_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "NodesOnlyMesh.hpp"
#include "WildTypeCellMutationState.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "CellLabel.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"

#include "MotileCellProperty.hpp"
#include "CellMotilityWriter.hpp"

#include "PetscSetupAndFinalize.hpp"

class TestNewCellBasedWriter : public AbstractCellBasedTestSuite
{
public:

    void TestOffLatticeSimulationWithMotileCellPropertyAndWriters()
    {
        EXIT_IF_PARALLEL;

        HoneycombMeshGenerator generator(10, 10);
        MutableMesh<2,2>* p_generating_mesh = generator.GetCircularMesh(5);

        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, 1.5);

        MAKE_PTR(MotileCellProperty, p_motile);
        MAKE_PTR(CellLabel, p_label);
        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(DifferentiatedCellProliferativeType, p_diff_type);

        std::vector<CellPtr> cells;
        for (unsigned i=0; i<mesh.GetNumNodes(); i++)
        {
            FixedG1GenerationalCellCycleModel* p_model = new FixedG1GenerationalCellCycleModel();

            CellPropertyCollection collection;
            if (RandomNumberGenerator::Instance()->ranf() < 0.2)
            {
                collection.AddProperty(p_motile);
                collection.AddProperty(p_label);
            }

            CellPtr p_cell(new Cell(p_state, p_model, NULL, false, collection));
            p_cell->SetCellProliferativeType(p_diff_type);

            double birth_time = - RandomNumberGenerator::Instance()->ranf() *
                                    (p_model->GetStemCellG1Duration() + p_model->GetSG2MDuration());

            p_cell->SetBirthTime(birth_time);
            cells.push_back(p_cell);
        }

        NodeBasedCellPopulation<2> cell_population(mesh, cells);

        cell_population.AddCellWriter<CellMotilityWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestOffLatticeSimulationWithMotileCellPropertyAndWriters");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(10.0);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        simulator.Solve();
    }
};

#endif // TESTNEWCELLBASEDWRITER_HPP_