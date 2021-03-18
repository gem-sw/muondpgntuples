/** \class MuNtupleConfig MuNtupleConfig.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleConfig.cc
 *  
 * Helper class to handle :
 * - configuration parameters for edm::ParameterSet
 * - DB information from edm::EventSetup
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleConfig.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"

#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"


#include "TString.h"
#include "TRegexp.h"

MuNtupleConfig::MuNtupleConfig(const edm::ParameterSet & config) 
{ 

  edm::InputTag none = edm::InputTag("none");

  m_inputTags["ph1DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtDigiTag", none);
  m_inputTags["ph2DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtDigiTag", none);

  m_inputTags["ph1DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtSegmentTag", none);
  m_inputTags["ph2DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtSegmentTag", none);

  if (m_inputTags["ph1DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH1] = DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("ph1DTtTrigMode"),
								 config.getUntrackedParameter<edm::ParameterSet>("ph1DTtTrigModeConfig"));

  if (m_inputTags["ph2DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH2] = DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("ph2DTtTrigMode"),
								 config.getUntrackedParameter<edm::ParameterSet>("ph2DTtTrigModeConfig"));
//RPC

  m_inputTags["rpcRecHitLabel"] = config.getUntrackedParameter<edm::InputTag>("rpcRecHitLabel", none);
  m_storeRpcRecHits = config.getUntrackedParameter<bool>("storeRpcRecHits");
  m_inputTags["rpcDigiLabel"] = config.getUntrackedParameter<edm::InputTag>("rpcDigiLabel", none);
  m_storeRpcDigis = config.getUntrackedParameter<bool>("storeRpcDigiHits");


}

void MuNtupleConfig::getES(const edm::EventSetup & environment) 
{ 

  if (m_inputTags["ph1DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH1]->setES(environment);

  if (m_inputTags["ph2DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH2]->setES(environment);

  environment.get<MuonGeometryRecord>().get(m_dtGeometry);
  environment.get<GlobalTrackingGeometryRecord>().get(m_trackingGeometry);
  //RPC
  environment.get<MuonGeometryRecord>().get(m_rpcGeometry);

}

void MuNtupleConfig::getES(const edm::Run &run, const edm::EventSetup & environment) 
{
 
  getES(environment);

}
