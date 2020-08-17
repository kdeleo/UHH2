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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlue, kBlack, kFALSE, kTRUE,kSienna,TLatex, Double, TMultiGraph, TString, TF1, kMagenta
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
    #if infile_dict.has_key("PUPPI default"):
    #    lista = infile_dict["PUPPI default"].Get(hist_folder).GetListOfKeys()
    if infile_dict.has_key("PUPPI chihuahua"):
        lista = infile_dict["PUPPI chihuahua"].Get(hist_folder).GetListOfKeys()
    else:
        lista = infile_dict["PUPPI 2017 v6"].Get(hist_folder).GetListOfKeys()

    for infile  in infile_dict:
        print "infile"
        print infile
        for el in lista:
            if "pt" in el.GetName(): continue
            if "Eta0to1p3" in el.GetName(): continue
            if "Eta1p3to1p6" in el.GetName(): continue
            if "Eta1p6to2" in el.GetName(): continue
            if "Eta2to2p5" in el.GetName(): continue
            #if "Eta2p5to3" in el.GetName(): continue
            if "Eta3to10" in el.GetName(): continue
            if "Eta3to3" in el.GetName(): continue
            #if "_PU0to10" in el.GetName(): continue
            #if "_PU10to20" in el.GetName(): continue
            #if "_PU30to40" in el.GetName(): continue
            #if "_PU40to50" in el.GetName(): continue
            #if "_PU50to70" in el.GetName(): continue
            if "JetPtResponse_" in el.GetName():
               #if "_PU0to10" in el.GetName(): 
               #if "_PU10to20" in el.GetName(): 
               #if "_PU30to40" in el.GetName(): 
               #if "_PU40to50" in el.GetName(): 
               #if "_PU50to70" in el.GetName(): 
                print "el GetName()"
                print el.GetName()

                # read in histogram JetPtResponse_*
                hist = infile_dict[infile].Get(hist_folder+"/"+el.GetName())
                if not hists.has_key(el.GetName()): hists[el.GetName()]={}
                hists[el.GetName()][infile] = hist

    return hists

# plots the different response histograms for each 10th bin
def plot_control(hists, folder):
    markers = [34,21,20,22,34,34,28,20,20,20,20,20,20,20]
    colors = [kOrange-3,kAzure-4,kBlack,kRed,kOrange-3,kRed,kBlack,kAzure-4, kBlack, kOrange-3,kAzure,kSpring,kGreen, kBlue,kBlue,kBlue,kBlue,kBlue]
    print "plot control"

    hist_dict = {}

    for key in hists:
        for pu in hists[key]:
            hist = hists[key][pu]
           
            for bin in range(0,hist.GetNbinsX()+1):
