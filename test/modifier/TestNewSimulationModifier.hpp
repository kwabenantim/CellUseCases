#ifndef TESTNEWSIMULATIONMODIFIER_HPP_
#define TESTNEWSIMULATIONMODIFIER_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractCellBasedSimulationModifier.hpp"
#include "AbstractForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "NodesOnlyMesh.hpp"
#include "CellsGenerator.hpp"
#include "UniformCellCycleModel.hpp"
#include "TransitCellProliferativeType.hpp"
#include "RepulsionForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"

#include "CellHeightTrackingModifier.hpp"

//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

class TestNewSimulationModifier : public AbstractCellBasedTestSuite
{
public:

    void TestOffLatticeSimulationWithCellHeightTrackingModifier()
    {
        HoneycombMeshGenerator generator(2, 2, 0);
        TetrahedralMesh<2,2>* p_generating_mesh = generator.GetMesh();
        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, 1.5);

        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_transit_type);

        NodeBasedCellPopulation<2> cell_population(mesh, cells);

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestOffLatticeSimulationWithCellHeightTrackingModifier");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(20.0);

        MAKE_PTR(RepulsionForce<2>, p_force);
        simulator.AddForce(p_force);

        MAKE_PTR(CellHeightTrackingModifier, p_modifier);
        simulator.AddSimulationModifier(p_modifier);

        simulator.Solve();
    }
};

#endif // TESTNEWSIMULATIONMODIFIER_HPP_