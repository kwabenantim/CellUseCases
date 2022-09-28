#include "AbstractCellPopulation.hpp"
#include "AbstractForce.hpp"
#include "MotileCellProperty.hpp"
#include "NewMotiveForce.hpp"

template<class Archive>
void NewMotiveForce::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractForce<2> >(*this);
    archive & mStrength;
}

NewMotiveForce::NewMotiveForce(double strength)
    : AbstractForce<2>(),
        mStrength(strength)
{
    assert(mStrength > 0.0);
}

void NewMotiveForce::AddForceContribution(AbstractCellPopulation<2>& rCellPopulation)
{
    for (AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
            cell_iter != rCellPopulation.End();
            ++cell_iter)
    {
        if (cell_iter->HasCellProperty<MotileCellProperty>())
        {
            unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);

            c_vector<double, 2> location;
            location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            c_vector<double, 2> force = -1.0 * mStrength * location;
            rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
        }
    }
}

void NewMotiveForce::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<Strength>" << mStrength << "</Strength>\n";
    AbstractForce<2>::OutputForceParameters(rParamsFile);
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(NewMotiveForce)
