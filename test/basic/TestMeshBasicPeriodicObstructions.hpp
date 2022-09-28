#ifndef TESTMESHBASICPERIODICOBSTRUCTIONS_HPP_
#define TESTMESHBASICPERIODICOBSTRUCTIONS_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"

#include "CellsGenerator.hpp"
#include "CylindricalHoneycombMeshGenerator.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "OffLatticeSimulation.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "VoronoiDataWriter.hpp"

#include "FakePetscSetup.hpp"

class TestMeshBasicPeriodicObstructions: public AbstractCellBasedTestSuite
{
    public:
    void TestMeshBasedMonolayerPeriodicSolidBottomBoundary()
    {
        CylindricalHoneycombMeshGenerator generator(5, 2, 2);
        Cylindrical2dMesh* p_mesh = generator.GetCylindricalMesh();

        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();
        std::vector<CellPtr> cells;
        MAKE_PTR(StemCellProliferativeType, p_stem_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, location_indices.size(), p_stem_type); 

        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices);
        cell_population.AddPopulationWriter<VoronoiDataWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("CellBasedDemo6"); 
        simulator.SetSamplingTimestepMultiple(50);
        simulator.SetEndTime(20.0);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
        simulator.AddForce(p_force);

        c_vector<double,2> point = zero_vector<double>(2);
        c_vector<double,2> normal = zero_vector<double>(2);
        normal(1) = -1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc);

        simulator.Solve();

        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 23u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
};

#endif // TESTMESHBASICPERIODICOBSTRUCTIONS_HPP_