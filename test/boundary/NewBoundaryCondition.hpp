#ifndef NEWBOUNDARYCONDITION_HPP_
#define NEWBOUNDARYCONDITION_HPP_

#include "AbstractCellPopulationBoundaryCondition.hpp"

class NewBoundaryCondition : public AbstractCellPopulationBoundaryCondition<2>
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    NewBoundaryCondition(AbstractCellPopulation<2>* pCellPopulation);

    void ImposeBoundaryCondition(const std::map<Node<2>*, c_vector<double, 2> >& rOldLocations);

    bool VerifyBoundaryCondition();

    void OutputCellPopulationBoundaryConditionParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(NewBoundaryCondition)

#endif // NEWBOUNDARYCONDITION_HPP_
