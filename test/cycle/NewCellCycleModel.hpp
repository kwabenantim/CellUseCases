#ifndef NEWCELLCYCLEMODEL_HPP_
#define NEWCELLCYCLEMODEL_HPP_

#include "CheckpointArchiveTypes.hpp"

#include "AbstractSimpleGenerationalCellCycleModel.hpp"

class NewCellCycleModel : public AbstractSimpleGenerationalCellCycleModel
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

    void SetG1Duration();

public:

    NewCellCycleModel() {}

    AbstractCellCycleModel* CreateCellCycleModel();
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(NewCellCycleModel)

#endif // NEWCELLCYCLEMODEL_HPP_
