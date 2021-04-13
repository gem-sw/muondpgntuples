/** \class MuNtupleGenFiller MuNtupleGenFiller.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGenFiller.cc
 *  
 * Helper class : the gen particles filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGenFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

MuNtupleGenFiller::MuNtupleGenFiller(edm::ConsumesCollector && collector,
				     const std::shared_ptr<MuNtupleConfig> config, 
				     std::shared_ptr<TTree> tree, const std::string & label) : 
  MuNtupleBaseFiller(config, tree, label)
{

  edm::InputTag & iTag = m_config->m_inputTags["genPartTag"];
  if (iTag.label() != "none") m_genPartToken = collector.consumes<reco::GenParticleCollection>(iTag);

}

MuNtupleGenFiller::~MuNtupleGenFiller() 
{ 

};

void MuNtupleGenFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nGenParts").c_str(), &m_nGenParts, (m_label + "_nGenParts/i").c_str());

  m_tree->Branch((m_label + "_pdgId").c_str(), &m_gen_pdgId);

  m_tree->Branch((m_label + "_pt").c_str(),     &m_gen_pt);
  m_tree->Branch((m_label + "_phi").c_str(),    &m_gen_phi);
  m_tree->Branch((m_label + "_eta").c_str(),    &m_gen_eta);
  m_tree->Branch((m_label + "_charge").c_str(), &m_gen_charge);
  
}

void MuNtupleGenFiller::clear()
{

  m_nGenParts = 0;

  m_gen_pdgId.clear();

  m_gen_pt.clear();
  m_gen_phi.clear();
  m_gen_eta.clear();
  m_gen_charge.clear();

}

void MuNtupleGenFiller::fill(const edm::Event & ev)
{

  clear();

  auto genParts = conditionalGet<reco::GenParticleCollection>(ev, m_genPartToken,"GenParticleCollection");

  if (genParts.isValid()) 
    {

      for (const auto & genPart : (*genParts))
	{

	  if(genPart.status() != 1)
	    continue;
      
	  m_gen_pdgId.push_back(genPart.pdgId());

	  m_gen_pt.push_back(genPart.pt());
	  m_gen_eta.push_back(genPart.eta());
	  m_gen_phi.push_back(genPart.phi());
	  m_gen_charge.push_back(genPart.charge());
	  
	  ++m_nGenParts;
	}
    }

  return;

}
