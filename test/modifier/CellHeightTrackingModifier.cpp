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
