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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlue, kBlack, kFALSE, kTRUE,kSienna,TLatex, kMagenta
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

#colors = [kBlue,kRed,kBlack,kGreen,kOrange]
colors = [kOrange-3,kRed,kAzure-4,kBlack, kRed, kMagenta,kAzure,kSpring,kGreen, kBlue,kBlue,kBlue,kBlue,kBlue]
markers = [34,22,21,20,20,24,20,22,21,34,24,20,20,20,20,20,20]

berror = False
folder = "koasta_Laurent/MET/"
beps = True

def calculate_ratio(infile,key):
    hist_folder = infile.Get("DY_output")
    hist = hist_folder.Get("upar_ZPT")
    ratio_histo  = TH1F("MET_ratio","ratio of the recoil of the Z Boson",500,0,500)

    # for each bin in response hist, caculate Mean of the projection and fill it in new histogram
    for xbin in range(1,hist.GetNbinsX()+1):
        projection  = hist.ProjectionY("_y",xbin,xbin+1)
        #Rebin hist and divide by bin center
        projection.Rebin(4)
        projection.Scale(1/4)

        mean = projection.GetMean()
        error = projection.GetMeanError()
        ratio_histo.SetBinContent(xbin,(- mean/xbin)+1)
        ratio_histo.SetBinError(xbin,error/xbin)

    #Rebin hist and divide by bin center
    ratio_histo.Rebin(14)
    ratio_histo.Scale(1/14)

    hist_dict[key+"_ratio"]=ratio_histo

def plot_MET_ratio(file_dict,folder,filenamebase):
    #set up canvas
    c = TCanvas()
    leg=TLegend(0.6,0.2,0.8,0.4,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);

    i=0
    for key in file_dict:
        calculate_ratio(file_dict[key],key)
    
        ratio_histo = hist_dict[key+"_ratio"]
        leg.AddEntry(hist_dict[key+"_ratio"],key,"LP")
    #set up histo
        ratio_histo.GetXaxis().SetTitle("q_{T} [GeV]")
        ratio_histo.GetYaxis().SetTitle("-<u_{||}>/<q_{T}>")
        ratio_histo.GetYaxis().SetRangeUser(0,1.1)
        
        ratio_histo.SetLineColor(colors[i])
        ratio_histo.SetMarkerStyle(markers[i])
        #if i==4: i+=1
        #if i==2: i+=1
        ratio_histo.SetMarkerColor(colors[i])
        ratio_histo.SetLineColor(colors[i])
    
#        ratio_histo.Draw("P same")
        ratio_histo.Draw("E same")

        i+=1

    leg.Draw()
    #save hist
    if beps: c.Print(folder + filenamebase+"MET_response.eps")
    c.Print(folder + filenamebase+"MET_response.pdf")
    

#plot all MET distributions of the different files we want to compare
#def plot_MET()

#calculate par and per
def calcualte_par_per(infile, key,folder,filenamebase):

    hist_folder = infile.Get("DY_output")
    hist_par = hist_folder.Get("rms_NPV")
    hist_per = hist_folder.Get("rms_uper_NPV")


    histo_rms_par = TH1F("histo_rms_par", "parallel resolution of the recoil",100,0,100)
    histo_rms_per = TH1F("histo_rms_per", "perpendicular resolution of the recoil",100,0,100)

    #make projection and take the rms
    for xbin in range(1,hist_par.GetNbinsX()+1):
        projection_par = hist_par.ProjectionY("_y",xbin,xbin+1)
        projection_per = hist_per.ProjectionY("_y",xbin,xbin+1)
       
        if xbin>35 and xbin<50:
            c_slice = TCanvas()
            projection_par.Draw()
            if beps: c_slice.Print(folder+filenamebase+"_slice_"+str(xbin)+".eps")
            c_slice.Print(folder+filenamebase+"_slice_"+str(xbin)+".pdf")

        rms_par = projection_par.GetRMS()
        mean_par = abs(projection_par.GetMean())
        error_par = projection_par.GetRMSError()
        rms_per = projection_per.GetRMS()
        mean_per = abs(projection_per.GetMean())
        error_per = projection_per.GetRMSError()

        if mean_par == 0:
            histo_rms_par.SetBinContent(xbin,0)
            histo_rms_par.SetBinError(xbin,0)
        else:
            histo_rms_par.SetBinContent(xbin,rms_par/mean_par)
            histo_rms_par.SetBinError(xbin,error_par/mean_par)
        if mean_per==0:
            histo_rms_per.SetBinContent(xbin,0)
            histo_rms_per.SetBinError(xbin,0)
        else:
            histo_rms_per.SetBinContent(xbin,rms_per/mean_per)
            histo_rms_per.SetBinError(xbin,error_per/mean_per)


    if key not in hist_dict:
        hist_dict[key+"_par"] =histo_rms_par
        hist_dict[key+"_per"] =histo_rms_per


