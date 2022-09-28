#ifndef CELLMOTILITYWRITER_HPP_
#define CELLMOTILITYWRITER_HPP_

#include "AbstractCellPopulation.hpp"
#include "AbstractCellWriter.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class CellMotilityWriter : public AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>
{
private:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    CellMotilityWriter();

    double GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);

    void VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);

};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellMotilityWriter)

#endif // CELLMOTILITYWRITER_HPP_
