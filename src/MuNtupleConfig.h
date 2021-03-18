#ifndef MuNtuple_MuNtupleConfig_h
#define MuNtuple_MuNtupleConfig_h

/** \class MuNtupleConfig MuNtupleConfig.h MuDPGAnalysis/MuNtuples/src/MuNtupleConfig.h
 *  
 * Helper class to handle :
 * - configuration parameters for edm::ParameterSet
 * - DB information from edm::EventSetup
 * - HLT configuration from dm::EventSetup and dm::Run
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigBaseSync.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include <map>
#include <string>
#include <memory>

namespace edm
{
  class ParameterSet;
  class EventSetup;
  class Run;
}

class MuNtupleConfig
{

 public :

  enum class PhaseTag { PH1 = 0, PH2 };

  /// Constructor
  MuNtupleConfig(const edm::ParameterSet & config);

  /// Update EventSetup information
  void getES(const edm::EventSetup & environment);

  /// Update EventSetup information
  void getES(const edm::Run &run, 
	     const edm::EventSetup & environment);

  /// Map containing different input tags
  std::map<std::string, edm::InputTag> m_inputTags;

  /// The class to handle DT trigger time pedestals
  std::map<PhaseTag, std::unique_ptr<DTTTrigBaseSync>> m_dtSyncs;

  /// Handle to the tracking geometry
  edm::ESHandle<GlobalTrackingGeometry> m_trackingGeometry;

  /// Handle to the DT geometry
  edm::ESHandle<DTGeometry> m_dtGeometry;


  bool m_storeRpcDigis, m_storeRpcRecHits;
  edm::ESHandle<RPCGeometry> m_rpcGeometry;


};

#endif
