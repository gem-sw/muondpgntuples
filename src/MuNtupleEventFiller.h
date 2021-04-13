#ifndef MuNtuple_MuNtupleEventFiller_h
#define MuNtuple_MuNtupleEventFiller_h

/** \class MuNtupleEventFiller MuNtupleEventFiller.h MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleEventFiller.h
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>
#include <cstdint>

class MuNtupleEventFiller : public MuNtupleBaseFiller
{

 public:

  /// Constructor
  MuNtupleEventFiller(edm::ConsumesCollector && collector,
		      const std::shared_ptr<MuNtupleConfig> config, 
		      std::shared_ptr<TTree> tree, const std::string & label);

  ///Destructor
  virtual ~MuNtupleEventFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// The DT FEB BX token
  edm::EDGetTokenT<int> m_dtFedBxToken;

  int m_runNumber;
  int  m_lumiBlock;
  int64_t m_eventNumber;

  uint64_t m_timeStamp;
  
  int  m_bunchCrossing;
  int64_t m_orbitNumber;
  
};
  
#endif

