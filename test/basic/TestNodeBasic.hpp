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
#ifndef TESTNODEBASIC_HPP_
#define TESTNODEBASIC_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"

#include "CellsGenerator.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "RandomCellKiller.hpp"
#include "RepulsionForce.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"

#include "FakePetscSetup.hpp"

class TestNodeBasic : public AbstractCellBasedTestSuite
{
    public:
    // In a "node-based" simulation, cells are represented by overlapping spheres
    // (these are actually circles in 2D).
    // This is a simple node-based simulation of an epithelial monolayer.
    void TestNodeBasedMonolayer()
    {
        // Create a 2D NodesOnlyMesh to hold the spatial information of the simulation
        HoneycombMeshGenerator generator(2, 2); // Generates generic meshes
        MutableMesh<2,2>* p_generating_mesh = generator.GetMesh(); // Make a generic mesh

        // Make a mesh which is not really a mesh but only has nodes in it
        NodesOnlyMesh<2> mesh;

        // Put nodes in the generic mesh
        // The "interaction cut off length" of 1.5 defines connectivity in the mesh.
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, 1.5); 

        // Prepare storage for a collection of cells
        std::vector<CellPtr> cells;

        // Specify the proliferative behaviour of the cell by choosing a CellCycleModel
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;

        // For a node-based simulation, num_cells == num_nodes
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_transit_type);

        // Create a 2-dimensinal CellPopulation object to connect the mesh and the cells.
        NodeBasedCellPopulation<2> cell_population(mesh, cells);

        // Create an OffLatticeSimulation with the CellPopulation
        OffLatticeSimulation<2> simulator(cell_population);

        // Set some simulation options
        simulator.SetOutputDirectory("NodeBasedMonolayer");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(20.0);

        // Add a Force object to specify how cells move around
        // A RepulsionForce is suitable for node-based simulations
        MAKE_PTR(RepulsionForce<2>, p_force);
        simulator.AddForce(p_force);

        // A CellKiller can remove cells from any simulation type.
        // MAKE_PTR_ARGS wraps boost::shared_ptr in global/src/SmartPointers.hpp
        MAKE_PTR_ARGS(RandomCellKiller<2>, p_cell_killer, (&cell_population, 0.01));
        simulator.AddCellKiller(p_cell_killer);

        // Run the simulation
        simulator.Solve();

        // Checking that we have the correct number of cells at the end.
        TS_ASSERT_EQUALS(cell_population.GetNumRealCells(), 7u);
        TS_ASSERT_DELTA(SimulationTime::Instance()->GetTime(), 20.0, 1e-10);
    }
};

#endif // TESTNODEBASIC_HPP_