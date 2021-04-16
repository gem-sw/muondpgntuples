#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMRecHitFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"


MuNtupleGEMRecHitFiller::MuNtupleGEMRecHitFiller(edm::ConsumesCollector && collector,
					     const std::shared_ptr<MuNtupleConfig> config,
					     std::shared_ptr<TTree> tree, const std::string & label) :

  MuNtupleBaseFiller(config, tree, label)
{

  edm::InputTag &iTag = m_config->m_inputTags["gemRecHitTag"];
  if (iTag.label() != "none")
    m_gemRecHitsToken = collector.consumes<GEMRecHitCollection>(iTag);

}

MuNtupleGEMRecHitFiller::~MuNtupleGEMRecHitFiller()
{

};

void MuNtupleGEMRecHitFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nRecHits").c_str(), &m_nRecHits, (m_label + "_nRecHits/i").c_str());

  m_tree->Branch((m_label + "_cluster_size").c_str(), &m_rechit_cluster_size);
  m_tree->Branch((m_label + "_bx").c_str(), &m_rechit_bx);

  m_tree->Branch((m_label + "_region").c_str(), &m_rechit_region);
  m_tree->Branch((m_label + "_chamber").c_str(), &m_rechit_chamber);
  m_tree->Branch((m_label + "_layer").c_str(), &m_rechit_layer);
  m_tree->Branch((m_label + "_etaPartition").c_str(), &m_rechit_etaPartition);

  m_tree->Branch((m_label + "_loc_r").c_str(), &m_rechit_loc_r);
  m_tree->Branch((m_label + "_loc_phi").c_str(), &m_rechit_loc_phi);
  m_tree->Branch((m_label + "_loc_x").c_str(), &m_rechit_loc_x);
  m_tree->Branch((m_label + "_loc_y").c_str(), &m_rechit_loc_y);
  m_tree->Branch((m_label + "_loc_z").c_str(), &m_rechit_loc_z);

  m_tree->Branch((m_label + "_g_r").c_str(), &m_rechit_g_r);
  m_tree->Branch((m_label + "_g_phi").c_str(), &m_rechit_g_phi);
  m_tree->Branch((m_label + "_g_x").c_str(), &m_rechit_g_x);
  m_tree->Branch((m_label + "_g_y").c_str(), &m_rechit_g_y);
  m_tree->Branch((m_label + "_g_z").c_str(), &m_rechit_g_z);
 
}

void MuNtupleGEMRecHitFiller::clear()
{

  m_nRecHits = 0;

  m_rechit_cluster_size.clear();
  m_rechit_bx.clear();

  m_rechit_region.clear();
  m_rechit_chamber.clear();
  m_rechit_layer.clear();
  m_rechit_etaPartition.clear();

  m_rechit_loc_r.clear();
  m_rechit_loc_phi.clear();
  m_rechit_loc_x.clear();
  m_rechit_loc_y.clear();
  m_rechit_loc_z.clear();
 
  m_rechit_g_r.clear();
  m_rechit_g_phi.clear();
  m_rechit_g_x.clear();
  m_rechit_g_y.clear();
  m_rechit_g_z.clear();
  
  
}

void MuNtupleGEMRecHitFiller::fill(const edm::Event & ev)
{
  
  clear();

  auto gemRecHits = conditionalGet<GEMRecHitCollection>(ev, m_gemRecHitsToken,"GEMRecHitCollection");

  if (gemRecHits.isValid())
    {

      auto recHitIt  = gemRecHits->begin();
      auto recHitEnd = gemRecHits->end();

      for (; recHitIt != recHitEnd; ++recHitIt)
	{
	  GEMDetId gemDetId(recHitIt->gemId());

	  m_rechit_etaPartition.push_back(gemDetId.roll());
	  m_rechit_region.push_back(gemDetId.region());
	  m_rechit_chamber.push_back(gemDetId.chamber());
	  m_rechit_layer.push_back(gemDetId.layer());

	  const BoundPlane& surface = m_config->m_gemGeometry->idToDet(gemDetId)->surface();
	  const auto recHitPosLoc = recHitIt->localPosition();
	  const auto recHitPosGlob = surface.toGlobal(recHitPosLoc);

	  m_rechit_loc_r.push_back(recHitPosLoc.perp());
	  m_rechit_loc_phi.push_back(recHitPosLoc.phi());
	  m_rechit_loc_x.push_back(recHitPosLoc.x());
	  m_rechit_loc_y.push_back(recHitPosLoc.y());
	  m_rechit_loc_z.push_back(recHitPosLoc.z());

	  m_rechit_g_r.push_back(recHitPosGlob.perp());
	  m_rechit_g_phi.push_back(recHitPosGlob.phi());
	  m_rechit_g_x.push_back(recHitPosGlob.x());
	  m_rechit_g_y.push_back(recHitPosGlob.y());
	  m_rechit_g_z.push_back(recHitPosGlob.z());

	  m_rechit_cluster_size.push_back(recHitIt->clusterSize());
	  m_rechit_bx.push_back(recHitIt->BunchX());

	  m_nRecHits++;
	}

    }

  return;

}
