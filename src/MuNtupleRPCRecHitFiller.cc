/** \class MuNtupleDigiFiller MuNtupleDigiFiller.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCRecHitFiller.cc
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 DT digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCRecHitFiller.h"

#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

MuNtupleRPCRecHitFiller::MuNtupleRPCRecHitFiller(edm::ConsumesCollector && collector,
				       const std::shared_ptr<MuNtupleConfig> config, 
				       std::shared_ptr<TTree> tree, const std::string & label
				       ) : 
  MuNtupleBaseFiller(config, tree, label)
{



  m_rpcRecHitToken = collector.consumes<RPCRecHitCollection>(m_config->m_inputTags["rpcRecHitLabel"]);

}

MuNtupleRPCRecHitFiller::~MuNtupleRPCRecHitFiller() 
{ 

};

void MuNtupleRPCRecHitFiller::initialize()
{

  m_tree->Branch((m_label + "_nRecHit").c_str(), &m_nRecHit, (m_label + "_nRecHit/I").c_str());
  m_tree->Branch((m_label + "_firstClusterStrip").c_str(), &m_firstClusterStrip);
  m_tree->Branch((m_label + "_clusterSize").c_str(), &m_clusterSize);
  m_tree->Branch((m_label + "_bx").c_str(), &m_bx);
  m_tree->Branch((m_label + "_time").c_str(), &m_time);
  m_tree->Branch((m_label + "_X").c_str(), &m_coordinateX);
  m_tree->Branch((m_label + "_Y").c_str(), &m_coordinateY);
  m_tree->Branch((m_label + "_Z").c_str(), &m_coordinateZ);
  
  m_tree->Branch((m_label + "_region").c_str(), &m_region);
  m_tree->Branch((m_label + "_ring").c_str(), &m_ring);
  m_tree->Branch((m_label + "_station").c_str(), &m_station);
  m_tree->Branch((m_label + "_layer").c_str(), &m_layer);
  m_tree->Branch((m_label + "_sector").c_str(), &m_sector);
  m_tree->Branch((m_label + "_subsector").c_str(), &m_subsector);
  m_tree->Branch((m_label + "_roll").c_str(), &m_roll);

  m_tree->Branch((m_label + "_rawId").c_str(), &m_rawId);

 
}

void MuNtupleRPCRecHitFiller::clear()
{

  m_nRecHit = 0;

  m_firstClusterStrip.clear();
  m_clusterSize.clear();
  m_bx.clear();
  m_time.clear();
  m_coordinateX.clear();
  m_coordinateY.clear();
  m_coordinateZ.clear();

  m_region.clear();
  m_ring.clear();
  m_station.clear();
  m_layer.clear();
  m_sector.clear();
  m_subsector.clear();
  m_roll.clear();

  m_rawId.clear();
}

void MuNtupleRPCRecHitFiller::fill(const edm::Event & ev)
{



   clear();

   auto rpcRecHits = conditionalGet<RPCRecHitCollection>(ev, m_rpcRecHitToken, "RPCRecHitCollection");
    if(rpcRecHits.isValid()){
        for (RPCRecHitCollection::const_iterator recHitIt = rpcRecHits->begin(); recHitIt != rpcRecHits->end(); recHitIt++) {

            m_firstClusterStrip.push_back(recHitIt->firstClusterStrip());
            m_clusterSize.push_back(recHitIt->clusterSize());
            m_bx.push_back(recHitIt->BunchX());
            m_time.push_back(recHitIt->time());
            m_coordinateX.push_back(recHitIt->localPosition().x());
            m_coordinateY.push_back(recHitIt->localPosition().y());
            m_coordinateZ.push_back(recHitIt->localPosition().z());

            RPCDetId rpcDetId = (RPCDetId)(*recHitIt).rpcId();
            m_region.push_back(rpcDetId.region());
            m_ring.push_back(rpcDetId.ring());
            m_station.push_back(rpcDetId.station());
            m_layer.push_back(rpcDetId.layer());
            m_sector.push_back(rpcDetId.sector());
            m_subsector.push_back(rpcDetId.subsector());
            m_roll.push_back(rpcDetId.roll());
            m_rawId.push_back(rpcDetId.rawId());

            m_nRecHit++;
        }
    }

  return;

}

