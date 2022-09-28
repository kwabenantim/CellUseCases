#ifndef TESTMESHCONTACTINHIBITIONMUTANTS_HPP_
#define TESTMESHCONTACTINHIBITIONMUTANTS_HPP_

#include <cxxtest/TestSuite.h>
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
#include "GeneralisedLinearSpringForce.hpp"
#include "MutableMesh.hpp"
#include "PlaneBoundaryCondition.hpp"

#include "FakePetscSetup.hpp"

class TestMeshContactInhibitionMutants : public AbstractCellBasedTestSuite
{
public:
    void TestContactInhibitionInBoxWithMutants()
    {
        HoneycombMeshGenerator generator(3, 3);
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(StemCellProliferativeType, p_stem_type);
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        std::vector<CellPtr> cells;
        for (unsigned i=0; i<p_mesh->GetNumNodes(); i++)
        {
            if (i==1)
            {
                UniformCellCycleModel* p_cycle_model = new UniformCellCycleModel();
                p_cycle_model->SetBirthTime(-14.0);

                CellPtr p_cell(new Cell(p_state, p_cycle_model));
                p_cell->SetCellProliferativeType(p_stem_type);
                p_cell->SetBirthTime(0.0);
                cells.push_back(p_cell);
            }
            else
            {
                ContactInhibitionCellCycleModel* p_cycle_model = new ContactInhibitionCellCycleModel();
                p_cycle_model->SetDimension(2);
                p_cycle_model->SetBirthTime(-2.0*(double)i);
                p_cycle_model->SetQuiescentVolumeFraction(0.8);
                p_cycle_model->SetEquilibriumVolume(1.0);

                CellPtr p_cell(new Cell(p_state, p_cycle_model));
                p_cell->SetCellProliferativeType(p_transit_type);
                p_cell->InitialiseCellCycleModel();
                cells.push_back(p_cell);
            }
        }

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        cell_population.AddCellPopulationCountWriter<CellMutationStatesCountWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestContactInhibitionTumourInBox");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(20.0);

        MAKE_PTR(VolumeTrackingModifier<2>, p_modifier);
        simulator.AddSimulationModifier(p_modifier);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
        p_force->SetCutOffLength(1.5);
        simulator.AddForce(p_force);

        c_vector<double,2> point = zero_vector<double>(2);
        c_vector<double,2> normal = zero_vector<double>(2);
        normal(0) = -1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc1, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc1);

        point(0) = 2.5;
        normal(0) = 1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc2, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc2);

        point(0) = 0.0;
        point(1) = 0.0;
        normal(0) = 0.0;
        normal(1) = -1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc3, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc3);

        point(1) = 2.5;
        normal(1) = 1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc4, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc4);

        simulator.Solve();
    }
};

#endif // TESTMESHCONTACTINHIBITIONMUTANTS_HPP_