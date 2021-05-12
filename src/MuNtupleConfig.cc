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

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"


#include "TString.h"
#include "TRegexp.h"

MuNtupleConfig::MuNtupleConfig(const edm::ParameterSet & config,
			       edm::ConsumesCollector && collector) 
{ 
  
  edm::InputTag none = edm::InputTag("none");

  m_inputTags["genPartTag"] = config.getUntrackedParameter<edm::InputTag>("genPartTag", none);

  m_inputTags["dtFedBxTag"] = config.getUntrackedParameter<edm::InputTag>("dtFedBxTag", none);

  m_inputTags["puInfoTag"] = config.getUntrackedParameter<edm::InputTag>("puInfoTag", none);
  m_inputTags["lumiScalerTag"] = config.getUntrackedParameter<edm::InputTag>("lumiScalerTag", none);
  m_inputTags["primaryVerticesTag"] = config.getUntrackedParameter<edm::InputTag>("primaryVerticesTag", none);  

  m_inputTags["dtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("dtDigiTag", none);
  m_inputTags["rpcDigiTag"] = config.getUntrackedParameter<edm::InputTag>("rpcDigiTag", none);
  m_inputTags["gemDigiTag"] = config.getUntrackedParameter<edm::InputTag>("gemDigiTag", none);
  
  m_inputTags["rpcRecHitTag"] = config.getUntrackedParameter<edm::InputTag>("rpcRecHitTag", none);
  m_inputTags["gemRecHitTag"] = config.getUntrackedParameter<edm::InputTag>("gemRecHitTag",none);

  m_inputTags["dtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("dtSegmentTag", none);
  m_inputTags["gemSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("gemSegmentTag",none);
  m_inputTags["cscSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("cscSegmentTag",none);

  if (m_inputTags["dtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH1] = DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("dttTrigMode"),
								 config.getUntrackedParameter<edm::ParameterSet>("dttTrigModeConfig"));

  m_inputTags["muonTag"] = config.getUntrackedParameter<edm::InputTag>("muonTag",none);
  m_inputTags["trigResultsTag"] = config.getUntrackedParameter<edm::InputTag>("trigResultsTag", none);
  m_inputTags["trigEventTag"] = config.getUntrackedParameter<edm::InputTag>("trigEventTag", none);

  m_inputTags["trigResultsTag"] = config.getUntrackedParameter<edm::InputTag>("trigResultsTag", none);
  m_inputTags["trigEventTag"] = config.getUntrackedParameter<edm::InputTag>("trigEventTag", none);

  m_inputTags["twinMuxInTag"] = config.getUntrackedParameter<edm::InputTag>("twinMuxInTag", none);
  m_inputTags["twinMuxOutTag"] = config.getUntrackedParameter<edm::InputTag>("twinMuxOutTag", none);
  m_inputTags["bmtfInPhiTag"] = config.getUntrackedParameter<edm::InputTag>("bmtfInPhiTag", none);

  m_inputTags["twinMuxInThTag"] = config.getUntrackedParameter<edm::InputTag>("twinMuxInThTag", none);
  m_inputTags["bmtfInThTag"] = config.getUntrackedParameter<edm::InputTag>("bmtfInThTag", none);

  m_inputTags["bmtfOutTag"] = config.getUntrackedParameter<edm::InputTag>("bmtfOutTag", none);
  
  // m_inputTags["dtDigiTagPh2"] = config.getUntrackedParameter<edm::InputTag>("dtDigiTagPh2", none);
  // m_inputTags["dtSegmentTagPh2"] = config.getUntrackedParameter<edm::InputTag>("dtSegmentTagPh2", none);

  // if (m_inputTags["dtSegmentTagPh2"].label() != "none")
  //   m_dtSyncs[PhaseTag::PH2] = DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("dttTrigModePh2"),
  //								    config.getUntrackedParameter<edm::ParameterSet>("dttTrigModeConfigPh2"));

  m_isoTrigName = config.getUntrackedParameter<std::string>("isoTrigName", "HLT_IsoMu24_v*");
  m_trigName = config.getUntrackedParameter<std::string>("trigName", "HLT_Mu50_v*");

  m_muonSP = std::make_unique<MuonServiceProxy>(config.getParameter<edm::ParameterSet>("ServiceParameters"), std::move(collector));

}

void MuNtupleConfig::getES(const edm::EventSetup & environment) 
{ 

  m_muonSP->update(environment);
  environment.get<GlobalTrackingGeometryRecord>().get(m_trackingGeometry);
  environment.get<TransientTrackRecord>().get("TransientTrackBuilder", m_transientTrackBuilder);
     
}

void MuNtupleConfig::getES(const edm::Run &run, const edm::EventSetup & environment) 
{
 
  // CB getES(environment);

  if (m_inputTags["ph1DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH1]->setES(environment);

  // if (m_inputTags["ph2DtSegmentTag"].label() != "none")
  //   m_dtSyncs[PhaseTag::PH2]->setES(environment);

  environment.get<MuonGeometryRecord>().get(m_dtGeometry);
  environment.get<MuonGeometryRecord>().get(m_cscGeometry);
  environment.get<MuonGeometryRecord>().get(m_rpcGeometry);
  environment.get<MuonGeometryRecord>().get(m_gemGeometry);

  edm::ESHandle<DTGeometry> dtIdealGeom; 
  environment.get<MuonGeometryRecord>().get("idealForDigi",dtIdealGeom);

  m_trigGeomUtils.reset();
  m_trigGeomUtils = std::make_unique<DTTrigGeomUtils>(dtIdealGeom);

  bool changed = true;
  m_hltConfig.init(run, environment, "HLT", changed);

  bool enableWildcard = true;

  TString tName = TString(m_trigName);
  TRegexp tNamePattern = TRegexp(tName, enableWildcard);

  for (unsigned iPath = 0; iPath < m_hltConfig.size(); ++iPath) 
    {

      TString pathName = TString(m_hltConfig.triggerName(iPath));
      if (pathName.Contains(tNamePattern)) 
	m_trigIndices.push_back(static_cast<int>(iPath));

    }
  
  tName = TString(m_isoTrigName);
  tNamePattern = TRegexp(tName, enableWildcard);
 
  for (unsigned iPath = 0; iPath < m_hltConfig.size(); ++iPath) 
    {

      TString pathName = TString(m_hltConfig.triggerName(iPath));
      if (pathName.Contains(tNamePattern)) 
	m_isoTrigIndices.push_back(static_cast<int>(iPath));

    }

}


