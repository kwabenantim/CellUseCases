#include <cxxtest/TestSuite.h>
#include "CellBasedSimulationArchiver.hpp"
#include "SmartPointers.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "AdhesionPottsUpdateRule.hpp"
#include "CellsGenerator.hpp"
#include "CylindricalHoneycombMeshGenerator.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "OnLatticeSimulation.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "PottsMeshGenerator.hpp"
#include "RandomCellKiller.hpp"
#include "RepulsionForce.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "SurfaceAreaConstraintPottsUpdateRule.hpp"
#include "TysonNovakCellCycleModel.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "VolumeConstraintPottsUpdateRule.hpp"
#include "VoronoiDataWriter.hpp"

#include "FakePetscSetup.hpp"

class TestBasicPotts : public AbstractCellBasedTestSuite
{
    public:
    void TestPottsBasedMonolayer()
    {
        PottsMeshGenerator<2> generator(20, 2, 4, 20, 2, 4); //**Changed**//
        PottsMesh<2>* p_mesh = generator.GetMesh(); //**Changed**//

        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_transit_type);

        PottsBasedCellPopulation<2> cell_population(*p_mesh, cells);//**Changed**//
        cell_population.SetTemperature(1.0);

        OnLatticeSimulation<2> simulator(cell_population);//**Changed**//
        simulator.SetOutputDirectory("CellBasedDemo7"); //**Changed**//
        simulator.SetEndTime(20.0);

        MAKE_PTR(VolumeConstraintPottsUpdateRule<2>, p_volume_constraint_update_rule); //**Changed**//
        simulator.AddUpdateRule(p_volume_constraint_update_rule); //**Changed**//
        MAKE_PTR(SurfaceAreaConstraintPottsUpdateRule<2>, p_surface_area_update_rule); //**Changed**//
        simulator.AddUpdateRule(p_surface_area_update_rule); //**Changed**//
        MAKE_PTR(AdhesionPottsUpdateRule<2>, p_adhesion_update_rule); //**Changed**//
        simulator.AddUpdateRule(p_adhesion_update_rule); //**Changed**//

        MAKE_PTR_ARGS(RandomCellKiller<2>, p_cell_killer, (&cell_population, 0.01));
        simulator.AddCellKiller(p_cell_killer);

        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 16u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
}
