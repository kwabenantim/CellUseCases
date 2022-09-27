#ifndef TESTNEWFORCE_HPP_
#define TESTNEWFORCE_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellsGenerator.hpp"
#include "TransitCellProliferativeType.hpp"
#include "SmartPointers.hpp"

#include "MyForce.hpp"

#include "FakePetscSetup.hpp"

class TestNewForce : public AbstractCellBasedTestSuite
{
public:

    void TestMyForce()
    {
        HoneycombMeshGenerator generator(7, 7);
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        std::vector<CellPtr> cells;
        CellsGenerator<FixedG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasic(cells, p_mesh->GetNumNodes());

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        for (unsigned i=0; i<cell_population.GetNumNodes(); i++)
        {
             cell_population.GetNode(i)->ClearAppliedForce();
        }

        MyForce force(5.0);

        force.AddForceContribution(cell_population);

        for (unsigned node_index=0; node_index<cell_population.GetNumNodes(); node_index++)
        {
            TS_ASSERT_DELTA(cell_population.GetNode(node_index)->rGetAppliedForce()[0], 0.0, 1e-4);
            TS_ASSERT_DELTA(cell_population.GetNode(node_index)->rGetAppliedForce()[1], -5.0, 1e-4);
        }

        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "my_force.arch";
        {
            AbstractForce<2>* const p_force = new MyForce(2.6);
            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);

            output_arch << p_force;
            delete p_force;
        }
        {
            std::ifstream ifs(archive_filename.c_str(), std::ios::binary);
            boost::archive::text_iarchive input_arch(ifs);

            AbstractForce<2>* p_force;
            input_arch >> p_force;

            TS_ASSERT_DELTA(dynamic_cast<MyForce*>(p_force)->GetStrength(), 2.6, 1e-4);

            delete p_force;
        }
    }

    void TestOffLatticeSimulationWithMyForce()
    {
        HoneycombMeshGenerator generator(5, 5);
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<FixedG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumNodes(), p_transit_type);

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestOffLatticeSimulationWithMyForce");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(5.0);

        MAKE_PTR_ARGS(MyForce, p_force, (0.5));
        simulator.AddForce(p_force);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(1.5);
        simulator.AddForce(p_linear_force);

        simulator.Solve();
    }
};

#endif // TESTNEWFORCE_HPP_