#                if bin%100 != 0: continue
                #if bin != 500: continue
                if bin%10 !=0: continue
                if bin>100: continue
                #if bin!=40: continue

                # plot for each pT bin the distribution and save in folder+"/control/"
                projection = hist.ProjectionY("_y",bin,bin+1)
                c = TCanvas()


                projection.GetXaxis().SetTitle("p_{T}^{reco} / p_{T}^{gen}")
                projection.GetYaxis().SetTitle("Events")
                #projection.GetXaxis().SetRangeUser(0.5,1.5)
                projection.GetXaxis().SetRangeUser(0.0,3.0)

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
def get_reso(hists):
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
                #if bin != 40 : continue
                if bin%10 : continue
                if bin>100 and bin < 200 and bin%30: continue
                if bin>200 and bin < 500 and bin%50: continue
                if bin>450 and bin%200: continue

                projection = hist.ProjectionY("_y",bin,bin+1)
                #projection.GetXaxis().SetRangeUser(0.5,1.5)
                #projection.GetXaxis().SetRangeUser(-10.0,10.0)
                projection.GetXaxis().SetRangeUser(0.0,3.0)
                mean = projection.GetMean()
                rms = projection.GetRMS()

                resolution = 0
                if rms !=0:
                    resolution = rms/mean

                #print key
                #print "mean = %.3f, rms = %.3f, resolution = %.3f " % (mean, rms, resolution)


                # Gaussian fit of the peaks
                gaussian_fit = TF1("gaussian_fit", "gaus", 0.0, 3.0);
                #gaussian_fit = TF1("gaussian_fit", "gaus", 0.8, 1.5);
                gaussian_fit.SetParameter(1, 1.0);
                gaussian_fit.SetParameter(2, 0.1);
                projection.Fit(gaussian_fit, "R"); 


                for i in range(0,2):
                    lower_bound = gaussian_fit.GetParameter(1) -1.5*gaussian_fit.GetParameter(2);
                    higher_bound = gaussian_fit.GetParameter(1)+1.5*gaussian_fit.GetParameter(2);
                    
                    gaussian_fit = TF1("gaussian_fit", "gaus",lower_bound,higher_bound);
                    gaussian_fit.SetParameter(1, gaussian_fit.GetParameter(1));
                    gaussian_fit.SetParameter(2, gaussian_fit.GetParameter(2));
                    projection.Fit(gaussian_fit,"R");

                resolution_2 = 0
                if gaussian_fit.GetParameter(2) !=0:
                    resolution_2 = gaussian_fit.GetParameter(2)/gaussian_fit.GetParameter(1)
 
                #print "mean gaussian = %.3f, rms gaussian = %.3f, resolution gaussian = %.3f " % (gaussian_fit.GetParameter(1), gaussian_fit.GetParameter(2), resolution_2) 


                #reso.SetBinContent(bin,resolution)
                #reso.SetBinError(bin,projection.GetRMSError())
                reso.SetBinContent(bin,resolution_2)
                reso.SetBinError(bin,gaussian_fit.GetParError(2))
                #mean_h.SetBinContent(bin,mean)
                #mean_h.SetBinError(bin,projection.GetRMSError())
                mean_h.SetBinContent(bin,gaussian_fit.GetParameter(1))
                mean_h.SetBinError(bin,gaussian_fit.GetParError(1))
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
    markers = [20,22,21,34,28,20,20,20,20,20,20]
    colors = [kBlack,kRed,kAzure-4,kOrange-3,kMagenta-3,kSpring,kGreen, kBlue,kBlue,kBlue,kBlue,kBlue] 

    for key in reso_hists: 
        print "key"
        print key
        c = TCanvas()
        leg = TLegend(0.5,0.5,0.9,0.85, "","brNDC")
        leg.SetBorderSize(0);	
        leg.SetFillStyle(0);
        leg.SetTextSize(0.035);

       
        i=0
        jj=0
        for key2 in reso_hists: 
         print "key2"
         print key2
         if not "_PU" in key2 : continue 
         for pu in reso_hists[key2]:
          #if i == 2 or i==4: i+=1
          reso_hists[key2][pu][reso_mean_rms].SetLineColor(colors[i])
          reso_hists[key2][pu][reso_mean_rms].SetMarkerColor(colors[i])
          reso_hists[key2][pu][reso_mean_rms].SetMarkerStyle(markers[jj])
          reso_hists[key2][pu][reso_mean_rms].SetMarkerSize(1.2)
          reso_hists[key2][pu][reso_mean_rms].GetXaxis().SetTitle("p_{T}^{gen}")
          reso_hists[key2][pu][reso_mean_rms].GetYaxis().SetTitle(name)
          if "resolution" in name:
              reso_hists[key2][pu][reso_mean_rms].GetYaxis().SetTitle("#sigma/mean")

          if "mean" in name:
              reso_hists[key2][pu][reso_mean_rms].GetYaxis().SetTitle("response (mean)")

          if "Pt" in key2:
              reso_hists[key2][pu][reso_mean_rms].GetXaxis().SetRangeUser(10,10000)
          reso_hists[key2][pu][reso_mean_rms].GetYaxis().SetRangeUser(ymin,ymax)
          reso_hists[key2][pu][reso_mean_rms].Draw("PE same")
          #leg.AddEntry(reso_hists[key2][pu][reso_mean_rms],pu,"lpe")
          legname = TString(key2)
          legname.ReplaceAll("JetPtResponse_","")
          legname.ReplaceAll("Eta0to1p3_","")
          legname.ReplaceAll("Eta1p3to1p63_","")
          legname.ReplaceAll("Eta1p6to2_","")
          legname.ReplaceAll("Eta2to2p5_","")
          legname.ReplaceAll("Eta2p5to3_","")
          legname.ReplaceAll("Eta3to10_","")
          legname = str(legname)
          leg.AddEntry(reso_hists[key2][pu][reso_mean_rms],legname,"lpe")
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
        
        if "central" in key2: 
            eta = CMSPlotStyle.draw_info("|#eta| < 2.5",0.92,0.87)
            eta.Draw()
            
        if "1p3to1p6" in key2:
            eta = CMSPlotStyle.draw_info("1.3 < |#eta| < 1.6",0.92,0.87)
            eta.Draw()

        if "1p6to2" in key2:
            eta = CMSPlotStyle.draw_info("1.6 < |#eta| < 2",0.92,0.87)
            eta.Draw()

        if "2to2p5" in key2:
            eta = CMSPlotStyle.draw_info("2 < |#eta| < 2.5",0.92,0.87)
            eta.Draw()

        if "2p5to3" in key2:
            eta = CMSPlotStyle.draw_info("2.5 < |#eta| < 3.0",0.92,0.87)
            eta.Draw()

        if "0to1p3" in key2:
            eta = CMSPlotStyle.draw_info(" |#eta| < 1.3",0.92,0.87)
            eta.Draw()

        if "3to10" in key2:
            eta = CMSPlotStyle.draw_info("3.0 < |#eta| < 5.0",0.92,0.87)
            eta.Draw()

        if "Pt" in key2:
            c.SetLogx()

        if blogy:
            c.SetLogy()
            
        c.Print(folder + name+"_"+key2+".eps")
        #c.Print(folder + name+"_"+key+".pdf")




