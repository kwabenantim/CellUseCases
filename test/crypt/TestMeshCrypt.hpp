#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "SmartPointers.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "CryptCellsGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "WntCellCycleModel.hpp"
#include "CylindricalHoneycombMeshGenerator.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "CryptSimulation2d.hpp"
#include "WntConcentration.hpp"
#include "SloughingCellKiller.hpp"

#include "FakePetscSetup.hpp"

class TestMeshCrypt : public AbstractCellBasedTestSuite
{
public:
    void TestCryptWithFixedCellCycle()
    {
        CylindricalHoneycombMeshGenerator generator(6, 9, 2);
        Cylindrical2dMesh* p_mesh = generator.GetCylindricalMesh();
        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();

        std::vector<CellPtr> cells;
        CryptCellsGenerator<FixedG1GenerationalCellCycleModel> cells_generator;
        cells_generator.Generate(cells, p_mesh, location_indices, true);

        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices);

        CryptSimulation2d simulator(cell_population);
        simulator.SetOutputDirectory("CryptTutorialFixedCellCycle");
        simulator.SetEndTime(1);
        simulator.SetSamplingTimestepMultiple(12);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        simulator.AddForce(p_linear_force);

        double crypt_height = 8.0;
        MAKE_PTR_ARGS(SloughingCellKiller<2>, p_killer, (&cell_population, crypt_height));
        simulator.AddCellKiller(p_killer);

        simulator.Solve();
    }
};