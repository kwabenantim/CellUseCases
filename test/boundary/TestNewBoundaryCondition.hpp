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
#ifndef TESTNEWBOUNDARYCONDITION_HPP_
#define TESTNEWBOUNDARYCONDITION_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "CheckpointArchiveTypes.hpp"
#include "SmartPointers.hpp"

#include "AbstractCellPopulationBoundaryCondition.hpp"
#include "OffLatticeSimulation.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "CellsGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"

#include "NewBoundaryCondition.hpp"

#include "FakePetscSetup.hpp"

class TestNewBoundaryCondition : public AbstractCellBasedTestSuite
{
public:

    void TestBoundaryCondition()
    {
        HoneycombMeshGenerator generator(7, 7);
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        std::vector<CellPtr> cells;
        CellsGenerator<FixedG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasic(cells, p_mesh->GetNumNodes());

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        NewBoundaryCondition bc(&cell_population);

        bool population_satisfies_bc = bc.VerifyBoundaryCondition();
        TS_ASSERT_EQUALS(population_satisfies_bc, false);

        std::map<Node<2>*, c_vector<double, 2> > old_node_locations;
        for (AbstractMesh<2,2>::NodeIterator node_iter = p_mesh->GetNodeIteratorBegin();
                node_iter != p_mesh->GetNodeIteratorEnd();
                ++node_iter)
        {
            old_node_locations[&(*node_iter)] = node_iter->rGetLocation();
        }

        bc.ImposeBoundaryCondition(old_node_locations);

        population_satisfies_bc = bc.VerifyBoundaryCondition();
        TS_ASSERT_EQUALS(population_satisfies_bc, true);

        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "my_bc.arch";
        {
            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);

            AbstractCellPopulationBoundaryCondition<2>* const p_bc = new NewBoundaryCondition(NULL);
            output_arch << p_bc;
            delete p_bc;
        }
        {
            std::ifstream ifs(archive_filename.c_str(), std::ios::binary);
            boost::archive::text_iarchive input_arch(ifs);

            AbstractCellPopulationBoundaryCondition<2>* p_bc;
            input_arch >> p_bc;

            delete p_bc;
        }
    }

    void TestOffLatticeSimulationWithBoundaryCondition()
    {
        HoneycombMeshGenerator generator(7, 7, 0);
        MutableMesh<2,2>* p_mesh = generator.GetMesh();

        std::vector<CellPtr> cells;
        CellsGenerator<FixedG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasic(cells, p_mesh->GetNumNodes());

        MeshBasedCellPopulation<2> cell_population(*p_mesh, cells);

        MAKE_PTR_ARGS(NewBoundaryCondition, p_bc, (&cell_population));

        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestOffLatticeSimulationWithBoundaryCondition");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(1.0);

        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_linear_force);
        p_linear_force->SetCutOffLength(3);
        simulator.AddForce(p_linear_force);

        simulator.AddCellPopulationBoundaryCondition(p_bc);

        simulator.Solve();
    }
};

#endif // TESTNEWBOUNDARYCONDITION_HPP_