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
#ifndef TESTVERTEXBASIC_HPP_
#define TESTVERTEXBASIC_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"

#include "CellsGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "OffLatticeSimulation.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "VertexBasedCellPopulation.hpp"

#include "FakePetscSetup.hpp"

// AbstractCellBasedTestSuite : public CxxTest::TestSuite sets up and destroys some singleton objects
// eg. SimulationTime, RandomNumberGenerator, and CellPropertyRegistry.
//   - SimulationTime is initialised to zero at the beginning and destroyed at the end.
//   - RandomNumberGenerator is seeded with zero at the beginning and destroyed at the end.
//   - CellPropertyRegistry stores CellProperties. It is cleared at the beginning of the test.

class TestVertexBasic : public AbstractCellBasedTestSuite
{
    public:
    // In a "vertex-based" simulation, cells are represented by polygons.
    // This is a simple vertex-based simulation of an epithelial monolayer.
    void TestVertexBasedMonolayer()
    {
        // Create a 2D mesh to hold the spatial information of the simulation
        HoneycombVertexMeshGenerator generator(2, 2); // Generates only vertex meshes
        MutableVertexMesh<2,2>* p_mesh = generator.GetMesh(); // Make a vertex mesh

        // Prepare storage for a collection of cells
        std::vector<CellPtr> cells;

        // Specify the proliferative behaviour of the cell by choosing a CellCycleModel:
        //   - With the UniformG1GenerationalCellCycleModel, each cell is given a division time
        //     drawn from a uniform distribution when it is created.
        //     This is a simple stochastic cell-cycle model.
        //     The cells will divide randomly and never stop proliferating.
        //   - Vertex based simulations need "target area modifiers". Phase based cell cycle models 
        //     such as the chosen one allow us to use such "target area modifiers".
        //   - MAKE_PTR is a macro wrapping boost::shared_ptr. See global/src/SmartPointers.hpp
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;

        // Generate the collection of cells
        // For a vertex-based simulation, num_cells == num_mesh_elements
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_transit_type);

        // Create a 2-dimensinal CellPopulation object to connect the mesh and the cells.
        VertexBasedCellPopulation<2> cell_population(*p_mesh, cells);

        // Create an OffLatticeSimulation with the CellPopulation
        OffLatticeSimulation<2> simulator(cell_population);

        // Set some simulation options
        simulator.SetOutputDirectory("VertexBasedMonolayer");
        simulator.SetSamplingTimestepMultiple(200);
        simulator.SetEndTime(20.0);

        // Create a Force object to specify how cells move around
        MAKE_PTR(NagaiHondaForce<2>, p_force);

        // Pass the Force object to the OffLatticeSimulation
        simulator.AddForce(p_force);

        // A NagaiHondaForce needs an AbstractTargetAreaModifier.
        // "Target area modifiers" can modify the target area of individual cells
        // and thus alter the relative forces between neighbouring cells.
        MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
        simulator.AddSimulationModifier(p_growth_modifier);

        // Run the simulation
        simulator.Solve();

        // Checking that we have the correct number of cells at the end.
        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 16u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
};

#endif // TESTVERTEXBASIC_HPP_