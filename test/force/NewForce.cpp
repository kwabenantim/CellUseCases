#include "AbstractForce.hpp"
#include "NewForce.hpp"

template<class Archive>
void NewForce::serialize(Archive & archive, const unsigned int version)
{
    archive & boost::serialization::base_object<AbstractForce<2> >(*this);
    archive & mStrength;
}

NewForce::NewForce(double strength)
    : AbstractForce<2>(),
        mStrength(strength)
{
    assert(mStrength > 0.0);
}

void NewForce::AddForceContribution(AbstractCellPopulation<2>& rCellPopulation)
{
    c_vector<double, 2> force = zero_vector<double>(2);
    force(1) = -mStrength;

    for (unsigned node_index=0; node_index<rCellPopulation.GetNumNodes(); node_index++)
    {
        rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
    }
}

double NewForce::GetStrength()
{
    return mStrength;
}

void NewForce::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<Strength>" << mStrength << "</Strength>\n";
    AbstractForce<2>::OutputForceParameters(rParamsFile);
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(NewForce)
