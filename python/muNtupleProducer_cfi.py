import FWCore.ParameterSet.Config as cms
from RecoMuon.TrackingTools.MuonServiceProxy_cff import MuonServiceProxy

muNtupleProducer = cms.EDAnalyzer("MuNtupleProducer",
                                  MuonServiceProxy,

                                  genPartTag = cms.untracked.InputTag("none"),

                                  puInfoTag = cms.untracked.InputTag("none"),
                                  lumiScalerTag = cms.untracked.InputTag("none"),
                                  primaryVerticesTag = cms.untracked.InputTag("offlinePrimaryVertices"),

                                  dtDigiTag = cms.untracked.InputTag("muonDTDigis"),
                                  rpcDigiTag = cms.untracked.InputTag("muonRPCDigis"),
                                  gemDigiTag = cms.untracked.InputTag("muonGEMDigis"),
                                  #dtDigiTagPh2 = cms.untracked.InputTag("none"),

                                  rpcRecHitTag = cms.untracked.InputTag("rpcRecHits"),
                                  gemRecHitTag = cms.untracked.InputTag("gemRecHits"),

                                  dtSegmentTag = cms.untracked.InputTag("dt4DSegments"),        
                                  gemSegmentTag = cms.untracked.InputTag("gemSegments"),
                                  cscSegmentTag = cms.untracked.InputTag("cscSegments"),
                                  #dtSegmentTagPh2 = cms.untracked.InputTag("none"),

                                  muonTag = cms.untracked.InputTag("muons"),

                                  trigEventTag = cms.untracked.InputTag("none"),
                                  trigResultsTag = cms.untracked.InputTag("none"),

                                  trigName = cms.untracked.string('none'),
                                  isoTrigName = cms.untracked.string('none'),

                                  twinMuxInTag  = cms.untracked.InputTag("twinMuxStage2Digis","PhIn"),
                                  twinMuxOutTag = cms.untracked.InputTag("twinMuxStage2Digis","PhOut"),
                                  bmtfInPhiTag  = cms.untracked.InputTag("bmtfDigis"),

                                  twinMuxInThTag = cms.untracked.InputTag("twinMuxStage2Digis","ThIn"),
                                  bmtfInThTag  = cms.untracked.InputTag("bmtfDigis"),

                                  bmtfOutTag  = cms.untracked.InputTag("bmtfDigis", "BMTF"),

                                  dttTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  dttTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                            doTOFCorrection = cms.bool(False),
                                                                            tofCorrType = cms.int32(2),
                                                                            wirePropCorrType = cms.int32(0),
                                                                            doWirePropCorrection = cms.bool(False),
                                                                            doT0Correction = cms.bool(True),
                                                                            tTrigLabel = cms.string(''),
                                                                            t0Label = cms.string(''),
                                                                            debug = cms.untracked.bool(False)
                                                                        ),

                                  dttTrigModePh2 = cms.untracked.string('DTTTrigSyncFromDB'),
                                  dttTrigModeConfigPh2 = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                            doTOFCorrection = cms.bool(False),
                                                                            tofCorrType = cms.int32(2),
                                                                            wirePropCorrType = cms.int32(0),
                                                                            doWirePropCorrection = cms.bool(False),
                                                                            doT0Correction = cms.bool(True),
                                                                            tTrigLabel = cms.string('ph2'),
                                                                            t0Label = cms.string('ph2'),
                                                                            debug = cms.untracked.bool(False)
                                                                        )

                            )
