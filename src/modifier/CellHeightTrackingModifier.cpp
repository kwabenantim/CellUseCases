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
#include "AbstractCellPopulation.hpp"
#include "AbstractCellBasedSimulationModifier.hpp"

#include "CellHeightTrackingModifier.hpp"

template<class Archive>
void CellHeightTrackingModifier::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractCellBasedSimulationModifier<2,2> >(*this);
}

CellHeightTrackingModifier::CellHeightTrackingModifier()
    : AbstractCellBasedSimulationModifier<2,2>()
{}

void CellHeightTrackingModifier::UpdateAtEndOfTimeStep(AbstractCellPopulation<2,2>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

void CellHeightTrackingModifier::SetupSolve(AbstractCellPopulation<2,2>& rCellPopulation, std::string outputDirectory)
{

    UpdateCellData(rCellPopulation);
}

void CellHeightTrackingModifier::UpdateCellData(AbstractCellPopulation<2,2>& rCellPopulation)
{
    rCellPopulation.Update();

    for (AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
            cell_iter != rCellPopulation.End();
            ++cell_iter)
    {
        double cell_height = rCellPopulation.GetLocationOfCellCentre(*cell_iter)[1];

        cell_iter->GetCellData()->SetItem("height", cell_height);
    }
}

void CellHeightTrackingModifier::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    AbstractCellBasedSimulationModifier<2>::OutputSimulationModifierParameters(rParamsFile);
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(CellHeightTrackingModifier)
