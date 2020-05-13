#!/usr/bin/env python

import subprocess
import os
import time

from python.ModuleRunner import *
from python.constants import *


"""This is macro to steer Root to Numpy conversion. """


#paths are set in constants.py
ModuleRunner = ModuleRunner(path_MLDIR, outpath)

# ---- Macro for ML inputs preparation ----
#names of the process, e.g part after uhh2.AnalysisModuleRunner. in the input file name
procnames = ['TTbar', 'ST', 'WJets_1', 'WJets_2','WJets_3', 'DY']
#procnames = ['ZprimeToTTJet_M1000_2017v2', 'ZprimeToTT_M1000_W100_2017v2', 'ZprimeToTT_M3000_W900_2017v2']
#name of branches to be skipped in conversion
## exact names
unwanted_exact_tags = ['Mttbar', 'NPV', 'event', 'isRealData', 'luminosityBlock', 'met_pt', 'passEcalBadCalib','rho', 'run', 'GenParticles', 'offlineSlimmedPrimaryVertices', 'year', 'rec_chi2']
## partial names to exclude common set of variables at once
unwanted_tags = ['ak4jet1_', 'ak8jet1_', 'beamspot_', 'weight_pu', 'prefiringWeight', 'trigger', 'gen', 'jets', 'slimmed', 'm_', 'offlineSlimmedPrimaryVertices_', 'GenParticles_',
                  'lep1_', 'weight_', 'Ele', 'Ak4_j7', 'Ak4_j8', 'Ak4_j9', 'Ak4_j10', 'Ak8_j4', 'Ak8_j5']

syst_vars = ['NOMINAL']
for syst_var in syst_vars:
    print'--- Convert inputs for ',syst_var,' variation ---'
    ModuleRunner.ReadoutMLVariables(procnames=procnames,unwanted_tags=unwanted_tags, unwanted_exact_tags=unwanted_exact_tags,syst_var=syst_var)

