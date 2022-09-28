#ifndef TESTCELLHEIGHTTRACKINGMODIFIER_HPP_
#define TESTCELLHEIGHTTRACKINGMODIFIER_HPP_

#include "AbstractCellPopulation.hpp"
#include "AbstractCellBasedSimulationModifier.hpp"

class CellHeightTrackingModifier : public AbstractCellBasedSimulationModifier<2,2>
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    CellHeightTrackingModifier();

    ~CellHeightTrackingModifier() {}

    void UpdateAtEndOfTimeStep(AbstractCellPopulation<2,2>& rCellPopulation);

    void SetupSolve(AbstractCellPopulation<2,2>& rCellPopulation, std::string outputDirectory);

    void UpdateCellData(AbstractCellPopulation<2,2>& rCellPopulation);

    void OutputSimulationModifierParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(CellHeightTrackingModifier)

#endif // TESTCELLHEIGHTTRACKINGMODIFIER_HPP_
