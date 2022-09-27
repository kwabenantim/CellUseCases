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

class TestMeshPeriodic: public AbstractCellBasedTestSuite
{
    public:
    void TestMeshBasedMonolayerPeriodic()
    {
        CylindricalHoneycombMeshGenerator generator(5, 2, 2); //**Changed**//
        Cylindrical2dMesh* p_mesh = generator.GetCylindricalMesh(); //**Changed**//

        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();
        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator; //**Changed**//
        cells_generator.GenerateBasicRandom(cells, location_indices.size(), p_transit_type);

        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices);

        cell_population.AddPopulationWriter<VoronoiDataWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellBasedDemo5"); //**Changed**//
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(20.0); //**Changed**//

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
        simulator.AddForce(p_force);

        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 29u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
};