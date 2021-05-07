import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras
from Configuration.AlCa.GlobalTag import GlobalTag

import subprocess
import sys

options = VarParsing.VarParsing()

options.register('globalTag',
                 '113X_mcRun3_2021_realistic_v9', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('nEvents',
                 1000, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('inputFolder',
                 '/RelValSingleMuPt100/CMSSW_11_3_0_pre6-113X_mcRun3_2021_realistic_v9-v1/GEN-SIM-RECO', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files")

options.register('secondaryInputFolder',
                 '/RelValSingleMuPt100/CMSSW_11_3_0_pre6-113X_mcRun3_2021_realistic_v9-v1/GEN-SIM-DIGI-RAW', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files for secondary files")

options.register('ntupleName',
                 './MuDPGNtuple_11_3_0_pre6_Run3_SingleMuPt100.root', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple")

options.parseArguments()

process = cms.Process("MUNTUPLES",eras.Run3)

#process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
                                        numberOfThreads = cms.untracked.uint32(4))
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string(options.globalTag)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(),
                            secondaryFileNames = cms.untracked.vstring()
)

files = subprocess.check_output(['dasgoclient', '--query', 'file dataset={}'.format(options.inputFolder)])
process.source.fileNames = [f for f in files.split() if ".root" in f]

print(process.source.fileNames)

if options.secondaryInputFolder != "" :
    files = subprocess.check_output(['dasgoclient', '--query', 'file dataset={}'.format(options.secondaryInputFolder)])
    process.source.secondaryFileNames = [f for f in files.split() if ".root" in f]

    print(process.source.secondaryFileNames)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(options.ntupleName)
)

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi')

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('MuDPGAnalysis.MuonDPGNtuples.muNtupleProducer_cfi')

process.p = cms.Path(process.muonDTDigis
                     + process.muonRPCDigis
                     + process.twinMuxStage2Digis
                     + process.bmtfDigis
                     + process.muNtupleProducer)

