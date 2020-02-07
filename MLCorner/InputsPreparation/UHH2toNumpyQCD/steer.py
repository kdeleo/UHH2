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
procnames = ['DY', 'Diboson', 'QCD', 'ST', 'TTbar', 'WJets']
#name of branches to be skipped in conversion
## exact names
unwanted_exact_tags = ['event', 'isRealData', 'luminosityBlock', 'passEcalBadCalib','rho', 'run', 'year', 'genInfo']
## partial names to exclude common set of variables at once
unwanted_tags = ['beamspot_', 'prefiringWeight', 'trigger', 'slimmed', 'offlineSlimmedPrimaryVertices_', 'GenParticles', 'offlineSlimmedPrimaryVertices', 'genjetsAk8SubstructureSoftDrop', 'jetsAk4Puppi', 'jetsAk8CHSSubstructure_SoftDropCHS', 'jetsAk8PuppiSubstructure_SoftDropPuppi', 'm_']

syst_vars = ['NOMINAL']
for syst_var in syst_vars:
    print'--- Convert inputs for ',syst_var,' variation ---'
    ModuleRunner.ReadoutMLVariables(procnames=procnames,unwanted_tags=unwanted_tags, unwanted_exact_tags=unwanted_exact_tags,syst_var=syst_var)

