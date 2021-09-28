- [MuonDPGNtuples](#muondpgntuples)
  * [Installation](#installation)
    + [Download](#download)
    + [Compile](#compile)
  * [How to run the Ntuplzier](#how-to-run-the-ntuplzier)
    + [Run the NTuplizer interactively](#run-the-ntuplizer-interactively)
    + [Run the NTuplizer with CRAB](#run-the-ntuplizer-with-crab)
  * [Overview of the ntuple structure](#overview-of-the-ntuple-structure)
  * [How to include quantities from your subsystem into the ntuple](#how-to-include-quantities-from-your-subsystem-into-the-ntuple)
  * [Current Branches](#current-branches)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>


# MuonDPGNtuples
Repository to host common ntuples developed and maintained by the CMS muon DPGs.
So far, two datasets have been tested as compatible with the current version:
```
/Cosmics/Commissioning2021-PromptReco-v1/AOD
/ExpressCosmics/Commissioning2021-Express-v1/FEVT
```
## Installation
### Download 
You may want to use a specific CMSSW  version (from now on referred as CMSSW_XXXX) and global tag based on the data you are about to Ntuplize:
- For **P5 data** check  [Global Tags for Conditions Data ](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions)
- For **MC data** check the production parameters

```
cmsrel CMSSW_XXXX 
cd CMSSW_XXXX/src/ 
cmsenv

git clone https://github.com/gem-sw/muondpgntuples MuDPGAnalysis/MuonDPGNtuples
```

### Compile
```
scram b
```
## How to run the Ntuplzier
### Run the NTuplizer interactively
- MC and MWGR Data use different **globalTag** and **CMSSW Release**. Edit config file (`test/muDpgNtuples_cfg.py`) accordingly!

```
cd MuDPGAnalysis/MuonDPGNtuples/test/
cmsRun muDpgNtuples_cfg.py isMC=False  nEvents=-1 inputFolder=/eos/cms//store/express/Commissioning2021/ExpressCosmics/FEVT/Express-v1/000/341/112/00000/
```

the current commit, provided with these options, will analyze:
- The files in the folder `inputFolder`
- It will assume it is NOT a MC datasample, so will NOT fill the SimHit branches
- Will analyze all the events in the files (nEvents = -1)


Change the settings above based on your intended use.
### Run the NTuplizer with CRAB
- You have to authenticate yourself by mean of the grid certificate ( check the  [Twiki](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookStartingGrid#Using_your_grid_certificate) )
- Edit the file `./CRAB_SUB/*_crabConfig.py`  providing:
   * Job paramters (i.e. nEvents, isMC, run number, ... )
   * The input dataset
   * `<Base_Path>` has to point to `CMSSW_XXXX/src/` currently in use   
   * Output site name and output path name
- Execute `crab submit -c  ./CRAB_SUB/crabConfig.py`

## Overview of the ntuple structure

These ntuples are intended to consist mostly of flat collection of `std::vectors<>`, with the exception for few `TClonesArrays` which handle "vectors of vectors".

The class steering ntuple production is called `MuNtupleProducer`. It runs a list of helper classes, called _fillers_ inheriting from `MuNtupleBaseFiller`.

Each _filler_ must include the `initialize()`, `clear()` and `fill()` functions, which are called within the `MuNtupleProducer` `beginJob()` and `analyze()` functions to perform the ntuple filling logic.

Many fillers of the same type may exist, for example in the DT slice-test the same data format for phase-1 and phase-2 digis is used, implying that the same _filler_ helper class can be reused to reduce code duplication. Different flavours of a filler are configured using _labels_ and _tags_. A _label_ specifies the initial part of the name of the branches that a _filler_ writes into the output `TTree`, for example `dtDigi_time` versus `ph2DtDigi_time`. A _tag_ is used to configure the specific behaviour of a given filler, for example phase-1 and phase-2 digis from the DT slice-test are produced with different `InputTags`, hence, in the DT digi _filler_ case, the _tag_ will specify what input collection should be used.

The _fillers_ included in the ntuple are defined in the `MuNtupleProducer` constructor with as in the following:

```
[...]

  m_fillers.push_back(std::make_unique<MuNtupleDTDigiFiller>(consumesCollector(), m_config, m_tree, "dtDigi",    MuNtupleDTDigiFiller::Tag::PH1));
  m_fillers.push_back(std::make_unique<MuNtupleDTDigiFiller>(consumesCollector(), m_config, m_tree, "ph2DtDigi", MuNtupleDTDigiFiller::Tag::PH2));

[...]
```

`dtDigi` or `ph2DtDigi` are _labels_ , and `MuNtupleDTDigiFiller::Tag::PH1` or `MuNtupleDTDigiFiller::Tag::PH1` are _tags_ of the same _filler_ base class.

A configuration helper class, called `MuNtupleConfig` also exists.

It deals with: _(i)_ the list of `InputTags` used by all _fillers_, _(ii)_ geometry, calibration and other `EventSetup` quantities.

The configuration of the ntuple producer is available under `python/muNtupleProducer_cfi.py`.

## How to include quantities from your subsystem into the ntuple

1. Create one (or more) filler(s), which inherit from `src/MuNtupleBaseFiller.{h,cc}` under the `src/` dircetory;
1. Add all relevant `InputTag` and `EventSetup` quantities into `src/MuNtupleConfig.{h,cc}`;
1. Include your filler into the `m_fillers` vector of the `plugins/MuNtupleProducer.{h,cc}` steering class;
1. Include all the needed configuration parameters in `python/muNtupleProducer_cfi.py;
1. Adjust `Buildfile.xml` as needed;
1. Compile, run ` cmsRun test/muDpgNtuples_cfg.py` and check that the ntuple includes your new quantities.

**NOTEs**: 
- a _filler_ .h file is the best place where to document the quantities that are filled (e.g. mention units, ranges, etc), look [`src/ MuNtupleDTDigiFiller.h`]() as an example;
- the `MuNtupleBaseFiller` class includes placeholders for default values in the ntuple (e.g. `DEFAULT_INT_VAL`), please use those (and not "magic" numbers) when filling with default values is needed;
- the `MuNtupleBaseFiller` class includes a `conditionalGet()` function that provides a default dump in case a collection is missing in the input file, please use it.

A complete example showing how to include phase-1 and phase-2 DT digis is available [here]().

## Current Branches
```
        muNtupleProducer (TDirectory)
                MuDPGTree (TTree)
                        event_runNumber/I (TBranch)
                        event_lumiBlock/I (TBranch)
                        event_eventNumber/L (TBranch)
                        gemDigi_nDigis/i (TBranch)
                        gemDigi_station (TBranchElement)
                        gemDigi_region (TBranchElement)
                        gemDigi_roll (TBranchElement)
                        gemDigi_bx (TBranchElement)
                        gemDigi_strip (TBranchElement)
                        gemDigi_pad (TBranchElement)
                        gemDigi_g_r (TBranchElement)
                        gemDigi_g_phi (TBranchElement)
                        gemDigig_eta (TBranchElement)
                        gemDigi_g_x (TBranchElement)
                        gemDigi_g_y (TBranchElement)
                        gemDigi_g_z (TBranchElement)
                        gemRecHit_nRecHits/i (TBranch)
                        gemRecHit_cluster_size (TBranchElement)
                        gemRecHit_firstClusterStrip (TBranchElement)
                        gemRecHit_bx (TBranchElement)
                        gemRecHit_region (TBranchElement)
                        gemRecHit_chamber (TBranchElement)
                        gemRecHit_layer (TBranchElement)
                        gemRecHit_etaPartition (TBranchElement)
                        gemRecHit_loc_r (TBranchElement)
                        gemRecHit_loc_phi (TBranchElement)
                        gemRecHit_loc_x (TBranchElement)
                        gemRecHit_loc_y (TBranchElement)
                        gemRecHit_loc_z (TBranchElement)
                        gemRecHit_loc_errX (TBranchElement)
                        gemRecHit_loc_errY (TBranchElement)
                        gemRecHit_g_r (TBranchElement)
                        gemRecHit_g_phi (TBranchElement)
                        gemRecHit_g_x (TBranchElement)
                        gemRecHit_g_y (TBranchElement)
                        gemRecHit_g_z (TBranchElement)
                        gemSegment_nSegments/i (TBranch)
                        gemSegment_region (TBranchElement)
                        gemSegment_ring (TBranchElement)
                        gemSegment_station (TBranchElement)
                        gemSegment_posLoc_x (TBranchElement)
                        gemSegment_posLoc_y (TBranchElement)
                        gemSegment_posLoc_z (TBranchElement)
                        gemSegment_dirLoc_x (TBranchElement)
                        gemSegment_dirLoc_y (TBranchElement)
                        gemSegment_dirLoc_z (TBranchElement)
                        gemSegment_posGlb_x (TBranchElement)
                        gemSegment_posGlb_y (TBranchElement)
                        gemSegment_posGlb_z (TBranchElement)
                        gemSegment_time (TBranchElement)
                        gemSegment_time_err (TBranchElement)
                        gemSegment_chi2 (TBranchElement)
                        gemSegment_posGlb_phi (TBranchElement)
                        gemSegment_posGlb_eta (TBranchElement)
                        gemSegment_dirGlb_phi (TBranchElement)
                        gemSegment_dirGlb_eta (TBranchElement)
                        mu_nMuons/i (TBranch)
                        mu_pt (TBranchElement)
                        mu_phi (TBranchElement)
                        mu_eta (TBranchElement)
                        mu_charge (TBranchElement)
                        mu_isGlobal (TBranchElement)
                        mu_isStandalone (TBranchElement)
                        mu_isTracker (TBranchElement)
                        mu_isGEM (TBranchElement)
                        mu_isCSC (TBranchElement)
                        mu_isME11 (TBranchElement)
                        mu_isLoose (TBranchElement)
                        mu_isMedium (TBranchElement)
                        mu_isTight (TBranchElement)
                        mu_propagated_isME11 (TBranchElement)
                        mu_propagated_TrackNormChi2 (TBranchElement)
                        mu_propagated_numberOfValidPixelHits (TBranchElement)
                        mu_propagated_innerTracker_ValidFraction (TBranchElement)
                        mu_propagated_numberOfValidTrackerHits (TBranchElement)
                        mu_path_length/F (TBranch)
                        mu_isinsideout (TBranchElement)
                        mu_isincoming (TBranchElement)
                        mu_propagated_region (TBranchElement)
                        mu_propagated_layer (TBranchElement)
                        mu_propagated_chamber (TBranchElement)
                        mu_propagated_etaP (TBranchElement)
                        mu_propagated_pt (TBranchElement)
                        mu_propagated_phi (TBranchElement)
                        mu_propagated_eta (TBranchElement)
                        mu_propagated_charge (TBranchElement)
                        mu_propagatedLoc_x (TBranchElement)
                        mu_propagatedLoc_y (TBranchElement)
                        mu_propagatedLoc_z (TBranchElement)
                        mu_propagatedLoc_r (TBranchElement)
                        mu_propagated_isGEM (TBranchElement)
                        mu_propagatedLoc_phi (TBranchElement)
                        mu_propagatedLoc_errX (TBranchElement)
                        mu_propagatedLoc_errY (TBranchElement)
                        mu_propagatedLoc_dirX (TBranchElement)
                        mu_propagatedLoc_dirY (TBranchElement)
                        mu_propagatedLoc_dirZ (TBranchElement)
                        mu_propagatedGlb_x (TBranchElement)
                        mu_propagatedGlb_y (TBranchElement)
                        mu_propagatedGlb_z (TBranchElement)
                        mu_propagatedGlb_r (TBranchElement)
                        mu_propagatedGlb_phi (TBranchElement)
                        mu_propagatedGlb_errX (TBranchElement)
                        mu_propagatedGlb_errY (TBranchElement)
                        mu_propagatedGlb_errR (TBranchElement)
                        mu_propagatedGlb_errPhi (TBranchElement)
                        mu_propagated_EtaPartition_centerX (TBranchElement)
                        mu_propagated_EtaPartition_centerY (TBranchElement)
                        mu_propagated_EtaPartition_rMax (TBranchElement)
                        mu_propagated_EtaPartition_rMin (TBranchElement)
                        mu_propagated_EtaPartition_phiMax (TBranchElement)
                        mu_propagated_EtaPartition_phiMin (TBranchElement)
                        mu_propagated_nME1hits (TBranchElement)
                        mu_propagated_nME2hits (TBranchElement)
                        mu_propagated_nME3hits (TBranchElement)
                        mu_propagated_nME4hits (TBranchElement)
                        mu_propagated_Innermost_x (TBranchElement)
                        mu_propagated_Innermost_y (TBranchElement)
                        mu_propagated_Innermost_z (TBranchElement)
                        mu_propagated_Outermost_x (TBranchElement)
                        mu_propagated_Outermost_y (TBranchElement)
                        mu_propagated_Outermost_z (TBranchElement)
```

