#ifndef MuNtuple_MuNtupleRPCDigiFiller_h
#define MuNtuple_MuNtupleRPCDigiFiller_h

/** \class MuNtupleRPCDigiFiller MuNtupleRPCDigiFiller.h MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleRPCDigiFiller.h
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 DT digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"

#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>
#include <cstdint>


class MuNtupleRPCDigiFiller : public MuNtupleBaseFiller
{

 public:

  //enum class Tag { PH1 = 0, PH2 };

  /// Constructor
  MuNtupleRPCDigiFiller(edm::ConsumesCollector && collector,
		     const std::shared_ptr<MuNtupleConfig> config, 
		     std::shared_ptr<TTree> tree, const std::string & label
		     );

  ///Destructor
  virtual ~MuNtupleRPCDigiFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :
 
  // members not for branch filling 
  edm::EDGetTokenT<RPCDigiCollection> m_rpcDigiToken;

  
  // members for branch filling
  int m_nDigi;

    
  std::vector<uint16_t> m_strip;
  std::vector<int32_t> m_bx;
  std::vector<double> m_time;
  std::vector<double> m_coordinateX;
  std::vector<double> m_coordinateY;

  std::vector<int> m_region; // 0: Barrel, +-1: Endcap
  std::vector<int> m_ring; // Ring id: Wheel number in Barrel (from -2 to +2) Ring Number in Endcap (from 1 to 3)
  std::vector<int> m_station; // chambers at same R in barrel, chamber at same Z ion endcap
  std::vector<int> m_layer; // Layer id: in station 1 and 2 for barrel, we have two layers of chambers: layer 1 is the inner chamber and layer 2 is the outer chamber
  std::vector<int> m_sector; // group of chambers at same phi
  std::vector<int> m_subsector; // some sectors are divided along the phi direction in subsectors (from 1 to 4 in Barrel, from 1 to 6 in Endcap)
  std::vector<int> m_roll; // Roll id (also known as eta partition) each chamber is divided along the strip direction in
    
  std::vector<uint32_t> m_rawId; // unique detector unit ID






};
  
#endif

