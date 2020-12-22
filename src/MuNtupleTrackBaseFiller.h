#ifndef MuNtuple_MuNtupleTrackBaseFiller_h
#define MuNtuple_MuNtupleTrackBaseFiller_h

/**
 *  
 *
 * Helper class developed by G. Milella on the basis of the BaseFiller developed by C. Battilana.
 *The difference wrt to the BaseFiller is in the the method fill_new that allow to access also to the eventSetup variables 
 *(while the fill() method in the BaseFiller just allow access to the Event)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleConfig.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TTree.h"

#include <memory>
#include <string>

class MuNtupleTrackBaseFiller
{

 public :

  /// Constructor
  MuNtupleTrackBaseFiller(const std::shared_ptr<MuNtupleConfig> config, 
		     std::shared_ptr<TTree> tree, const std::string & label);
  
  /// Destructor
  virtual ~MuNtupleTrackBaseFiller();

  /// Intialize function : setup tree branches etc ...
  virtual void initialize() = 0;

  /// Clear branches before event filling
  virtual void clear() = 0;

    
  virtual void fill_new(const edm::Event & ev, const edm::EventSetup & environment) = 0;

 protected :

  /// Definition of default values for int variables
  static constexpr int DEFAULT_INT_VAL = -999;

  /// Definition of default values for positive int variables
  static constexpr int DEFAULT_INT_VAL_POS = -1;

  /// Definition of default values for float variables
  static constexpr double DEFAULT_DOUBLE_VAL = -999.;

  /// Definition of default values for positive float variables
  static constexpr double DEFAULT_DOUBLE_VAL_POS = -1.;
 
  /// Ponter to the configuration
  std::shared_ptr<MuNtupleConfig> m_config;

  /// Pointer to the TTree
  std::shared_ptr<TTree> m_tree;

  /// The label at the beginning of each branch
  std::string m_label;

 
  /// Conditional getter :
  /// checks whether a token is valid and if
  /// retireving the data collection succeded
  template<typename T> edm::Handle<T> conditionalGet(const edm::Event & ev,
						     const edm::EDGetTokenT<T> & token,
						     const std::string & collectionName)
    {

      edm::Handle<T> collection ;

      if (!token.isUninitialized()) 
	{
	  if (!ev.getByToken(token, collection)) 
	    edm::LogError("") << "[MuNtuple]::conditionalGet: "
			      << collectionName << " collection does not exist !!!";
	}
      
      return collection;

    }
};

#endif
