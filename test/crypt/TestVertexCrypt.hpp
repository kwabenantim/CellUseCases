#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "CellsGenerator.hpp"
#include "CryptCellsGenerator.hpp"
#include "WntConcentration.hpp"
#include "SloughingCellKiller.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"
#include "FakePetscSetup.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "SimpleWntCellCycleModel.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "CylindricalHoneycombVertexMeshGenerator.hpp"
#include "CryptSimulation2d.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"

class TestVertexCrypt : public AbstractCellBasedTestSuite
{
public:

    void TestVertexBasedCrypt()
    {
         CylindricalHoneycombVertexMeshGenerator generator(6, 9);
         Cylindrical2dVertexMesh* p_mesh = generator.GetCylindricalMesh();

        std::vector<CellPtr> cells;
        CryptCellsGenerator<FixedG1GenerationalCellCycleModel> cells_generator;
        cells_generator.Generate(cells, p_mesh, std::vector<unsigned>(), true, 1.0, 2.0, 3.0, 4.0);

        VertexBasedCellPopulation<2> crypt(*p_mesh, cells);

        CryptSimulation2d simulator(crypt);
        simulator.SetOutputDirectory("VertexCrypt");
        simulator.SetEndTime(0.1);

        MAKE_PTR(NagaiHondaForce<2>, p_force);
        simulator.AddForce(p_force);

        MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
        simulator.AddSimulationModifier(p_growth_modifier);

        double crypt_length = 6.0;
        MAKE_PTR_ARGS(SloughingCellKiller<2>, p_killer, (&crypt, crypt_length));
        simulator.AddCellKiller(p_killer);

        simulator.Solve();
    }

    void TestVertexBasedCryptWithSimpleWntCellCycleModel()
    {
        CylindricalHoneycombVertexMeshGenerator generator(6, 9);
        Cylindrical2dVertexMesh* p_mesh = generator.GetCylindricalMesh();

        std::vector<CellPtr> cells;
        CryptCellsGenerator<SimpleWntCellCycleModel> cells_generator;
        cells_generator.Generate(cells, p_mesh, std::vector<unsigned>(), true);

        VertexBasedCellPopulation<2> crypt(*p_mesh, cells);

        double crypt_length = 6.0;

        WntConcentration<2>::Instance()->SetType(LINEAR);
        WntConcentration<2>::Instance()->SetCellPopulation(crypt);
        WntConcentration<2>::Instance()->SetCryptLength(crypt_length);

        CryptSimulation2d simulator(crypt);
        simulator.SetOutputDirectory("VertexCryptWithSimpleWntCellCycleModel");
        simulator.SetEndTime(0.1);

        MAKE_PTR(NagaiHondaForce<2>, p_force);
        simulator.AddForce(p_force);

        MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
        simulator.AddSimulationModifier(p_growth_modifier);

        MAKE_PTR_ARGS(SloughingCellKiller<2>, p_killer, (&crypt, crypt_length));
        simulator.AddCellKiller(p_killer);

        simulator.UseJiggledBottomCells();

        simulator.Solve();
    }
};