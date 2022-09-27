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

class TestBasicMeshGhost: public AbstractCellBasedTestSuite
{
    public:
    void TestMeshBasedMonolayerWithGhostNodes()
    {
        HoneycombMeshGenerator generator(2, 2, 2); //**Changed**//
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();//**Changed**//
        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<TysonNovakCellCycleModel, 2> cells_generator; //**Changed**//
        cells_generator.GenerateBasicRandom(cells, location_indices.size(), p_transit_type); //**Changed**//

        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices); //**Changed**//

        cell_population.AddPopulationWriter<VoronoiDataWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellBasedDemo4"); //**Changed**//
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(2.0); //**Changed**//

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
        simulator.AddForce(p_force);
        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 16u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 2.0, 1e-10);
    }
}