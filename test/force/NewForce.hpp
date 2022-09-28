#ifndef NEWFORCE_HPP_
#define NEWFORCE_HPP_

#include "AbstractForce.hpp"

class NewForce : public AbstractForce<2>
{
private:

    double mStrength;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:
    NewForce(double strength=1.0);

    void AddForceContribution(AbstractCellPopulation<2>& rCellPopulation);

    double GetStrength();

    void OutputForceParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(NewForce)

#endif // NEWFORCE_HPP_