#include "AbstractCellPopulation.hpp"
#include "AbstractCellKiller.hpp"
#include "NewCellKiller.hpp"

template<class Archive>
void NewCellKiller::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractCellKiller<2> >(*this);
}

NewCellKiller::NewCellKiller(AbstractCellPopulation<2>* pCellPopulation)
    : AbstractCellKiller<2>(pCellPopulation)
{}

void NewCellKiller::CheckAndLabelCellsForApoptosisOrDeath()
{
    for (AbstractCellPopulation<2>::Iterator cell_iter = this->mpCellPopulation->Begin();
        cell_iter != this->mpCellPopulation->End();
        ++cell_iter)
    {
        c_vector<double, 2> location;
        location = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter);

        if (pow(location[0]/20, 2) + pow(location[1]/10, 2) > 1.0)
        {
            this->mpCellPopulation->KillCell(*cell_iter, "NewCellKiller");
        }
    }
}

void NewCellKiller::OutputCellKillerParameters(out_stream& rParamsFile)
{
    AbstractCellKiller<2>::OutputCellKillerParameters(rParamsFile);
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(NewCellKiller)

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        inline void save_construct_data(
            Archive & ar, const NewCellKiller * t, const unsigned int file_version)
        {
            const AbstractCellPopulation<2>* const p_cell_population = t->GetCellPopulation();
            ar << p_cell_population;
        }

        template<class Archive>
        inline void load_construct_data(
            Archive & ar, NewCellKiller * t, const unsigned int file_version)
        {
            AbstractCellPopulation<2>* p_cell_population;
            ar >> p_cell_population;

            // Invoke inplace constructor to initialise instance
            ::new(t)NewCellKiller(p_cell_population);
        }
    }
}
