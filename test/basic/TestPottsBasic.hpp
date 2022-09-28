#ifndef TESTPOTTSBASIC_HPP_
#define TESTPOTTSBASIC_HPP_

#include <cxxtest/TestSuite.h>
#include "SmartPointers.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "AdhesionPottsUpdateRule.hpp"
#include "CellsGenerator.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "OnLatticeSimulation.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "PottsMeshGenerator.hpp"
#include "RandomCellKiller.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "SurfaceAreaConstraintPottsUpdateRule.hpp"
#include "VolumeConstraintPottsUpdateRule.hpp"

#include "FakePetscSetup.hpp"

class TestPottsBasic : public AbstractCellBasedTestSuite
{
    public:
    void TestPottsBasedMonolayer()
    {
        PottsMeshGenerator<2> generator(20, 2, 4, 20, 2, 4); 
        PottsMesh<2>* p_mesh = generator.GetMesh(); 

        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_transit_type);

        PottsBasedCellPopulation<2> cell_population(*p_mesh, cells);
        cell_population.SetTemperature(1.0);

        OnLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellBasedDemo7"); 
        simulator.SetEndTime(20.0);

        MAKE_PTR(VolumeConstraintPottsUpdateRule<2>, p_volume_constraint_update_rule); 
        simulator.AddUpdateRule(p_volume_constraint_update_rule); 
        MAKE_PTR(SurfaceAreaConstraintPottsUpdateRule<2>, p_surface_area_update_rule); 
        simulator.AddUpdateRule(p_surface_area_update_rule); 
        MAKE_PTR(AdhesionPottsUpdateRule<2>, p_adhesion_update_rule); 
        simulator.AddUpdateRule(p_adhesion_update_rule); 

        MAKE_PTR_ARGS(RandomCellKiller<2>, p_cell_killer, (&cell_population, 0.01));
        simulator.AddCellKiller(p_cell_killer);

        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 16u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
};

#endif // TESTPOTTSBASIC_HPP_