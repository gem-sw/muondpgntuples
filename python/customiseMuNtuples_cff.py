import FWCore.ParameterSet.Config as cms

def customiseForRunningOnMC(process, pathName) :

    if hasattr(process,"muNtupleProducer") :
        print("[customiseForRunningOnMC]: updating ntuple input tags")

        process.muNtupleProducer.genPartTag = "prunedGenParticles"
        process.muNtupleProducer.puInfoTag = "addPileupInfo"

        process.muNtupleProducer.lumiScalerTag = "none"

        # CB temporary hacks, until proper packing/unpacking
        process.muNtupleProducer.rpcDigiTag = "simMuonRPCDigis" 

        process.muNtupleProducer.twinMuxInTag = "none"
        process.muNtupleProducer.twinMuxOutTag = "none"
        process.muNtupleProducer.twinMuxInThTag = "none"

        if hasattr(process,pathName) :
            print("[customiseForRunningOnMC]: adding prunedGenParitcles")

            process.load('MuDPGAnalysis.MuonDPGNtuples.prunedGenParticles_cfi')

            getattr(process,pathName).replace(process.muNtupleProducer,
                                              process.prunedGenParticles
                                              + process.muNtupleProducer)

    return process
