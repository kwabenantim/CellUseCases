#include "SmartPointers.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "CellsGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "OffLatticeSimulation.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "VertexBasedCellPopulation.hpp"

// AbstractCellBasedTestSuite : public CxxTest::TestSuite sets up and destroys some singleton objects
// eg. SimulationTime, RandomNumberGenerator, and CellPropertyRegistry.
//   - SimulationTime is initialised to zero at the beginning and destroyed at the end.
//   - RandomNumberGenerator is seeded with zero at the beginning and destroyed at the end.
//   - CellPropertyRegistry stores CellProperties. It is cleared at the beginning of the test.

class TestBasicVertex : public AbstractCellBasedTestSuite
{
    public:
    // In a "vertex-based" simulation, cells are represented by polygons.
    // This is a simple vertex-based simulation of an epithelial monolayer.
    void TestVertexBasedMonolayer()
    {
        // Create a 2D mesh to hold the spatial information of the simulation
        HoneycombVertexMeshGenerator generator(2, 2); // Generates only vertex meshes
        MutableVertexMesh<2,2>* p_mesh = generator.GetMesh(); // Make a vertex mesh

        // Prepare storage for a collection of cells
        std::vector<CellPtr> cells;

        // Specify the proliferative behaviour of the cell by choosing a CellCycleModel:
        //   - With the UniformG1GenerationalCellCycleModel, each cell is given a division time
        //     drawn from a uniform distribution when it is created.
        //     This is a simple stochastic cell-cycle model.
        //     The cells will divide randomly and never stop proliferating.
        //   - Vertex based simulations need "target area modifiers". Phase based cell cycle models 
        //     such as the chosen one allow us to use such "target area modifiers".
        //   - MAKE_PTR is a macro wrapping boost::shared_ptr. See global/src/SmartPointers.hpp
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;

        // Generate the collection of cells
        // For a vertex-based simulation, num_cells == num_mesh_elements
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_transit_type);

        // Create a 2-dimensinal CellPopulation object to connect the mesh and the cells.
        VertexBasedCellPopulation<2> cell_population(*p_mesh, cells);

        // Create an OffLatticeSimulation with the CellPopulation
        OffLatticeSimulation<2> simulator(cell_population);

        // Set some simulation options
        simulator.SetOutputDirectory("VertexBasedMonolayer");
        simulator.SetSamplingTimestepMultiple(200);
        simulator.SetEndTime(20.0);

        // Create a Force object to specify how cells move around
        MAKE_PTR(NagaiHondaForce<2>, p_force);

        // Pass the Force object to the OffLatticeSimulation
        simulator.AddForce(p_force);

        // A NagaiHondaForce needs an AbstractTargetAreaModifier.
        // "Target area modifiers" can modify the target area of individual cells
        // and thus alter the relative forces between neighbouring cells.
        MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
        simulator.AddSimulationModifier(p_growth_modifier);

        // Run the simulation
        simulator.Solve();

        // Checking that we have the correct number of cells at the end.
        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 16u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
}
