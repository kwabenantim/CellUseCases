#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "SmartPointers.hpp"
#include "Exception.hpp"

#include "AbstractSimpleGenerationalCellCycleModel.hpp"

#include "CheckReadyToDivideAndPhaseIsUpdated.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

class TestNewCellCycleModel : public AbstractCellBasedTestSuite
{
public:

    void TestMyCellCycleModel()
    {
        TS_ASSERT_THROWS_NOTHING(MyCellCycleModel cell_model3);

        unsigned num_cells = (unsigned) 1e5;
        std::vector<CellPtr> cells;
        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(StemCellProliferativeType, p_stem_type);
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        for (unsigned i=0; i<num_cells; i++)
        {
            MyCellCycleModel* p_cell_cycle_model = new MyCellCycleModel;
            CellPtr p_cell(new Cell(p_state, p_cell_cycle_model));
            p_cell->SetCellProliferativeType(p_stem_type);
            p_cell->InitialiseCellCycleModel();
            cells.push_back(p_cell);
        }

        double expected_mean_g1_duration = static_cast<MyCellCycleModel*>(cells[0]->GetCellCycleModel())->GetStemCellG1Duration();
        double sample_mean_g1_duration = 0.0;

        for (unsigned i=0; i<num_cells; i++)
        {
            sample_mean_g1_duration += static_cast<MyCellCycleModel*>(cells[i]->GetCellCycleModel())->GetG1Duration()/ (double) num_cells;
        }

        TS_ASSERT_DELTA(sample_mean_g1_duration, expected_mean_g1_duration, 0.1);

        MyCellCycleModel* p_my_model = new MyCellCycleModel;
        CellPtr p_my_cell(new Cell(p_state, p_my_model));
        p_my_cell->SetCellProliferativeType(p_transit_type);
        p_my_cell->InitialiseCellCycleModel();

        unsigned num_steps = 100;
        double mean_cell_cycle_time = p_my_model->GetTransitCellG1Duration()
                                        + p_my_model->GetSG2MDuration();

        SimulationTime::Instance()->SetEndTimeAndNumberOfTimeSteps(mean_cell_cycle_time, num_steps);

        for (unsigned i=0; i<num_steps; i++)
        {
            SimulationTime::Instance()->IncrementTimeOneStep();

            CheckReadyToDivideAndPhaseIsUpdated(p_my_model, 2.35762);
        }

        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "my_cell_cycle_model.arch";

        {
            SimulationTime::Destroy();
            SimulationTime::Instance()->SetStartTime(0.0);
            SimulationTime* p_simulation_time = SimulationTime::Instance();
            p_simulation_time->SetEndTimeAndNumberOfTimeSteps(3.0, 4);

            MyCellCycleModel* p_model = new MyCellCycleModel;
            CellPtr p_cell(new Cell(p_state, p_model));
            p_cell->SetCellProliferativeType(p_transit_type);
            p_cell->InitialiseCellCycleModel();

            p_simulation_time->IncrementTimeOneStep();
            p_simulation_time->IncrementTimeOneStep();

            p_model->SetBirthTime(-1.0);
            p_model->ReadyToDivide();

            TS_ASSERT_EQUALS(p_model->GetCurrentCellCyclePhase(), S_PHASE);

            CellPtr const p_const_cell = p_cell;

            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);
            output_arch << p_const_cell;
        }

        {
            SimulationTime::Destroy();
            SimulationTime* p_simulation_time = SimulationTime::Instance();
            p_simulation_time->SetStartTime(0.0);
            p_simulation_time->SetEndTimeAndNumberOfTimeSteps(1.0, 1);

            CellPtr p_cell;

            std::ifstream ifs(archive_filename.c_str(), std::ios::binary);
            boost::archive::text_iarchive input_arch(ifs);

            input_arch >> p_cell;

            MyCellCycleModel* p_model = static_cast<MyCellCycleModel*>(p_cell->GetCellCycleModel());

            TS_ASSERT_DELTA(p_model->GetBirthTime(), -1.0, 1e-12);
            TS_ASSERT_DELTA(p_model->GetAge(), 2.5, 1e-12);
            TS_ASSERT_EQUALS(p_model->GetCurrentCellCyclePhase(), S_PHASE);
        }
    }

    void TestOffLatticeSimulationWithMyCellCycleModel()
    {
        HoneycombMeshGenerator generator(10, 10, 0);
        MutableMesh<2,2>* p_mesh = generator.GetCircularMesh(5);

        std::vector<CellPtr> cells;
        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(StemCellProliferativeType, p_stem_type);
        for (unsigned i=0; i<p_mesh->GetNumNodes(); i++)
        {
            MyCellCycleModel* p_model = new MyCellCycleModel();
            CellPtr p_cell(new Cell(p_state, p_model));
            p_cell->SetCellProliferativeType(p_stem_type);

            double birth_time = - RandomNumberGenerator::Instance()->ranf() * (p_model->GetStemCellG1Duration() + p_model->GetSG2MDuration());
            p_cell->SetBirthTime(birth_time);
            cells.push_back(p_cell);
        }

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestOffLatticeSimulationWithMyCellCycleModel");
        simulator.SetEndTime(10.0);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(3);
        simulator.AddForce(p_linear_force);

        simulator.Solve();
    }
};