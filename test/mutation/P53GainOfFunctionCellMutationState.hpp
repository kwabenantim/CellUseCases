#ifndef P53GAINOFFUNCTIONCELLMUTATIONSTATE_HPP_
#define P53GAINOFFUNCTIONCELLMUTATIONSTATE_HPP_

#include "AbstractCellMutationState.hpp"

class P53GainOfFunctionCellMutationState : public AbstractCellMutationState
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    P53GainOfFunctionCellMutationState();
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(P53GainOfFunctionCellMutationState)

#endif // P53GAINOFFUNCTIONCELLMUTATIONSTATE_HPP_
