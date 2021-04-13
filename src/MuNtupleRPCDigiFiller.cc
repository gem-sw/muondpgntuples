/** \class MuNtupleDigiFiller MuNtupleDigiFiller.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCDigiFiller.cc
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 DT digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCDigiFiller.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

MuNtupleRPCDigiFiller::MuNtupleRPCDigiFiller(edm::ConsumesCollector && collector,
				       const std::shared_ptr<MuNtupleConfig> config, 
				       std::shared_ptr<TTree> tree, const std::string & label
				       ) : 
  MuNtupleBaseFiller(config, tree, label)
{



  m_rpcDigiToken = collector.consumes<RPCDigiCollection>(m_config->m_inputTags["rpcDigiLabel"]);

}

MuNtupleRPCDigiFiller::~MuNtupleRPCDigiFiller() 
{ 

};

void MuNtupleRPCDigiFiller::initialize()
{

  m_tree->Branch((m_label + "_strip").c_str(), &m_strip, (m_label + "_strip/I").c_str());
  m_tree->Branch((m_label + "_strip").c_str(), &m_strip);
  m_tree->Branch((m_label + "_bx").c_str(), &m_bx);
  m_tree->Branch((m_label + "_time").c_str(), &m_time);
  m_tree->Branch((m_label + "_X").c_str(), &m_coordinateX);
  m_tree->Branch((m_label + "_Y").c_str(), &m_coordinateY);
             
  m_tree->Branch((m_label + "_region").c_str(), &m_region);
  m_tree->Branch((m_label + "_ring").c_str(), &m_ring);
  m_tree->Branch((m_label + "_station").c_str(), &m_station);
  m_tree->Branch((m_label + "_layer").c_str(), &m_layer);
  m_tree->Branch((m_label + "_sector").c_str(), &m_sector);
  m_tree->Branch((m_label + "_subsector").c_str(), &m_subsector);
  m_tree->Branch((m_label + "_roll").c_str(), &m_roll);
  
  m_tree->Branch((m_label + "_rawId").c_str(), &m_rawId);

 
}

void MuNtupleRPCDigiFiller::clear()
{

  m_nDigi   = 0;

  m_strip.clear();
  m_bx.clear();
  m_time.clear();
  m_coordinateX.clear();
  m_coordinateY.clear();

  m_region.clear();
  m_ring.clear();
  m_station.clear();
  m_layer.clear();
  m_sector.clear();
  m_subsector.clear();
  m_roll.clear();

  m_rawId.clear();

}

void MuNtupleRPCDigiFiller::fill(const edm::Event & ev)
{



   clear();

   auto rpcDigis = conditionalGet<RPCDigiCollection>(ev, m_rpcDigiToken, "RPCDigiCollection");
   if(rpcDigis.isValid()){ 
    RPCDigiCollection::DigiRangeIterator detUnitIt;
    for (detUnitIt = rpcDigis->begin(); detUnitIt != rpcDigis->end(); ++detUnitIt){

        const RPCDetId& rpcDetId = (*detUnitIt).first;
        const RPCDigiCollection::Range& range = (*detUnitIt).second;

        for (RPCDigiCollection::const_iterator digiIt = range.first; digiIt != range.second; ++digiIt){
            m_strip.push_back(digiIt->strip());  
            m_bx.push_back(digiIt->bx());  
            m_time.push_back(digiIt->time());  
            m_coordinateX.push_back(digiIt->coordinateX());  
            m_coordinateY.push_back(digiIt->coordinateY());  

            m_region.push_back(rpcDetId.region());
            m_ring.push_back(rpcDetId.ring());
            m_station.push_back(rpcDetId.station());
            m_layer.push_back(rpcDetId.layer());
            m_sector.push_back(rpcDetId.sector());
            m_subsector.push_back(rpcDetId.subsector());
            m_roll.push_back(rpcDetId.roll());

            m_rawId.push_back(rpcDetId.rawId());

            m_nDigi++;
         }

       }
    }

  return;

}