#plot per and par resolution of MET
def plot_MET_resolution(file_dict,folder,filenamebase,ymin=0.2,ymax=1.2,ymax2=1.2):

    for key in file_dict:
        calcualte_par_per(file_dict[key],key,folder,filenamebase)

    #setup canvas for parallel
    c = TCanvas()
    #setup canvas for perpendicular
    c_per = TCanvas()

    leg=TLegend(0.2,0.2,0.4,0.4,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);

    i=0
    for key in file_dict:
        leg.AddEntry(hist_dict[key+"_par"],key,"LP")

        c.cd()
    #setup histos
        hist_dict[key+"_par"].GetXaxis().SetTitle("Number of True Interaction")
        hist_dict[key+"_par"].GetYaxis().SetTitle("#sigma(u_{||}) / <u_{||}>")
        #hist_dict[key+"_par"].SetMarkerStyle(2)
        #if i==4: i+=1
        #if i==2: i+=1
        hist_dict[key+"_par"].SetMarkerSize(1)
        hist_dict[key+"_par"].SetMarkerStyle(markers[i])
        hist_dict[key+"_par"].SetMarkerColor(colors[i])
        hist_dict[key+"_par"].SetLineColor(colors[i])
        hist_dict[key+"_par"].GetYaxis().SetRangeUser(ymin,ymax)

        hist_dict[key+"_par"].Draw("E same")
 
        c_per.cd()
    #setup histos
        hist_dict[key+"_per"].GetXaxis().SetTitle("Number of True Interaction")
        hist_dict[key+"_per"].GetYaxis().SetTitle("#sigma(u_{#perp}) / <u_{#perp}>")
        hist_dict[key+"_per"].SetMarkerStyle(markers[i])
        hist_dict[key+"_per"].SetMarkerColor(colors[i])
        hist_dict[key+"_per"].SetLineColor(colors[i])
        hist_dict[key+"_per"].GetYaxis().SetRangeUser(ymin,ymax2)

        hist_dict[key+"_per"].Draw("E same")
 
        i+=1

    c.cd()
    leg.Draw("same")

    if beps: c.Print(folder+filenamebase+"response_par.eps")
    c.Print(folder+filenamebase+"response_par.pdf")


    c_per.cd()
    leg.Draw("same")
    if beps: c_per.Print(folder+filenamebase+"response_per.eps")
    c_per.Print(folder+filenamebase+"response_per.pdf")


def plot_MET(infiles,folder, name):
    c = TCanvas()
    leg=TLegend(0.7,0.7,0.9,0.9,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);

    hist_to_draw={}
    
    for key in infiles:
        hist = infiles[key].Get("DY_output/MET_h")
        hist.Rebin(4)
        hist_to_draw[key]=hist

    i=0
    firstkey = ''
    for key in hist_to_draw:
        if i==0: firstkey = key
        print key + "  "+str(i)
        #todo change in histodraw
        # todo: normalise hist?
        hist_to_draw[key].Rebin(4)
        hist_to_draw[key].GetXaxis().SetTitle("raw MET")
        hist_to_draw[key].GetYaxis().SetTitle("Events")
        hist_to_draw[key].GetYaxis().SetRangeUser(0.0001,2e6)
        hist_to_draw[key].SetMarkerStyle(markers[i])
