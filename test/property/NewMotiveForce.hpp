#ifndef NEWMOTIVEFORCE_HPP_
#define NEWMOTIVEFORCE_HPP_

#include "AbstractCellPopulation.hpp"
#include "AbstractForce.hpp"

class NewMotiveForce : public AbstractForce<2>
{
private:

    double mStrength;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    NewMotiveForce(double strength=2.0);

    void AddForceContribution(AbstractCellPopulation<2>& rCellPopulation);

    void OutputForceParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(NewMotiveForce)

#endif // NEWMOTIVEFORCE_HPP_
