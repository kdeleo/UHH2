#!/usr/bin/env python
from __future__ import division
import subprocess
import glob
from os import system
import sys
from os import mkdir
from os.path import exists
from array import array
import math
import re
import time
import datetime
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlue, kBlack, kFALSE, kTRUE,kSienna,TLatex, Double, TMultiGraph, TString, TF1
from collections import OrderedDict
import CMSPlotStyle


class bcolors:
    HEADER = '\033[95m'
    RED = '\033[91m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW  = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    BR = '\033[94m' #BLue
    PROCESS = '\033[93m' #Yellow
    CHANNEL = '\033[95m' #Magenta
    SYSTEMATIC = '\033[96m' #Cyan

gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetPaintTextFormat("2.3f")

colors = [kBlack, kRed, kSpring, kOrange,kAzure,kSpring,kGreen, kBlue,kBlue,kBlue,kBlue,kBlue]
markers = [22,25,28,32,21,20,20,22,22,22,22,22,22,22,22]

berror = False
beps = True

###
# This script is used to calculate the JER. Dictonaires are used to group the files together that should end up in the same plot.
# The root files are produced with QCDEffPurityModule.cxx in https://gitlab.cern.ch/abenecke/PUPPI_tuning
###


# gathering the different hist to calculate the resolution
def get_hists(infile_dict,hist_folder):
    hists={}
    # make list for all histogramm JetPtResponse_* based on first file
    if infile_dict.has_key("PUPPI default"):
        lista = infile_dict["PUPPI default"].Get(hist_folder).GetListOfKeys()
    else:
        lista = infile_dict["PUPPI 2017 v6"].Get(hist_folder).GetListOfKeys()

    for infile  in infile_dict:
        print infile
        for el in lista:
            if "pt" in el.GetName(): continue
            if "Eta3to3" in el.GetName(): continue
            if "JetPtResponse_" in el.GetName():
                print el.GetName()

                # read in histogram JetPtResponse_*
                hist = infile_dict[infile].Get(hist_folder+"/"+el.GetName())
                if not hists.has_key(el.GetName()): hists[el.GetName()]={}
                hists[el.GetName()][infile] = hist

    return hists

# plots the different response histograms for each 10th bin
def plot_control(hists, folder):
    markers = [20,22,21,28,34,20,20,20,20,20,20]
    colors = [kBlack, kRed, (kAzure-4), kBlack,kAzure,kSpring,kGreen, kBlue,kBlue,kBlue,kBlue,kBlue]
    print "plot control"

    hist_dict = {}

    for key in hists:
        for pu in hists[key]:
            hist = hists[key][pu]
           
            for bin in range(0,hist.GetNbinsX()+1):
#                if bin%100 != 0: continue
                #if bin != 50: continue
                if bin%10 !=0: continue
                if bin>100 : continue

                # plot for each pT bin the distribution and save in folder+"/control/"
                projection = hist.ProjectionY("_y",bin,bin+1)
                c = TCanvas()


                projection.GetXaxis().SetTitle("p_{T}^{reco} / p_{T}^{gen}")
                projection.GetYaxis().SetTitle("Events")
                projection.GetXaxis().SetRangeUser(0.0,1.5)

                projection.Draw()


                info = extratext("pT " + str(bin),0.2,0.5)
                info.Draw()

                info2 = extratext(pu ,0.2,0.6)
                info2.Draw()

                etabins = re.findall(r'Eta\d*p*\d*to\d*p*\d*', key)
                eta = etabins[0].replace("Eta","").replace("p",".").replace("to","< |#eta| <")
                info3 = extratext(eta ,0.2,0.7)
                info3.Draw()

                name = pu.replace(" ","_")
                hist_dict[key+"_"+name+"_"+str(bin)] = projection.Clone()

                c.Print(folder+"/control/"+key+"_"+name+"_"+str(bin)+".eps")


    # plot different scale in same TCanvas
    print "============= JER scale"
    for key in hist_dict:
        print "####################3 new key " + key
        if not "PUPPI_default" in key: continue
        splitted = key.split("_")
        c1 = TCanvas()
        leg = TLegend(0.5,0.5,0.9,0.9,"","brNDC")        
        leg.SetBorderSize(0);
        leg.SetTextSize(0.035);
        leg.SetFillColor(0);
        leg.SetLineColor(1);
        leg.SetTextFont(42);

        i=0
        for key2 in hist_dict:
            if splitted[1] not in key2: continue
            if splitted[-1] not in key2.split("_")[-1]: continue
            if splitted[-1]=='0': continue

            print key2
            projection2  = hist_dict[key2]
            projection2.Rebin(2)
            if projection2.Integral() != 0:
                projection2.Scale(1/projection2.Integral())
            projection2.GetXaxis().SetTitle("p_{T}^{reco} / p_{T}^{gen}")
            projection2.GetYaxis().SetTitle("Events")
            projection2.GetXaxis().SetRangeUser(0.0,3)

            print i
            projection2.SetLineColor(colors[i])
            projection2.SetMarkerColor(colors[i])
            projection2.SetMarkerStyle(markers[i])
            c1.cd()
            projection2.Draw("same")
            legname = TString(key2)
            legname.ReplaceAll("JetPtResponse_","")
            legname.ReplaceAll(re.findall(r'Eta\d*p*\d*to\d*p*\d*_', key2)[0],"")
            legname.ReplaceAll("_"+key2.split("_")[-1],"")
            print legname
            legname = str(legname)
            leg.AddEntry(projection2,legname,"lp")
            i+=1

        # info = extratext("pT " + str(key2.split("_")[-1]),0.2,0.5)
        # info.Draw()
        
        # info2 = extratext(pu ,0.2,0.6)
        # info2.Draw()
        
        etabins = re.findall(r'Eta\d*p*\d*to\d*p*\d*', key)
        eta = etabins[0].replace("Eta","").replace("p",".").replace("to","< |#eta| <")
        info3 = extratext(eta ,0.7,0.7)
        info3.Draw()
        
        leg.Draw()
            
        c1.Print(folder+"/control_all/"+key+".eps")

        


# used to place extra text in the Canvas if needed
def extratext(text,x,y):
    info = TLatex(3.5, 24, text);
    info.SetNDC()
    info.SetX(x)
    info.SetY(y)
    info.SetTextFont(52)
    info.SetTextSize(0.035)

    return info


# calculate the resolution
def get_reso(hists,folder):
    reso_hists = {}
    print "get mean and rms"

    for key in hists:
        for pu in hists[key]:
            hist = hists[key][pu]
            print hist.GetNbinsX()
            reso = TH1F("reso","Resolution",hist.GetNbinsX(),0,hist.GetNbinsX())
            mean_h = TH1F("mean_h","Mean",hist.GetNbinsX(),0,hist.GetNbinsX())
            rms_h = TH1F("rms_h","RMS",hist.GetNbinsX(),0,hist.GetNbinsX())
           
            for bin in range(0,hist.GetNbinsX()+1):
                #if bin != 50 : continue
                if bin%10 : continue
                if bin>100 and bin%30: continue
                projection = hist.ProjectionY("_y",bin,bin+1)
                projection.GetXaxis().SetRangeUser(0.5,1.5)
                #projection.GetXaxis().SetRangeUser(-10.0,10.0)
                mean = projection.GetMean()
                rms = projection.GetRMS()

                resolution = 0
                if rms !=0:
                    resolution = rms/mean

                print key
                print "mean = %.3f, rms = %.3f, resolution = %.3f " % (mean, rms, resolution)


                # Gaussian fit of the peaks
                gaussian_fit = TF1("gaussian_fit", "gaus", 0.0, 3.0);
                gaussian_fit.SetParameter(1, 1.0);
                gaussian_fit.SetParameter(2, 0.1);
                projection.Fit(gaussian_fit, "R"); 


                for i in range(0,2):
                    lower_bound = gaussian_fit.GetParameter(1) -1.5*gaussian_fit.GetParameter(2);
                    higher_bound = gaussian_fit.GetParameter(1)+1.5*gaussian_fit.GetParameter(2);
                    
                    gaussian_fit = TF1("gaussian_fit", "gaus",lower_bound,higher_bound);
                    gaussian_fit.SetParameter(1, gaussian_fit.GetParameter(1));
                    gaussian_fit.SetParameter(2, gaussian_fit.GetParameter(2));
                    
                    fit_result = projection.Fit(gaussian_fit,"R");
                    
                #c1 = TCanvas() 
                #projection.Draw()
                #text = CMSPlotStyle.draw_info("Chi2/NDF %.2f"%(gaussian_fit.GetChisquare()/gaussian_fit.GetNDF()),0.92,0.87)
                #text.Draw()              
                #text2 = CMSPlotStyle.draw_info("reso %.2f"%(gaussian_fit.GetParameter(2)),0.92,0.7)
                #text2.Draw()
                #text3 = CMSPlotStyle.draw_info("rms %.2f"%(rms),0.92,0.5)
                #text3.Draw()
                #name = pu.replace(" ","_")
                #c1.Print(folder + "gaussian"+"_"+key+"_"+name+"_"+str(bin)+".eps") 

                resolution_2 = 0
                if gaussian_fit.GetParameter(2) !=0:
                    resolution_2 = gaussian_fit.GetParameter(2)/gaussian_fit.GetParameter(1)
 
                print "mean gaussian = %.3f, rms gaussian = %.3f, resolution gaussian = %.3f " % (gaussian_fit.GetParameter(1), gaussian_fit.GetParameter(2), resolution_2) 


                reso.SetBinContent(bin,resolution)
                reso.SetBinError(bin,projection.GetRMSError())
                #reso.SetBinContent(bin,resolution_2)
                #reso.SetBinError(bin,gaussian_fit.GetParError(2))
                mean_h.SetBinContent(bin,mean)
                mean_h.SetBinError(bin,projection.GetRMSError())
                rms_h.SetBinContent(bin,rms)
    
            if not reso_hists.has_key(key): reso_hists[key] = {}
            if not reso_hists[key].has_key(pu): reso_hists[key][pu] = {}

            reso_hists[key][pu]["reso"] = reso
            reso_hists[key][pu]["mean"] = mean_h
            reso_hists[key][pu]["rms"] = rms_h

    return reso_hists


#plots the resolution for the different hists
def plot_reso(reso_hists, folder, reso_mean_rms, name, ymin=0.1, ymax=0.4,blogy = False):
    print "plot" + name + "  "+ str(len(reso_hists))
    markers = [22,21,20,28,34,20,20,20,20,20,20]
    colors = [kRed, (kAzure-4), kBlack,kAzure,kSpring,kGreen, kBlue,kBlue,kBlue,kBlue,kBlue,kRed] 

    for key in reso_hists:
        c = TCanvas()
        leg = TLegend(0.5,0.5,0.9,0.85, "","brNDC")
        leg.SetBorderSize(0);	
        leg.SetFillStyle(0);
        leg.SetTextSize(0.035);

        i=0
        jj=0
        for pu in reso_hists[key]:
            #if i == 2 or i==4: i+=1
            reso_hists[key][pu][reso_mean_rms].SetLineColor(colors[i])
            reso_hists[key][pu][reso_mean_rms].SetMarkerColor(colors[i])
            reso_hists[key][pu][reso_mean_rms].SetMarkerStyle(markers[jj])
            reso_hists[key][pu][reso_mean_rms].SetMarkerSize(1.2)
            reso_hists[key][pu][reso_mean_rms].GetXaxis().SetTitle("p_{T}^{gen}")
            reso_hists[key][pu][reso_mean_rms].GetYaxis().SetTitle(name)
            if "resolution" in name:
                reso_hists[key][pu][reso_mean_rms].GetYaxis().SetTitle("#sigma/mean")

            if "mean" in name:
                reso_hists[key][pu][reso_mean_rms].GetYaxis().SetTitle("response (mean)")

            if "Pt" in key:
                reso_hists[key][pu][reso_mean_rms].GetXaxis().SetRangeUser(30,10000)
            reso_hists[key][pu][reso_mean_rms].GetYaxis().SetRangeUser(ymin,ymax)
            reso_hists[key][pu][reso_mean_rms].Draw("PE same")
            leg.AddEntry(reso_hists[key][pu][reso_mean_rms],pu,"lpe")
            i+=1
            jj+=1


        leg.Draw()
        CMSPlotStyle.extratext = "Simulation"
        text = CMSPlotStyle.draw_cmstext("left", True)
        text[0].Draw()
        text[1].Draw()
        lumi = CMSPlotStyle.draw_lumi(True)
        lumi.Draw()
        
        anti = CMSPlotStyle.draw_info("Anti-k_{T}, R=0.4,",0.7,0.87)
        anti.Draw()
        
        if "central" in key: 
            eta = CMSPlotStyle.draw_info("|#eta| < 2.5",0.92,0.87)
            eta.Draw()
            
        if "1p3to2" in key:
            eta = CMSPlotStyle.draw_info("1.3 < |#eta| < 2",0.92,0.87)
            eta.Draw()

        if "2to2p5" in key:
            eta = CMSPlotStyle.draw_info("2 < |#eta| < 2.5",0.92,0.87)
            eta.Draw()


        if "2p5to3" in key:
            eta = CMSPlotStyle.draw_info("2.5 < |#eta| < 3.0",0.92,0.87)
            eta.Draw()

        if "0to1p3" in key:
            eta = CMSPlotStyle.draw_info(" |#eta| < 1.3",0.92,0.87)
            eta.Draw()

        if "3to10" in key:
            eta = CMSPlotStyle.draw_info("3.0 < |#eta| < 5.0",0.92,0.87)
            eta.Draw()

        if "Pt" in key:
            c.SetLogx()

        if blogy:
            c.SetLogy()
            
        c.Print(folder + name+"_"+key+".eps")
        c.Print(folder + name+"_"+key+".pdf")

#default
infile_dict={}

###
# all variants of the charged protection included
###
infile_dict_chargedProtection={}
folder_CP = "JER_fit_16_newPR/ChargedPRotection/"

###
# all variante of PUPPI CHS versions included
###
infile_dict_CHSVersion={}
folder_CHS = "JER_fit_16_newPR/PUPPI_CHSVersions/"

###
# all variants of the dzcut true/false included
###
infile_dict_dzcut={}
folder_dzcut = "JER_fit_16_newPR/PUPPI_dzcut/"




#### PUPPI v13ultimative (newNPP)
#infile_puppi_inc_2016_v13ultimative = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles/uhh2.AnalysisModuleRunner.MC.QCD_2016v2_v13ultimative.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI v13 beagle"]=infile_puppi_inc_2016_v13ultimative


### PUPPI v13ultimative new commit
infile_puppi_inc_2016_v13newcommit = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles2/uhh2.AnalysisModuleRunner.MC.QCD_2016v2_newcommit.root")
TH1.AddDirectory(0)
infile_dict_CHSVersion["PUPPI v13 beagle"]=infile_puppi_inc_2016_v13newcommit

### Original QCD file from 2016v2 in CMSSW102
infile_QCD_orig_2016_102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison/uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_effmis.root")
TH1.AddDirectory(0)
infile_dict["PUPPI default"]=infile_QCD_orig_2016_102
infile_dict_chargedProtection["PUPPI default"]=infile_QCD_orig_2016_102
infile_dict_CHSVersion["PUPPI default"]=infile_QCD_orig_2016_102
infile_dict_dzcut["PUPPI default"]=infile_QCD_orig_2016_102

### CHS original QCD file from 2016v2 in CMSSW102 
infile_QCD_CHS = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_CHS_koastas.root")
TH1.AddDirectory(0)
infile_dict["CHS"]=infile_QCD_CHS
infile_dict_chargedProtection["CHS"]=infile_QCD_CHS
infile_dict_CHSVersion["CHS"]=infile_QCD_CHS
infile_dict_dzcut["CHS"]=infile_QCD_CHS

### PUPPI where charged particle above 200 GeV get a weight of one but do not paticipate in the alpha calculation of neutrals
infile_puppi_inc_2016_chargedparticleprotection = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_effmis.root")
TH1.AddDirectory(0)
infile_dict_chargedProtection["PUPPI ch. prot."]=infile_puppi_inc_2016_chargedparticleprotection 

### PUPPI where charged particles with fromPV 1or2 above 200 GeV get a weight of one but do not paticipate in the alpha calculation of neutrals
infile_puppi_inc_2016_chargedparticleprotection_fromPV = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_fromPV_effmis.root")
TH1.AddDirectory(0)
infile_dict_chargedProtection["PUPPI ch. prot. fromPV"]=infile_puppi_inc_2016_chargedparticleprotection_fromPV 

### PUPPI where charged particles with fromPV 1or2 above 100 GeV get a weight of one but do not paticipate in the alpha calculation of neutrals  
infile_puppi_inc_2016_chargedparticleprotection_100GeV = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_100GeV_effmis.root")
TH1.AddDirectory(0)
infile_dict_chargedProtection["PUPPI ch. prot. 100 GeV"]=infile_puppi_inc_2016_chargedparticleprotection_100GeV

### PUPPI where charged particles with fromPV 1or2 above 50 GeV get a weight of one but do not paticipate in the alpha calculation of neutrals  
infile_puppi_inc_2016_chargedparticleprotection_50GeV = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_50GeV_effmis.root")
TH1.AddDirectory(0)
infile_dict_chargedProtection["PUPPI ch. prot. 50 GeV"]=infile_puppi_inc_2016_chargedparticleprotection_50GeV

### PUPPI where charged particles with fromPV 1or2 above 50 GeV get a weight of one but do not paticipate in the alpha calculation of neutrals and neutrals above 50 get a weight of 1
infile_puppi_inc_2016_chargedparticleprotection_50GeVneutral = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_50GeVneutral_effmis.root")
TH1.AddDirectory(0)
infile_dict_chargedProtection["PUPPI ch. neu. prot. 50 GeV"]=infile_puppi_inc_2016_chargedparticleprotection_50GeVneutral

### PUPPI where the charged particle protection of 100 is applied and the dzcut is disabled-> charged particles treated as neutrals
infile_puppi_inc_2016_chargedparticleprotection_dzcutfalse = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_dzcutfalse_effmis.root")
TH1.AddDirectory(0)
infile_dict_dzcut["PUPPI dzcutfalse"]=infile_puppi_inc_2016_chargedparticleprotection_dzcutfalse

#### PUPPI where the charged particles are kept in the same way as CHS -> also influences the enutrals since more charged are know participating in the alpha calculation
#infile_puppi_inc_2016_chargednoneutrals = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargednonneutrals_effmis.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI PUPPI all LV, no dzcut"]=infile_puppi_inc_2016_chargednoneutrals

#### PUPPI where the charged particles are kept in the same way as CHS startig from a particle pT of 20 GeV
#infile_puppi_inc_2016_CHShighpT = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016v2_CHShighpT_effmis.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI PUPPI low pT neutral, high pT LV, no dz cut"]=infile_puppi_inc_2016_CHShighpT

#### PUPPI where the charged particles are kept in the same way as CHS startig from a particle pT of 20 GeV with dzcut enabled
##infile_puppi_inc_2016_CHShighpT_dzcutenabled = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016v2_CHShighpT_dzcutenabled_effmis.root")
#infile_puppi_inc_2016_CHShighpT_dzcutenabled = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016v2_CHShighpT_dzcutenabled_noCPP_effmis.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI low pT with dz cut (LV/PU), high pT LV, no dz cut"]=infile_puppi_inc_2016_CHShighpT_dzcutenabled
#infile_dict_dzcut["PUPPI low pT with dz cut (LV/PU), high pT LV, no dz cut"]=infile_puppi_inc_2016_CHShighpT_dzcutenabled

#### PUPPI v13
#infile_puppi_inc_2016_v13 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles/uhh2.AnalysisModuleRunner.MC.QCD_2016v2_v13.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI v13 beagle"]=infile_puppi_inc_2016_v13


### PUPPI v13v2
#infile_puppi_inc_2016_v13v2 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles/uhh2.AnalysisModuleRunner.MC.QCD_2016v2_v13v2.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI v13v2 beagle"]=infile_puppi_inc_2016_v13v2 


#### PUPPI all1
#infile_puppi_inc_2016_all1 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles/uhh2.AnalysisModuleRunner.MC.QCD_2016v2_all1.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI all1"]=infile_puppi_inc_2016_all1

#### PUPPI highpt no CPP
#infile_puppi_inc_2016_CHShighpT_noCPP = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison/uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016v2_CHShighpT_dzcutenabled_noCPP_effmis.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI low pT neutral, high pT LV, no dz cut, no CPP"]=infile_puppi_inc_2016_CHShighpT_noCPP



# ###
# # Charged particle protection
# ###
# # QCD CHS vs PUPPI
# hists_CP = get_hists(infile_dict_chargedProtection,"puppi_jet_pt_8_wJEC")
# #plot_control(hists_CP,folder_CP)

# reso_hists_CP = get_reso(hists_CP)
# plot_reso(reso_hists_CP,folder_CP,"reso","resolution",0.05,0.4,True)


###
# CHS versions
###
# QCD CHS vs PUPPI
hists_CHS = get_hists(infile_dict_CHSVersion,"puppi_jet_pt_8_wJEC")

reso_hists_CHS = get_reso(hists_CHS,folder_CHS)
plot_reso(reso_hists_CHS,folder_CHS,"reso","resolution",0.05,0.4,True)
plot_control(hists_CHS,folder_CHS)


# ###
# # dzcut
# ###
# # QCD CHS vs PUPPI
# hists_dzcut = get_hists(infile_dict_dzcut,"puppi_jet_pt_8_wJEC")

# reso_hists_dzcut = get_reso(hists_dzcut)
# plot_reso(reso_hists_dzcut,folder_dzcut,"reso","resolution",0.05,0.4,True)
