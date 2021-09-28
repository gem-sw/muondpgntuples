import CRABClient
from WMCore.Configuration import Configuration 

config = Configuration()

config.section_("General")
config.General.requestName = 'GEMMuonNTuplizerOverMC'
config.General.workArea = "<Base_Path>/MuDPGAnalysis/MuonDPGNtuples/"
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '<Base_Path>/MuDPGAnalysis/MuonDPGNtuples/test/muDpgNtuples_cfg.py'
config.JobType.allowUndistributedCMSSW = True
# Zmumu
config.JobType.pyCfgParams = ['isMC=False','nEvents=-1']

config.section_("Data")
# MC
config.Data.inputDataset = "/Cosmics2021/caruta-Cosmics2021_112X_mcRun3_2021cosmics_realistic_deco_v13_RECO-7cd9c17a284f0ac1b81e68557875ecb2/USER"
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'Automatic'
config.Data.unitsPerJob = 180
config.Data.publication = False
config.Data.outLFNDirBase = '<output/path/>' # '/store/user/fivone/GEMMuonNtuplizerMC'
config.Data.outputDatasetTag = 'MC_Cosmic'

config.section_("Site")
config.Site.storageSite = '<your_storage_site>' #'T2_DE_RWTH'

config.section_("User")
config.User.voGroup = 'cms'
