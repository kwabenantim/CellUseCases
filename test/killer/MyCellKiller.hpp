#ifndef MYCELLKILLER_HPP_
#define MYCELLKILLER_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "AbstractCellKiller.hpp"

#include "CellRemovalLocationsWriter.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellsGenerator.hpp"
#include "SmartPointers.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

class MyCellKiller : public AbstractCellKiller<2>
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellKiller<2> >(*this);
    }

public:

    MyCellKiller(AbstractCellPopulation<2>* pCellPopulation)
        : AbstractCellKiller<2>(pCellPopulation)
    {}

    void CheckAndLabelCellsForApoptosisOrDeath()
    {
        for (AbstractCellPopulation<2>::Iterator cell_iter = this->mpCellPopulation->Begin();
            cell_iter != this->mpCellPopulation->End();
            ++cell_iter)
        {
            c_vector<double, 2> location;
            location = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter);

            if (pow(location[0]/20, 2) + pow(location[1]/10, 2) > 1.0)
            {
                this->mpCellPopulation->KillCell(*cell_iter, "MyCellKiller");
            }
        }
    }

    void OutputCellKillerParameters(out_stream& rParamsFile)
    {
        AbstractCellKiller<2>::OutputCellKillerParameters(rParamsFile);
    }
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(MyCellKiller)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(MyCellKiller)

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        inline void save_construct_data(
            Archive & ar, const MyCellKiller * t, const unsigned int file_version)
        {
            const AbstractCellPopulation<2>* const p_cell_population = t->GetCellPopulation();
            ar << p_cell_population;
        }

        template<class Archive>
        inline void load_construct_data(
            Archive & ar, MyCellKiller * t, const unsigned int file_version)
        {
            AbstractCellPopulation<2>* p_cell_population;
            ar >> p_cell_population;

            // Invoke inplace constructor to initialise instance
            ::new(t)MyCellKiller(p_cell_population);
        }
    }
}

#endif // MYCELLKILLER_HPP_