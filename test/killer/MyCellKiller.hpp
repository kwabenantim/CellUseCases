#ifndef MYCELLKILLER_HPP_
#define MYCELLKILLER_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "AbstractCellKiller.hpp"

#include "CellRemovalLocationsWriter.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellsGenerator.hpp"
#include "SmartPointers.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

class MyCellKiller : public AbstractCellKiller<2>
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version);

public:

    MyCellKiller(AbstractCellPopulation<2>* pCellPopulation);

    void CheckAndLabelCellsForApoptosisOrDeath();

    void OutputCellKillerParameters(out_stream& rParamsFile);
};

#endif // MYCELLKILLER_HPP_