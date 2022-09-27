#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "UniformCellCycleModel.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "CylindricalHoneycombMeshGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "CellsGenerator.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "OffLatticeSimulation.hpp"
#include "TransitCellProliferativeType.hpp"
#include "SmartPointers.hpp"
#include "VoronoiDataWriter.hpp"

#include "FakePetscSetup.hpp"

class TestMeshPeriodicParaview : public AbstractCellBasedTestSuite
{
public:
    void Test2DPeriodicMeshBasedMonolayerSimulationForVisualizing()
    {
        CylindricalHoneycombMeshGenerator generator(10, 10, 2);
        Cylindrical2dMesh* p_mesh = generator.GetCylindricalMesh();
        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();

        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, location_indices.size(), p_transit_type);

        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices);

        cell_population.SetWriteVtkAsPoints(false);
        cell_population.AddPopulationWriter<VoronoiDataWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("Test2DPeriodicMeshBasedMonolayerSimulationForVisualizing");
        simulator.SetEndTime(1.0);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 108u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 1.0, 1e-10);
    }
};