#        if i==4: i+=1
#        if i==2: i+=1
        #hist_to_draw[key].SetMarkerColor(i+1)
        #hist_to_draw[key].SetLineColor(i+1)
        hist_to_draw[key].SetMarkerColor(colors[i])
        hist_to_draw[key].SetMarkerStyle(markers[i])
        hist_to_draw[key].SetLineColor(colors[i])
        
        leg.AddEntry(hist_to_draw[key], key,"LP")

        hist_to_draw[key].Scale(1/hist_to_draw[key].Integral())
        hist_to_draw[key].Draw("P same")
        i+=1

    leg.Draw()
    c.Print(folder+name+".eps")
    c.Print(folder+name+".pdf")


    c.SetLogy()
    c.Print(folder+name+"_log.eps")
    c.Print(folder+name+"_log.pdf")

    hist_to_draw[firstkey].GetXaxis().SetRangeUser(0,200)
    hist_to_draw[firstkey].GetYaxis().SetRangeUser(1,2e6)
    c.Print(folder+name+"focus_log.eps")
    c.Print(folder+name+"focus_log.pdf")
    
    #todo not all markers same color
    # todo Axis title verschieben

#main
hist_dict={}
file_dict={}

#original
#read in DY file
# infile = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.KoastasSetup_DY.root")
# TH1.AddDirectory(0)
# file_dict["original"] = infile
# plot_MET_ratio(file_dict,folder,"original_")
# plot_MET_resolution(file_dict,folder,"original_")

# #v3
# infile_v3 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V3_DY.root")
# TH1.AddDirectory(0)
# file_dict["v3"] = infile_v3
# plot_MET_ratio(file_dict,folder,"compare_")
# plot_MET_resolution(file_dict,folder,"compare_")

# #v4
# infile_v4 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V4_middle_DY.root")
# TH1.AddDirectory(0)
# file_dict["v4 middle"] = infile_v4

# #v5
# infile_v5 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V5_middle_DY.root")
# TH1.AddDirectory(0)
# file_dict["v5 middle"] = infile_v5

# #CHS
# infile = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.CHS_DY.root")
# TH1.AddDirectory(0)
# file_dict["CHS"] = infile

##v6 2017
#infile_v6 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2017/uhh2.AnalysisModuleRunner.MC.2017v2_MC_DYJetsToLL_M_50_inc_v6.root")
#TH1.AddDirectory(0)
#file_dict["PUPPI 2017 v6"] = infile_v6
#
##v4 2017
#infile_v4_2017 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2017/uhh2.AnalysisModuleRunner.MC.2017v2_MC_DYJetsToLL_M_50_inc_v4.root")
#TH1.AddDirectory(0)
#file_dict["PUPPI 2017 v4"] = infile_v4_2017
#
##PUPPI 2017
#infile_2017_orig = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2017/uhh2.AnalysisModuleRunner.MC.2017v2_MC_DYJetsToLL_M_50_inc_PUPPI_orig.root")
#TH1.AddDirectory(0)
#file_dict["PUPPI 2017 orig"] = infile_2017_orig

# PUPPI default UL 2017
infile_2017_UL = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_UL_JECorder/uhh2.AnalysisModuleRunner.MC.MC_DY_2017UL_default.root")
TH1.AddDirectory(0)
file_dict["PUPPI default"] = infile_2017_UL

# CHS UL 2017
infile_2017_chs_UL = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_UL_JECorder/uhh2.AnalysisModuleRunner.MC.MC_DY_2017UL_chs.root")
TH1.AddDirectory(0)
file_dict["CHS"] = infile_2017_chs_UL

# PUPPI v14 UL 2017
infile_2017_v14_UL = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_UL_JECorder/uhh2.AnalysisModuleRunner.MC.MC_DY_2017UL_v14.root")
TH1.AddDirectory(0)
file_dict["PUPPI v14"] = infile_2017_v14_UL

# PUPPI v14 Laurent UL 2017
infile_2017_v14_UL_Laurent = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_UL_JECorder/uhh2.AnalysisModuleRunner.MC.MC_DY_2017UL_v14_Laurent.root")
TH1.AddDirectory(0)
file_dict["PUPPI Laurent"] = infile_2017_v14_UL_Laurent

plot_MET_ratio(file_dict,folder,"compare_")
plot_MET_resolution(file_dict,folder,"compare_",0.2,3,3)
plot_MET(file_dict,folder,"MET")

