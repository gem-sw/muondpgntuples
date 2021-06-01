/** \class MuNtupleProducer MuNtupleProducer.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleProducer.cc
 *  
 * Steering class: the edm::EDAnalyzer for MuDPGNtuple prdouction
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/plugins/MuNtupleProducer.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "FWCore/Framework/interface/Event.h" 
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
// #include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGenFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleEventFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleEnvironmentFiller.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTDigiFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCDigiFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMDigiFiller.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCRecHitFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMRecHitFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTSegmentFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMSegmentFiller.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleMuonFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMMuonFiller.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTTPGPhiFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTTPGThetaFiller.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBmtfFiller.h"

#include <iostream>
#include <algorithm>

MuNtupleProducer::MuNtupleProducer( const edm::ParameterSet & config )
{

  usesResource("TFileService");
  edm::Service<TFileService> fileService;
  m_tree = std::shared_ptr<TTree>(fileService->make<TTree>("MuDPGTree","Mu DPG Tree"));

  m_config = std::make_shared<MuNtupleConfig>(MuNtupleConfig(config,consumesCollector()));

  m_fillers.push_back(std::make_unique<MuNtupleGenFiller>(consumesCollector(), m_config, m_tree, "gen"));

  m_fillers.push_back(std::make_unique<MuNtupleEventFiller>(consumesCollector(), m_config, m_tree, "event"));
  m_fillers.push_back(std::make_unique<MuNtupleEnvironmentFiller>(consumesCollector(), m_config, m_tree, "environment"));

  m_fillers.push_back(std::make_unique<MuNtupleDTDigiFiller>(consumesCollector(), m_config, m_tree, "dtDigi", MuNtupleDTDigiFiller::Tag::PH1));
  m_fillers.push_back(std::make_unique<MuNtupleRPCDigiFiller>(consumesCollector(), m_config, m_tree, "rpcDigi"));
  m_fillers.push_back(std::make_unique<MuNtupleGEMDigiFiller>(consumesCollector(), m_config, m_tree, "gemDigi"));

  m_fillers.push_back(std::make_unique<MuNtupleRPCRecHitFiller>(consumesCollector(), m_config, m_tree, "rpcRecHit"));
  m_fillers.push_back(std::make_unique<MuNtupleGEMRecHitFiller>(consumesCollector(), m_config, m_tree, "gemRecHit"));

  m_fillers.push_back(std::make_unique<MuNtupleDTSegmentFiller>(consumesCollector(), m_config, m_tree, "dtSeg", MuNtupleDTSegmentFiller::Tag::PH1));
  m_fillers.push_back(std::make_unique<MuNtupleGEMSegmentFiller>(consumesCollector(), m_config, m_tree, "gemSegment"));

  m_fillers.push_back(std::make_unique<MuNtupleMuonFiller>(consumesCollector(), m_config, m_tree, "mu"));
  m_fillers.push_back(std::make_unique<MuNtupleGEMMuonFiller>(consumesCollector(), m_config, m_tree, "gemMu"));

  m_fillers.push_back(std::make_unique<MuNtupleDTTPGPhiFiller>(consumesCollector(), m_config, m_tree, "ltTwinMuxIn",MuNtupleDTTPGPhiFiller::TriggerTag::TM_IN));
  m_fillers.push_back(std::make_unique<MuNtupleDTTPGPhiFiller>(consumesCollector(), m_config, m_tree, "ltTwinMuxOut", MuNtupleDTTPGPhiFiller::TriggerTag::TM_OUT));
  m_fillers.push_back(std::make_unique<MuNtupleDTTPGPhiFiller>(consumesCollector(), m_config, m_tree, "ltBmtfIn", MuNtupleDTTPGPhiFiller::TriggerTag::BMTF_IN));

  m_fillers.push_back(std::make_unique<MuNtupleDTTPGThetaFiller>(consumesCollector(), m_config, m_tree, "ltTwinMuxInTh", MuNtupleDTTPGThetaFiller::TriggerTag::TM_IN));
  m_fillers.push_back(std::make_unique<MuNtupleDTTPGThetaFiller>(consumesCollector(), m_config, m_tree, "ltBmtfInTh", MuNtupleDTTPGThetaFiller::TriggerTag::BMTF_IN));

  m_fillers.push_back(std::make_unique<MuNtupleBmtfFiller>(consumesCollector(), m_config, m_tree, "tfBmtfOut"));

  //m_fillers.push_back(std::make_unique<MuNtupleDTDigiFiller>(consumesCollector(), m_config, m_tree, "ph2DtDigi", MuNtupleDTDigiFiller::Tag::PH2));
  //m_fillers.push_back(std::make_unique<MuNtupleDTSegmentFiller>(consumesCollector(), m_config, m_tree, "ph2DtSeg", MuNtupleDTSegmentFiller::Tag::PH2));

  // m_fillers.push_back(std::make_unique<MuNtuplePh2TPGPhiFiller>(consumesCollector(), m_config, m_tree, "ph2TpgPhiHw",    MuNtuplePh2TPGPhiFiller::TriggerTag::HW));
  // m_fillers.push_back(std::make_unique<MuNtuplePh2TPGPhiFiller>(consumesCollector(), m_config, m_tree, "ph2TpgPhiEmuHb", MuNtuplePh2TPGPhiFiller::TriggerTag::HB));
  // m_fillers.push_back(std::make_unique<MuNtuplePh2TPGPhiFiller>(consumesCollector(), m_config, m_tree, "ph2TpgPhiEmuAm", MuNtuplePh2TPGPhiFiller::TriggerTag::AM));
  
}

void MuNtupleProducer::beginJob() 
{

  auto initialize = [](auto const& filler)
    {
      filler->initialize(); 
      filler->clear(); 
    };

  std::for_each(m_fillers.begin(), m_fillers.end(), initialize);

}

void MuNtupleProducer::beginRun(const edm::Run & run, const edm::EventSetup & environment )
{

  m_config->getES(run, environment);

}

void MuNtupleProducer::endJob() 
{

  m_tree->GetCurrentFile()->Write();

}

void MuNtupleProducer::analyze(const edm::Event & ev, const edm::EventSetup & environment )
{

  m_config->getES(environment);

  auto fill = [& ev](auto const& filler)
    { 
      filler->fill(ev);
    };

  std::for_each(m_fillers.begin(), m_fillers.end(), fill);
 
}

// define this as a plug-in
DEFINE_FWK_MODULE(MuNtupleProducer);
