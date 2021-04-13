#ifndef MuNtuple_MuNtupleBmtfFiller_h
#define MuNtuple_MuNtupleBmtfFiller_h

/** \class MuNtupleBmtfFiller MuNtupleBmtfFiller.h MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBmtfFiller.h
 *  
 * Helper class : the BMTF filler
 *
 * \author L. Borgonovi (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTTPGPhiFiller.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
 
#include <vector>

class MuNtupleBmtfFiller : public MuNtupleBaseFiller
{

 public:

  /// Constructor
  MuNtupleBmtfFiller(edm::ConsumesCollector && collector,
		     const std::shared_ptr<MuNtupleConfig> config, 
		     std::shared_ptr<TTree> tree, const std::string & label);

  ///Destructor
  virtual ~MuNtupleBmtfFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// Scale to convert HW pt to GeV
  static constexpr double PT_SCALE = 0.5;

  /// Scale to convert HW eta to phisical value
  static constexpr double ETA_SCALE = 0.010875;

  /// Scale to convert HW eta to rad
  static constexpr double PHI_SCALE = 0.010908308; // 2 * pi / 576 

  /// Number of DT stations
  static constexpr int N_STAT= 0.5;

  /// The gen particles token
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_dtBmtfToken;
  edm::EDGetTokenT<L1MuDTChambPhContainer> m_dtTpgPhiToken;

  unsigned int m_nBmtfCands; // the # of BMTF candidates (size of all following vectors)    

  std::vector<float> m_tf_pt;       // BMTF cand pt  [GeV/c]
  std::vector<float> m_tf_phi;      // BMTF cand phi [rad]
  std::vector<float> m_tf_eta;      // BMTF cand eta 
  std::vector<int> m_tf_bx;         // BMTF cand BX
  std::vector<int> m_tf_dxy;        // BMTF cand dxy [cm??] CB check this
  std::vector<int> m_tf_qual;       // BMTF cand quality
  std::vector<int> m_tf_etaFine;    // fine eta bit
  TClonesArray *m_matchedTpIdx;     // index of BMTF input TPs
};
  
#endif
