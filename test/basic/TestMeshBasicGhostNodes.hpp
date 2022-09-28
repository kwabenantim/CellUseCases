#ifndef TESTMESHBASICGHOSTNODES_HPP_
#define TESTMESHBASICGHOSTNODES_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"

#include "CellsGenerator.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "OffLatticeSimulation.hpp"
#include "TysonNovakCellCycleModel.hpp"
#include "VoronoiDataWriter.hpp"

#include "FakePetscSetup.hpp"

class TestMeshBasicGhostNodes: public AbstractCellBasedTestSuite
{
    public:
    void TestMeshBasedMonolayerWithGhostNodes()
    {
        HoneycombMeshGenerator generator(2, 2, 2); 
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();
        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<TysonNovakCellCycleModel, 2> cells_generator; 
        cells_generator.GenerateBasicRandom(cells, location_indices.size(), p_transit_type); 

        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices); 

        cell_population.AddPopulationWriter<VoronoiDataWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellBasedDemo4"); 
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(2.0); 

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
        simulator.AddForce(p_force);
        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 16u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 2.0, 1e-10);
    }
};

#endif // TESTMESHBASICGHOSTNODES_HPP_