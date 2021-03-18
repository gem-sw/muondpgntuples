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

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTDigiFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTSegmentFiller.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCRecHitFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCDigiFiller.h"




#include <iostream>

MuNtupleProducer::MuNtupleProducer( const edm::ParameterSet & config )
{

  usesResource("TFileService");
  edm::Service<TFileService> fileService;
  m_tree = std::shared_ptr<TTree>(fileService->make<TTree>("MuDPGTree","Mu DPG Tree"));

  m_config = std::make_shared<MuNtupleConfig>(MuNtupleConfig(config));

  m_fillers.push_back(std::make_unique<MuNtupleDTDigiFiller>(consumesCollector(), m_config, m_tree, "dtDigi",    MuNtupleDTDigiFiller::Tag::PH1));
  m_fillers.push_back(std::make_unique<MuNtupleDTDigiFiller>(consumesCollector(), m_config, m_tree, "ph2DtDigi", MuNtupleDTDigiFiller::Tag::PH2));

  m_fillers.push_back(std::make_unique<MuNtupleDTSegmentFiller>(consumesCollector(), m_config, m_tree, "dtSeg",    MuNtupleDTSegmentFiller::Tag::PH1));
  m_fillers.push_back(std::make_unique<MuNtupleDTSegmentFiller>(consumesCollector(), m_config, m_tree, "ph2DtSeg", MuNtupleDTSegmentFiller::Tag::PH2));
  //rpc
  m_fillers.push_back(std::make_unique<MuNtupleRPCRecHitFiller>(consumesCollector(), m_config, m_tree, "rpcRecHit"));

  if (m_config->m_storeRpcDigis){
     m_fillers.push_back(std::make_unique<MuNtupleRPCDigiFiller>(consumesCollector(), m_config, m_tree, "rpcDigi"));
    }

}


void MuNtupleProducer::beginJob() 
{

  for (const auto & filler : m_fillers) 
    {
      filler->initialize();
      filler->clear();
    }

}

void MuNtupleProducer::beginRun(const edm::Run & run, const edm::EventSetup & environment )
{
  environment.get<MuonGeometryRecord>().get(m_rpcGeo);

  m_config->getES(run, environment);

}

void MuNtupleProducer::endJob() 
{

  m_tree->GetCurrentFile()->Write();

}

void MuNtupleProducer::analyze(const edm::Event & ev, const edm::EventSetup & environment )
{

 for (const auto & filler : m_fillers) 
    {
      filler->fill(ev);
    }

 m_tree->Fill();

}

// define this as a plug-in
DEFINE_FWK_MODULE(MuNtupleProducer);
