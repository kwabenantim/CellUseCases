/*

Copyright (c) 2005-2022, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#ifndef TESTNEWCELLMUTATIONSTATE_HPP_
#define TESTNEWCELLMUTATIONSTATE_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "CheckpointArchiveTypes.hpp"
#include "SmartPointers.hpp"

#include "AbstractCellMutationState.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "CellsGenerator.hpp"

#include "P53GainOfFunctionCellMutationState.hpp"

#include "FakePetscSetup.hpp"

class TestNewCellMutationState : public AbstractCellBasedTestSuite
{
public:

    void TestP53GainOfFunctionCellMutationState()
    {
        MAKE_PTR(P53GainOfFunctionCellMutationState, p_state);

        TS_ASSERT_EQUALS(p_state->GetCellCount(), 0u);
        p_state->IncrementCellCount();
        TS_ASSERT_EQUALS(p_state->GetCellCount(), 1u);
        p_state->DecrementCellCount();
        TS_ASSERT_EQUALS(p_state->GetCellCount(), 0u);
        TS_ASSERT_THROWS_THIS(p_state->DecrementCellCount(),
                "Cannot decrement cell count: no cells have this cell property");

        TS_ASSERT_EQUALS(p_state->GetColour(), 5u);

        TS_ASSERT_EQUALS(p_state->IsType<WildTypeCellMutationState>(), false);
        TS_ASSERT_EQUALS(p_state->IsType<P53GainOfFunctionCellMutationState>(), true);

        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "p53_mutation.arch";

        {
            AbstractCellProperty* const p_const_state = new P53GainOfFunctionCellMutationState();
            p_const_state->IncrementCellCount();

            TS_ASSERT_EQUALS(p_const_state->GetCellCount(), 1u);
            TS_ASSERT_EQUALS(dynamic_cast<AbstractCellMutationState*>(p_const_state)->GetColour(), 5u);

            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);

            output_arch << p_const_state;

            delete p_const_state;
        }

        {
            AbstractCellProperty* p_arch_state;

            std::ifstream ifs(archive_filename.c_str());
            boost::archive::text_iarchive input_arch(ifs);

            input_arch >> p_arch_state;

            TS_ASSERT_EQUALS(p_arch_state->GetCellCount(), 1u);

            P53GainOfFunctionCellMutationState* p_real_state = dynamic_cast<P53GainOfFunctionCellMutationState*>(p_arch_state);
            TS_ASSERT(p_real_state != NULL);
            TS_ASSERT_EQUALS(p_real_state->GetColour(), 5u);

            delete p_arch_state;
        }
    }

    void TestOffLatticeSimulationWithP53GainOfFunctionCellMutationState()
    {
        HoneycombMeshGenerator generator(10, 10);
        MutableMesh<2,2>* p_mesh = generator.GetCircularMesh(5);

        MAKE_PTR(P53GainOfFunctionCellMutationState, p_state);

        std::vector<CellPtr> cells;
        CellsGenerator<FixedG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumNodes());

        cells[10]->SetMutationState(p_state);
        cells[50]->SetMutationState(p_state);

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        cell_population.AddCellPopulationCountWriter<CellMutationStatesCountWriter>();

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestOffLatticeSimulationWithNewMutationState");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(10.0);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(3);
        simulator.AddForce(p_linear_force);

        simulator.Solve();
    }
};

#endif // TESTNEWCELLMUTATIONSTATE_HPP_