#default
infile_dict={}


###
# all variante of PUPPI CHS versions included
###
infile_dict_CHSVersion={}
folder_CHS = "Laurent_JER_UL17_v14/"


#### PUPPI v14  
infile_puppi_inc_2017UL_v14 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesUL_JECsV4/uhh2.AnalysisModuleRunner.MC.QCD_2017UL_v14.root")
TH1.AddDirectory(0)
infile_dict_CHSVersion["PUPPI chihuahua"]=infile_puppi_inc_2017UL_v14

#### PUPPI v14 - Laurent tune 
#infile_puppi_inc_2017UL_v14_Laurent = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesUL_JECsV4/uhh2.AnalysisModuleRunner.MC.QCD_2017UL_v14_Laurent.root")
#TH1.AddDirectory(0)
#infile_dict_CHSVersion["PUPPI Laurent tune"]=infile_puppi_inc_2017UL_v14_Laurent 


#### Original QCD file from 2017 in CMSSW106
#infile_QCD_orig_2017_106 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesUL_JECsV4/uhh2.AnalysisModuleRunner.MC.QCD_2017UL_puppi.root")
#TH1.AddDirectory(0)
#infile_dict["PUPPI default"]=infile_QCD_orig_2017_106
#infile_dict_CHSVersion["PUPPI default"]=infile_QCD_orig_2017_106

##### CHS original QCD file from 2017 in CMSSW106 
#infile_QCD_CHS = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesUL_JECsV4/uhh2.AnalysisModuleRunner.MC.QCD_2017UL_chs.root")
#TH1.AddDirectory(0)
#infile_dict["CHS"]=infile_QCD_CHS
#infile_dict_CHSVersion["CHS"]=infile_QCD_CHS





###
# CHS versions
###
# QCD CHS vs PUPPI
hists_CHS = get_hists(infile_dict_CHSVersion,"puppi_jet_pt_8_wJEC")
#hists_CHS = get_hists(infile_dict_CHSVersion,"puppi_jet_pt_8")

reso_hists_CHS = get_reso(hists_CHS)
#plot_reso(reso_hists_CHS,folder_CHS,"reso","resolution",0.05,0.4,True)
plot_reso(reso_hists_CHS,folder_CHS,"mean","mean",0.0,2.0,False)
plot_control(hists_CHS,folder_CHS)


