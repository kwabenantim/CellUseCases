#ifndef TESTNODEBASIC_HPP_
#define TESTNODEBASIC_HPP_

#include <cxxtest/TestSuite.h>
#include "SmartPointers.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "CellsGenerator.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "RandomCellKiller.hpp"
#include "RepulsionForce.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"

#include "FakePetscSetup.hpp"

class TestNodeBasic : public AbstractCellBasedTestSuite
{
    public:
    // In a "node-based" simulation, cells are represented by overlapping spheres
    // (these are actually circles in 2D).
    // This is a simple node-based simulation of an epithelial monolayer.
    void TestNodeBasedMonolayer()
    {
        // Create a 2D NodesOnlyMesh to hold the spatial information of the simulation
        HoneycombMeshGenerator generator(2, 2); // Generates generic meshes
        MutableMesh<2,2>* p_generating_mesh = generator.GetMesh(); // Make a generic mesh

        // Make a mesh which is not really a mesh but only has nodes in it
        NodesOnlyMesh<2> mesh;

        // Put nodes in the generic mesh
        // The "interaction cut off length" of 1.5 defines connectivity in the mesh.
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, 1.5); 

        // Prepare storage for a collection of cells
        std::vector<CellPtr> cells;

        // Specify the proliferative behaviour of the cell by choosing a CellCycleModel
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;

        // For a node-based simulation, num_cells == num_nodes
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_transit_type);

        // Create a 2-dimensinal CellPopulation object to connect the mesh and the cells.
        NodeBasedCellPopulation<2> cell_population(mesh, cells);

        // Create an OffLatticeSimulation with the CellPopulation
        OffLatticeSimulation<2> simulator(cell_population);

        // Set some simulation options
        simulator.SetOutputDirectory("NodeBasedMonolayer");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(20.0);

        // Add a Force object to specify how cells move around
        // A RepulsionForce is suitable for node-based simulations
        MAKE_PTR(RepulsionForce<2>, p_force);
        simulator.AddForce(p_force);

        // A CellKiller can remove cells from any simulation type.
        // MAKE_PTR_ARGS wraps boost::shared_ptr in global/src/SmartPointers.hpp
        MAKE_PTR_ARGS(RandomCellKiller<2>, p_cell_killer, (&cell_population, 0.01));
        simulator.AddCellKiller(p_cell_killer);

        // Run the simulation
        simulator.Solve();

        // Checking that we have the correct number of cells at the end.
        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 7u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
};

#endif // TESTNODEBASIC_HPP_