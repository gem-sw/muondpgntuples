/** \class MuNtupleEnvironmentFiller MuNtupleEnvironmentFiller.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleEnvironmentFiller.cc
 *  
 * Helper class : the pile-up, luminosity and reco vertices filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleEnvironmentFiller.h"

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

MuNtupleEnvironmentFiller::MuNtupleEnvironmentFiller(edm::ConsumesCollector && collector,
						     const std::shared_ptr<MuNtupleConfig> config, 
						     std::shared_ptr<TTree> tree, const std::string & label) : 
  MuNtupleBaseFiller(config, tree, label)
{

  edm::InputTag iTag = m_config->m_inputTags["puInfoTag"];
  if (iTag.label() != "none") 
    m_puInfoToken = collector.consumes<std::vector<PileupSummaryInfo> >(iTag);

  iTag = m_config->m_inputTags["lumiScalerTag"];
  if (iTag.label() != "none") 
    m_lumiScalerToken = collector.consumes<LumiScalersCollection>(iTag);

  iTag = m_config->m_inputTags["primaryVerticesTag"];
  if (iTag.label() != "none") 
    m_primariVerticesToken = collector.consumes<reco::VertexCollection>(iTag);

}

MuNtupleEnvironmentFiller::~MuNtupleEnvironmentFiller() 
{ 

};

void MuNtupleEnvironmentFiller:: initialize()
{
  
  m_tree->Branch((m_label + "_truePileUp").c_str(), &m_truePileUp, (m_label + "_truePileUp/S").c_str());
  m_tree->Branch((m_label + "_actualPileUp").c_str(), &m_actualPileUp, (m_label + "_actualPileUp/S").c_str());

  m_tree->Branch((m_label + "_instLumi").c_str(), &m_instLumi, (m_label + "_instLumi/I").c_str());

  m_tree->Branch((m_label + "_nPV").c_str(), &m_nPV, (m_label + "_nPV/S").c_str());

  m_tree->Branch((m_label + "_pv_x").c_str(), &m_pv_x, (m_label + "_pv_x/F").c_str());
  m_tree->Branch((m_label + "_pv_y").c_str(), &m_pv_y, (m_label + "_pv_y/F").c_str());
  m_tree->Branch((m_label + "_pv_z").c_str(), &m_pv_z, (m_label + "_pv_z/F").c_str());

  m_tree->Branch((m_label + "_pv_xxErr").c_str(), &m_pv_xxErr, (m_label + "_pv_xxErr/F").c_str());
  m_tree->Branch((m_label + "_pv_yyErr").c_str(), &m_pv_yyErr, (m_label + "_pv_yyErr/F").c_str());
  m_tree->Branch((m_label + "_pv_zzErr").c_str(), &m_pv_zzErr, (m_label + "_pv_zzErr/F").c_str());
  m_tree->Branch((m_label + "_pv_xyErr").c_str(), &m_pv_xyErr, (m_label + "_pv_xyErr/F").c_str());
  m_tree->Branch((m_label + "_pv_xzErr").c_str(), &m_pv_xzErr, (m_label + "_pv_xzErr/F").c_str());
  m_tree->Branch((m_label + "_pv_yzErr").c_str(), &m_pv_yzErr, (m_label + "_pv_yzErr/F").c_str());

  
}

void MuNtupleEnvironmentFiller::clear()
{

  m_truePileUp   = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_actualPileUp = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;

  m_instLumi = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;

  m_nPV = 0;

  m_pv_x = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_y = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_z = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;

  m_pv_xxErr = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_yyErr = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_zzErr = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_xyErr = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_xzErr = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_yzErr = MuNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  
}

void MuNtupleEnvironmentFiller::fill(const edm::Event & ev)
{

  clear();

  auto puInfo = conditionalGet<std::vector<PileupSummaryInfo> >(ev, m_puInfoToken, "vector<PileupSummaryInfo>");

  if (puInfo.isValid()) 
    {

      for (const auto & puInfoBx : (*puInfo))
	{

	  int bx = puInfoBx.getBunchCrossing();
	       
	  if(bx == 0) 
	    { 
	      m_truePileUp   = puInfoBx.getTrueNumInteractions();
	      m_actualPileUp = puInfoBx.getPU_NumInteractions();
	      break;
	    }
	}
    }

  auto lumiScalers = conditionalGet<LumiScalersCollection>(ev, m_lumiScalerToken, "LumiScalersCollection");

  if (lumiScalers.isValid()) 
    {
      m_instLumi = lumiScalers->begin()->instantLumi();
    }

  auto primaryVtx = conditionalGet<reco::VertexCollection>(ev, m_primariVerticesToken,"VertexCollection");

  if (primaryVtx.isValid()) 
    {

      m_nPV = primaryVtx->size();

      if (m_nPV > 0) 
	{
	  const auto& firstPV = (*primaryVtx)[0];
	  
	  m_pv_x = firstPV.position().x();
	  m_pv_x = firstPV.position().x();
	  m_pv_x = firstPV.position().x();
    
	  m_pv_xxErr = firstPV.covariance(0,0);
	  m_pv_yyErr = firstPV.covariance(1,1);
	  m_pv_zzErr = firstPV.covariance(2,2);

	  m_pv_xyErr = firstPV.covariance(0,1);
	  m_pv_xzErr = firstPV.covariance(0,2);
          m_pv_yzErr = firstPV.covariance(1,2);

	}
    }
    
  return;

}
