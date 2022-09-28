#ifndef NEWCELLKILLER_HPP_
#define NEWCELLKILLER_HPP_

#include "AbstractCellPopulation.hpp"
#include "AbstractCellKiller.hpp"

class NewCellKiller : public AbstractCellKiller<2>
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    NewCellKiller(AbstractCellPopulation<2>* pCellPopulation);

    void CheckAndLabelCellsForApoptosisOrDeath();

    void OutputCellKillerParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(NewCellKiller)

#endif // NEWCELLKILLER_HPP_
