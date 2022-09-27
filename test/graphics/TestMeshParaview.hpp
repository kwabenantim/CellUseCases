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

class TestMeshParaview : public AbstractCellBasedTestSuite
{
public:
    void Test2DMeshBasedMonolayerSimulationForVisualizing()
    {
        HoneycombMeshGenerator generator(10, 10);
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumNodes(), p_transit_type);

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        cell_population.AddPopulationWriter<VoronoiDataWriter>();

        cell_population.SetWriteVtkAsPoints(true);

        cell_population.SetBoundVoronoiTessellation(true);

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("Test2DMeshBasedMonolayerSimulationForVisualizing");
        simulator.SetEndTime(1.0);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 108u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 1.0, 1e-10);
    }
};
