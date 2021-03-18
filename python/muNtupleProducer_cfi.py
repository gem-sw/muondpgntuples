import FWCore.ParameterSet.Config as cms

muNtupleProducer = cms.EDAnalyzer("MuNtupleProducer",
                                  storeRpcRecHits = cms.untracked.bool(True),
				  storeRpcDigiHits = cms.untracked.bool(True),
                                  rpcDigiLabel = cms.untracked.InputTag("muonRPCDigis"),
                                  rpcRecHitLabel = cms.untracked.InputTag("rpcRecHits"),
                                  ph1DtDigiTag = cms.untracked.InputTag("muonDTDigis"),
                                  ph2DtDigiTag = cms.untracked.InputTag("none"),

                                  ph1DtSegmentTag = cms.untracked.InputTag("dt4DSegments"),        
                                  ph2DtSegmentTag = cms.untracked.InputTag("none"),

                                  ph1DTtTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  ph1DTtTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                            doTOFCorrection = cms.bool(False),
                                                                            tofCorrType = cms.int32(2),
                                                                            wirePropCorrType = cms.int32(0),
                                                                            doWirePropCorrection = cms.bool(False),
                                                                            doT0Correction = cms.bool(True),
                                                                            tTrigLabel = cms.string(''),
                                                                            debug = cms.untracked.bool(False)
                                                                        )
        )
