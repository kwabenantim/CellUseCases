#ifndef TESTMESHCRYPTWNT_HPP_
#define TESTMESHCRYPTWNT_HPP_

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

class TestMeshCryptWnt : public AbstractCellBasedTestSuite
{
public:
    void TestCryptWithWntCellCycle()
    {
        CylindricalHoneycombMeshGenerator generator(6, 9, 2);
        Cylindrical2dMesh* p_mesh = generator.GetCylindricalMesh();

        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();

        std::vector<CellPtr> cells;
        CryptCellsGenerator<WntCellCycleModel> cells_generator;
        cells_generator.Generate(cells, p_mesh, location_indices, true);

        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices);

        double crypt_height = 8.0;

        WntConcentration<2>::Instance()->SetType(LINEAR);
        WntConcentration<2>::Instance()->SetCellPopulation(cell_population);
        WntConcentration<2>::Instance()->SetCryptLength(crypt_height);

        CryptSimulation2d simulator(cell_population);
        simulator.SetOutputDirectory("CryptTutorialWntCellCycle");
        simulator.SetEndTime(1);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        simulator.AddForce(p_linear_force);
        MAKE_PTR_ARGS(SloughingCellKiller<2>, p_killer, (&cell_population, crypt_height));
        simulator.AddCellKiller(p_killer);

        simulator.Solve();

        WntConcentration<2>::Destroy();
    }
};

#endif // TESTMESHCRYPTWNT_HPP_