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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlue, kBlack, kFALSE, kTRUE,kSienna,TLatex, Double, TMultiGraph, kMagenta, kCyan, kViolet, kWhite
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

# colors = [kBlue,kRed,kBlack,kGreen,kOrange]
# markers = [20,25,28,32,47,20,20]

quark_gluon_colors = [kBlack, kRed, kBlue, kCyan,kBlue]
colors = [kAzure-4,  kRed,kBlack,kBlack,kBlue,kOrange, kCyan]
#colors = [kRed,   kGreen+2, kOrange+1,kBlack,kBlue]
marker_sel = [21,22,20,32,21,20,20,22,22]

berror = False
beps = True
blogy = False

folder = "koasta/"

def calculate_eff(infile,folder,name_all,name_trig,name='orig'):
    print name_all
    hist_all = infile.Get(folder+"/"+name_all)
    hist_trig = infile.Get(folder+"/"+name_trig)

    if "NPV" in el.GetName():
        bins=array('d',[0,10,20,30,40,50,60,70])
        hist_all = hist_all.Rebin(5,hist_all.GetTitle(),bins)
        print hist_all.Integral()
        hist_trig = hist_trig.Rebin(5,hist_trig.GetTitle(),bins)
        print hist_trig.Integral()

        for bin in range(1,hist_all.GetNbinsX()):
            if (hist_all.GetBinContent(bin) - hist_trig.GetBinContent(bin)) < 0:
                print str(bin)+"  all  "+str(hist_all.GetBinContent(bin))+ "  tri "+str(bin)+str(hist_trig.GetBinContent(bin))

    if "Eff" in el.GetName() and "Pt" in el.GetName():
        hist_all.Rebin(2)
        hist_trig.Rebin(2)

    g_efficiency = TGraphAsymmErrors(hist_trig,hist_all,"cl=0.683 b(1,1) mode")
    if (("NPV" in name_all and "to" in name_all and "alljet" in name_all)  or ("Pt" in name_all and "to" not in name_all)) and "to" not in name_all:
        if "Eff" in name_all:
            if not eff_purity_dict[name_all].has_key(name):
                eff_purity_dict[name_all][name]={}
            eff_purity_dict[name_all][name]['eff']=g_efficiency
        else:
            key_name = name_all.replace("Purity","Efficiency") 
            key_name = key_name.replace("Reco","Gen") 
            print "   ==============================================================  "+key_name +"  "+name
            eff_purity_dict[key_name][name]['purity']=g_efficiency
    return g_efficiency


#def plot(eff_list,leg_list,folder,filenamebase,nameX,nameY, miny=0, legxmin = 0.5,legymin=0.3,legxmax=0.8,legymax=0.5, maxy = 1.05, setmarker=False,markers=[20,25,28,32,21,47,20,20,20]):
def plot(eff_list,leg_list,folder,filenamebase,nameX,nameY, miny=0, legxmin = 0.5,legymin=0.3,legxmax=0.8,legymax=0.5, maxy = 1.05, setmarker=False,markers=marker_sel, textalign =31, all_pre = False):

    c = TCanvas()
    leg=TLegend(legxmin,legymin,legxmax,legymax,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(kWhite);
    leg.SetLineColor(1);
    leg.SetTextFont(42);

    hr = c.DrawFrame(0,miny,50,maxy);
    if "Pt" in filenamebase:     hr = c.DrawFrame(0,miny,250,maxy);
    if "Eta" in filenamebase:     hr = c.DrawFrame(-5,miny,5,maxy);
    c.GetFrame().SetFillColor(21);
    c.GetFrame().SetBorderSize(12);
    hr.GetYaxis().SetTitle(nameY)
    hr.GetXaxis().SetTitle(nameX)


    ii=0
    jj=0
    for i in range(0,len(eff_list)):
        eff_list[i].GetYaxis().SetTitle(nameY)
        eff_list[i].GetXaxis().SetTitle(nameX)
        if ii==2: ii+=1
        if ii==4: ii+=1
        if ii==8: ii=28
        if ii==29: ii=48
#        eff_list[i].SetMarkerColor(ii+1)
        if "quark" in folder:
            eff_list[i].SetMarkerColor(quark_gluon_colors[jj])
        else:
            eff_list[i].SetMarkerColor(colors[jj])
        if setmarker: 
            eff_list[i].SetMarkerStyle(markers[jj])
            eff_list[i].SetMarkerSize(1.5)
#        eff_list[i].SetLineColor(ii+1)
        if "quark" in folder:
            eff_list[i].SetLineColor(quark_gluon_colors[jj])
        else:
            eff_list[i].SetLineColor(colors[jj])



        leg.AddEntry(eff_list[i],leg_list[i],"LPE")
    
        eff_list[i].Draw("P same")
        ii+=1
        jj+=1


    leg.Draw()

    CMSPlotStyle.extratext = "Simulation"
    if "2p5to3" in filenamebase and not "v6" in folder:
        if all_pre:
            CMSPlotStyle.extratext = "#splitline{Simulation}{Preliminary}"
        else:
            CMSPlotStyle.extratext = "#splitline{Simulation}{Supplementary}"
    if "quark" in folder:
        if all_pre:
            CMSPlotStyle.extratext = "#splitline{Simulation}{Preliminary}"
        else:
            CMSPlotStyle.extratext = "#splitline{Simulation}{Supplementary}"



    text = CMSPlotStyle.draw_cmstext("left", True,1.5)
    text[0].Draw()
    text[1].Draw()
    lumi = CMSPlotStyle.draw_lumi(True)
    lumi.Draw()


    antitext = "Anti-k_{T}, R=0.4"
    eta = ""
    pt = CMSPlotStyle.draw_info("",0.9,0.81)

    if "central" in filenamebase: 
        eta = "|#eta| < 2.5"
        
    if "2p5to3" in filenamebase:
        eta ="2.5 < |#eta| < 3"

    if "3to5" in filenamebase:
        eta = "3 < |#eta| < 5"

    if "_ptcut20GeV" in folder:
        if "eff" in filenamebase:
            pt = CMSPlotStyle.draw_info("p_{T}^{gen} > 30 GeV, p_{T}^{reco} > 20 GeV",0.9,0.801)
        if "purity" in filenamebase:
            pt = CMSPlotStyle.draw_info("p_{T}^{gen} > 20 GeV, p_{T}^{reco} > 30 GeV",0.9,0.801)
    else:
        if "eff" in filenamebase:
            pt = CMSPlotStyle.draw_info("p_{T}^{gen} > 30 GeV, p_{T}^{reco} > 10 GeV",0.9,0.801)
        if "purity" in filenamebase:
            pt = CMSPlotStyle.draw_info("p_{T}^{gen} > 10 GeV, p_{T}^{reco} > 30 GeV",0.9,0.801)


    antitext =antitext +", "+eta
    anti = CMSPlotStyle.draw_info(antitext,0.9,0.865,textalign)        
    if textalign == 11 :     anti = CMSPlotStyle.draw_info(antitext,0.4,0.865,textalign)        
    anti.Draw()
    pt.Draw()
    if beps: c.Print(folder+filenamebase+".eps")
    c.Print(folder+filenamebase+".pdf")

    CMSPlotStyle.extratext = "#splitline{Simulation}{Preliminary}"
    text = CMSPlotStyle.draw_cmstext("left", True,1.5)
    text[0].Draw()
    text[1].Draw()
    if beps: c.Print(folder+filenamebase+"_pre.eps")
    c.Print(folder+filenamebase+"_pre.pdf")


def plot_control_variables(infile_dict,hist_names,folder):
    
    
    for name in hist_names:
        print name
        i=0
        c = TCanvas()
        leg=TLegend(0.7,0.7,0.9,0.9,"","brNDC")
        leg.SetBorderSize(0);
        leg.SetTextSize(0.035);
        leg.SetFillColor(kWhite);
        leg.SetLineColor(1);
        leg.SetTextFont(42);
        
        for inf in infile_dict:
            if i==2: i+=1
            if i==4: i+=1
            hist = infile_dict[inf].Get(name)
            leg.AddEntry(hist,inf,"L")
            hist.SetLineColor(colors[i+1])
            hist.SetMarkerColor(colors[i+1])
            
            hist.Draw("same")
            i+=1 

        leg.Draw("same")
        if beps: c.Print(folder+name+".eps")
        c.Print(folder+name+".pdf")
        
    
def plot_resolution(infile_dict,folder,filenamebase,histo_folder_name ="jet_pt_8" ):
    infile_hist_dict={}
    for key in infile_dict:
        print key
        hist_dict={}
        mean_rms_dict={}
        infile = infile_dict[key]
        histo_folder = infile.Get(histo_folder_name)
        print histo_folder_name
        lista = histo_folder.GetListOfKeys()
        for el in lista:
            if "Response" in el.GetName() and "vsmu" in el.GetName():
                histo = histo_folder.Get(el.GetName())
                hist_resolution = TH1F(el.GetName()+"_resolution","JER",histo.GetNbinsY(),0,histo.GetNbinsY())
                hist_resolution_max = TH1F(el.GetName()+"_resolution_max","JER MAX",histo.GetNbinsY(),0,histo.GetNbinsY())
                hist_mean = TH1F(el.GetName()+"_resolution_mean","JER mean",histo.GetNbinsY(),0,histo.GetNbinsY())
                hist_rms = TH1F(el.GetName()+"_resolution_rms","JER rms",histo.GetNbinsY(),0,histo.GetNbinsY())
            
                # #normalize hist for each y bin
                # for bin in range(0,histo.GetNbinsY()+1):
                #     integral = 0
                #     for xbin in range(0,histo.GetNbinsX()+1):
                #         integral+=histo.GetBinContent(xbin,bin)

                #     value = 0
                #     error = 0
                #     for xbin in range(0,histo.GetNbinsX()+1):
                #         if integral != 0:
                #             value = histo.GetBinContent(xbin,bin)/integral
                #             error = histo.GetBinError(xbin,bin)/integral
                #         histo.SetBinContent(xbin,bin,value)
                #         histo.SetBinError(xbin,bin,error)


                #get projection
                for ybin in range(0,histo.GetNbinsY()+1):
                    projection = histo.ProjectionX("_x",ybin,ybin+1)
                    #rebin projection
                    projection.Rebin(4)
                    # projection.Scale(1/4)

                    #save also projections
                    c_slice = TCanvas()
                    projection.Draw()
                    name = key.replace(" ","_")
                    c_slice.Print(folder+"slices/"+filenamebase+"_"+name+"_"+el.GetName()+"_slice_"+str(ybin)+".eps")


                    #fill into new histogram
                    if projection.GetMean() == 0:
                        value = 0
                    else:
                        value = abs(projection.GetRMS())/projection.GetMean()

                    if (projection.GetBinCenter(projection.GetMaximumBin())) == 0:
                        value_max = 0
                    else:
                        value_max = abs(projection.GetRMS())/projection.GetBinCenter(projection.GetMaximumBin())


                    hist_resolution.SetBinContent(ybin,value)
                    hist_resolution_max.SetBinContent(ybin,value_max)
                    hist_rms.SetBinContent(ybin,abs(projection.GetRMS()))
                    hist_mean.SetBinContent(ybin,projection.GetBinCenter(projection.GetMaximumBin()))
                
                hist_resolution.Rebin(2)
                hist_resolution.Scale(1/2)

                hist_resolution_max.Rebin(2)
                hist_resolution_max.Scale(1/2)

                hist_dict[el.GetName()] = hist_resolution
                mean_rms_dict[el.GetName()+"_max"] = hist_resolution_max
                mean_rms_dict[el.GetName()+"_mean"] = hist_mean
                mean_rms_dict[el.GetName()+"_rms"] = hist_rms
        infile_hist_dict[key]=hist_dict
        infile_hist_dict[key+"_meanrms"]=mean_rms_dict


    #save Canvas
    #loop over all hist typs

    for key in infile_hist_dict["PUPPI orig"]:
        print "plot resolution for key  "+key 
        c = TCanvas()
        leg = TLegend(0.3,0.1,0.6,0.4, "","brNDC")
        
        i=0
        for dicts in infile_hist_dict:
            if "meanrms" in dicts: continue
            print "and for sample  " +dicts
            hist = infile_hist_dict[dicts][key]

            if i==2: i+=1            
            if i==4: i+=1
            #set hist attributes
            hist.GetXaxis().SetTitle("True Interaction")
            hist.GetYaxis().SetTitle("JER")
            hist.SetLineColor(i+1)
            hist.GetYaxis().SetRangeUser(0,1)
            leg.AddEntry(hist,dicts,"lpe")

            hist.Draw("same")
            i+=1
        
        #add text about pt bin and eta bin
        ptbins = re.findall(r'\d+\.*\d+', key)
        ptbin = ptbins[0] + "<= p_{T}^{gen} <=" +ptbins[1]
        pt_text = TLatex(3.5, 24, ptbin);
        pt_text.SetNDC()
        pt_text.SetX(0.6)
        pt_text.SetY(0.86)
        pt_text.SetTextFont(52)
        pt_text.SetTextSize(0.035)
        pt_text.Draw()

        #add text about eta bin and eta bin
        etabin = ""
        if "3to" in key:
            etabin = "3<= |#eta| <=5"
        elif "to3" in key:
            etabin = "2.5< |#eta| <=3.0"
        else:
            etabin = "0<= |#eta| <=2.5"

        eta_text = TLatex(3.5, 24, etabin);
        eta_text.SetNDC()
        eta_text.SetX(0.6)
        eta_text.SetY(0.80)
        eta_text.SetTextFont(52)
        eta_text.SetTextSize(0.035)
        eta_text.Draw()
        

        leg.SetBorderSize(0);	
        leg.SetFillStyle(0);

        leg.Draw()
        
        if beps: c.Print(folder+filenamebase+key+"_resolution.eps")
        c.Print(folder+filenamebase+key+"_resolution.pdf")

        #### plot mean and rms
    for key in infile_hist_dict["PUPPI orig_meanrms"]:
        c = TCanvas()
        leg = TLegend(0.6,0.5,0.9,0.8, "","brNDC")
        ptbins = re.findall(r'\d+\.*\d+', key)
        ptbin = ptbins[0] + "<= p_{T}^{gen} <=" +ptbins[1]
        pt_text = TLatex(3.5, 24, ptbin);
        pt_text.SetNDC()
        pt_text.SetX(0.6)
        pt_text.SetY(0.86)
        pt_text.SetTextFont(52)
        pt_text.SetTextSize(0.035)
        
        etabin = ""
        if "3" in key:
            etabin = "3<= |#eta| <=5"
        else:
            etabin = "0<= |#eta| <=2.5"
            
        eta_text = TLatex(3.5, 24, etabin);
        eta_text.SetNDC()
        eta_text.SetX(0.6)
        eta_text.SetY(0.80)
        eta_text.SetTextFont(52)
        eta_text.SetTextSize(0.035)

        i=0
        print "key for mean  "+ key
        if "mean" in key: 
            for dicts in infile_hist_dict:
                if "meanrms" not in dicts: continue

                print "==============  mean hist"
                print dicts + "  and  "+ key
                hist = infile_hist_dict[dicts][key]
                hist.Rebin(2)
                hist.Scale(1/2)
                if i==2: i+=1
                if i==4: i+=1

                #set hist attributes
                hist.GetXaxis().SetTitle("True Interaction")
                hist.GetYaxis().SetTitle("Mean")
                hist.SetLineColor(i+1)
                hist.GetYaxis().SetRangeUser(0,3)

                leg.AddEntry(hist,dicts,"lpe")
                hist.Draw("same")
                i+=1
            pt_text.Draw()
            eta_text.Draw()

            leg.SetBorderSize(0);	
            leg.SetFillStyle(0);

            leg.Draw()
            c.Print(folder+filenamebase+key+"_mean.eps")

        if "max" in key: 
            i=0
            for dicts in infile_hist_dict:
                if "meanrms" not in dicts: continue

                print "==============  mean hist"
                print dicts + "  and  "+ key
                hist = infile_hist_dict[dicts][key]
                hist.Rebin(2)
                hist.Scale(1/2)

                if i==2: i+=1
                if i==4: i+=1
                #set hist attributes
                hist.GetXaxis().SetTitle("True Interaction")
                hist.GetYaxis().SetTitle("JER (max)")
                hist.SetLineColor(i+1)
                hist.GetYaxis().SetRangeUser(0,3)

                leg.AddEntry(hist,dicts,"lpe")
                hist.Draw("same")
                i+=1
            pt_text.Draw()
            eta_text.Draw()

            leg.SetBorderSize(0);	
            leg.SetFillStyle(0);

            leg.Draw()
            c.Print(folder+filenamebase+key+"_max.eps")

        if "rms" in key:
            i=0
            for dicts in infile_hist_dict:
                if "meanrms" not in dicts: continue

                print "==============  mean hist"
                print dicts + "  and  "+ key
                hist = infile_hist_dict[dicts][key]
                hist.Rebin(2)
                hist.Scale(1/2)

                if i==2: i+=1
                if i==4: i+=1
               
                #set hist attributes
                hist.GetXaxis().SetTitle("True Interaction")
                hist.GetYaxis().SetTitle("rms")
                hist.SetLineColor(i+1)
                hist.GetYaxis().SetRangeUser(0,3)

                leg.AddEntry(hist,dicts,"lpe")
                hist.Draw("same")
                i+=1
            pt_text.Draw()
            eta_text.Draw()

            leg.SetBorderSize(0);	
            leg.SetFillStyle(0);

            leg.Draw()
            c.Print(folder+filenamebase+key+"_rms.eps")

def plot_signal_over_bkg(eff_purity_dict, folder, filenamebase):
    xeff=Double(0)
    yeff=Double(0)
    xpurity=Double(0)
    ypurity=Double(0)
    for key in eff_purity_dict:
        c = TCanvas()
        leg = TLegend(0.6,0.6,0.9,0.9, "","brNDC")
        leg.SetBorderSize(0);	
        leg.SetFillStyle(0);

        # hier aendern
        hr = c.DrawFrame(0,0,70,1);
        c.GetFrame().SetFillColor(21);
        c.GetFrame().SetBorderSize(12);
        hr.GetYaxis().SetTitle("#frac{#epsilon}{1- purity}")
        hr.GetXaxis().SetTitle("NPV")
        mg = TMultiGraph()
        jj=0
        for name  in eff_purity_dict[key]:
            if "CHS" not in key:
                print key + "  " + name
                eff = eff_purity_dict[key][name]["eff"]
                sig_over_bkg_g = eff_purity_dict[key][name]["eff"].Clone()
                purity = eff_purity_dict[key][name]["purity"]

                for i in range(0,eff.GetN()):
                    eff.GetPoint(i,xeff,yeff)
                    purity.GetPoint(i,xpurity,ypurity)
                    if ypurity==1:
                        sig_over_bkg =0
                    else:
                        sig_over_bkg = yeff / (math.sqrt(abs(1-ypurity)))
                    sig_over_bkg_g.SetPoint(i,xeff,yeff)

                #setup sig_over_bkg_g
                if jj==2: jj+=1
                if jj==4: jj+=1
                sig_over_bkg_g.SetLineColor(jj+1)
                sig_over_bkg_g.GetYaxis().SetTitle("#frac{#epsilon}{1- purity}")
                sig_over_bkg_g.GetXaxis().SetTitle("NPV")
        
                leg.AddEntry(sig_over_bkg_g,name,"l")
#                sig_over_bkg_g.Draw("")
                mg.Add(sig_over_bkg_g)
                jj+=1
            

        mg.Draw("AL")
        mg.GetYaxis().SetTitle("#frac{#epsilon}{1- purity}")
        mg.GetXaxis().SetTitle("NPV")
        leg.Draw()

        print folder+filenamebase+key
        if beps: c.Print(folder+filenamebase+key+".eps")
        c.Print(folder+filenamebase+key+".pdf")


            
##########################   main

b_DY = True
b_comp_CHS_PUPPI_signal = False
b_puppituning = True
b_2016_vs_2017_vs_2018 = True
b_charged_particle_protection = True
b_pTcut_HTbinnend_inc = False
b_paper_plot = False
b_quarkgluon = False

b_QCD = False
eff_purity_dict={}

if b_DY:
    infile_dict={}
    hist_names={"DY_output/number_genjets","DY_output/number_recojets","DY_output/pT_genjets","DY_output/pT_recojets","DY_output/pT_eff_recojets","DY_output/pT_purity_genjets","DY_output/eta_genjets","DY_output/eta_recojets","DY_output/muon_invariant_mass","DY_output/JetEfficiency_GenJetNPV","DY_output/JetEfficiency_GenJetNPV_matchedjet"}

    ########   CHS normal samples
    infile_CHS = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.DY_inc_CHS_effpurity_withoutpTcut.root")
    TH1.AddDirectory(0)

    infile_CHS_PUIDTight = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_tight_withoutpTcut.root")
    TH1.AddDirectory(0)

    infile_CHS_PUIDLoose = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_loose_withoutpTcut.root")
    TH1.AddDirectory(0)

    infile_CHS_PUIDMedium = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_medium_withoutpTcut.root")
    TH1.AddDirectory(0)

    ########   CHS normal samples pTcut 20 GeV
#    infile_CHS_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.DY_inc_CHS_effpurity_ptcut20GeV.root")
    infile_CHS_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.DY_inc_CHS_effpurity_withmatching0p4.root")
    TH1.AddDirectory(0)

#    infile_CHS_PUIDTight_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_tight_ptcut20GeV.root")
    infile_CHS_PUIDTight_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_tight_quark_gluon.root")
    TH1.AddDirectory(0)

#    infile_CHS_PUIDLoose_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_loose_ptcut20GeV.root")
    infile_CHS_PUIDLoose_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_loose_withmatching0p4.root")
    TH1.AddDirectory(0)

#    infile_CHS_PUIDMedium_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_medium_ptcut20GeV.root")
    infile_CHS_PUIDMedium_pt20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.MC_DY_inc_CHS_effpurity_medium_withmatching0p4.root")
    TH1.AddDirectory(0)


    ############  PUPPI normal (2016/2017/2018) in 102
    #infile_puppi_2016_CMSSW102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2016/uhh2.AnalysisModuleRunner.MC.2016v3_MC_DYJetsToLL_M_50_inc_PUPPI_orig.root")
    infile_puppi_2016_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3_puppi.root")
    #infile_puppi_2016_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3.root")
    TH1.AddDirectory(0)
    infile_dict["PUPPI 2016 v2"]=infile_puppi_2016_CMSSW102

    #infile_puppi_2017_CMSSW102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2017/uhh2.AnalysisModuleRunner.MC.2017v2_MC_DYJetsToLL_M_50_inc_PUPPI_orig.root")
    #infile_puppi_2017_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY/uhh2.AnalysisModuleRunner.MC.MC_DY_2017v2.root")
    infile_puppi_2017_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2017v2_puppi.root")
    TH1.AddDirectory(0)
    infile_dict["PUPPI 2017"]=infile_puppi_2017_CMSSW102

    #infile_puppi_2018_CMSSW102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2018/uhh2.AnalysisModuleRunner.MC.2018v2_MC_DYJetsToLL_M_50_inc_PUPPI_orig.root")
    infile_puppi_2018_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2018_puppi.root")
    TH1.AddDirectory(0)
    infile_dict["PUPPI 2018"]=infile_puppi_2018_CMSSW102

    ############  CHS normal (2016/2017/2018) in 102
    #infile_chs_2016_CMSSW102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2016/uhh2.AnalysisModuleRunner.MC.2016v3_MC_DYJetsToLL_M_50_inc_CHS.root")
    infile_chs_2016_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3_chs.root")
    #infile_chs_2016_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3_chs.root")
    TH1.AddDirectory(0)
    infile_dict["CHS 2016 v3"]=infile_chs_2016_CMSSW102

    #infile_chs_2017_CMSSW102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2017/uhh2.AnalysisModuleRunner.MC.2017v2_MC_DYJetsToLL_M_50_inc_CHS.root")
    #infile_chs_2017_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY/uhh2.AnalysisModuleRunner.MC.MC_DY_2017v2_chs.root")
    infile_chs_2017_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2017v2_chs.root")
    TH1.AddDirectory(0)
    infile_dict["CHS 2017"]=infile_chs_2017_CMSSW102

    #infile_chs_2018_CMSSW102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2018/uhh2.AnalysisModuleRunner.MC.2018v2_MC_DYJetsToLL_M_50_inc_CHS.root")
    infile_chs_2018_CMSSW102 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2018_chs.root")
    TH1.AddDirectory(0)
    infile_dict["CHS 2018"]=infile_chs_2018_CMSSW102

    ############ puppi: charged particle protection
    infile_puppi_2016_QCDorig = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_effmis.root")
    TH1.AddDirectory(0)

    infile_puppi_2016_chprot_200GeV = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_effmis.root")
    TH1.AddDirectory(0)

    infile_puppi_2016_chprot_100GeV = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_100GeV_effmis.root")
    TH1.AddDirectory(0)

    infile_puppi_2016_chprot_50GeV = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_50GeV_effmis.root")
    TH1.AddDirectory(0)

    infile_puppi_2016_chprot_fromPV = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison//uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_chargedparticleprotection_fromPV_effmis.root")
    TH1.AddDirectory(0)


    ############ puppi v4: pT cut and inc
    infile_puppi_v4 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V4_DY.root")
    TH1.AddDirectory(0)

    infile_puppi_v4_middle = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V4_middle_DY.root")
    TH1.AddDirectory(0)

    infile_puppi_v4_inc_ptCut20 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V4_middle_MC_DY_inc_PUPPI_effpurity_pTcut20_withmatching0p4.root")
    TH1.AddDirectory(0)

    infile_puppi_v4_inc = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V4_middle_MC_DY_inc_PUPPI_effpurity.root")
    TH1.AddDirectory(0)

    infile_puppi_v4_2016tune = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3_v4.root")
    TH1.AddDirectory(0)

    infile_puppi_v13 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v2_v13.root")
    TH1.AddDirectory(0)


    ############ puppi v13 newPR 2016
    #infile_puppi_v13_newPR = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfiles2/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3_v13_PR.root")
    infile_puppi_v13_newPR_16 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3_v13.root")
    #infile_puppi_v13_newPR_16 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY/uhh2.AnalysisModuleRunner.MC.MC_DY_2016v3_v13_PR.root")
    TH1.AddDirectory(0)

    ############ puppi v13 newPR 2017
    #infile_puppi_v13_newPR_17 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY/uhh2.AnalysisModuleRunner.MC.MC_DY_2017v2_v13_PR.root")
    infile_puppi_v13_newPR_17 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2017v2_v13.root")
    TH1.AddDirectory(0)

    ############ puppi v13 newPR 2018
    #infile_puppi_v13_newPR_18 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_2/uhh2.AnalysisModuleRunner.MC.MC_DY_2018_v13.root")
    infile_puppi_v13_newPR_18 = TFile("/nfs/dust/cms/user/deleokse/analysis/PUPPI_tuning/rootfilesDY_3/uhh2.AnalysisModuleRunner.MC.MC_DY_2018_v13_PR.root")
    TH1.AddDirectory(0)


    ########## puppi v6 for Irene
    infile_puppi_v6_inc = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2017/uhh2.AnalysisModuleRunner.MC.2017v2_MC_DYJetsToLL_M_50_inc_v6.root")
    TH1.AddDirectory(0)

    infile_puppi_v4_inc_2017 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/DY_2017/uhh2.AnalysisModuleRunner.MC.2017v2_MC_DYJetsToLL_M_50_inc_v4.root")
    TH1.AddDirectory(0)



    ########## puppi v3, v5, v5 middle
    infile_puppi_v3 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V3_DY.root")
    TH1.AddDirectory(0)

    infile_puppi_v5 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V5_DY.root")
    TH1.AddDirectory(0)

    infile_puppi_v5_middle = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.V5_middle_DY.root")
    TH1.AddDirectory(0)

    ############ puppi signal v4 and v5
    infile_signal_v4 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.Signal_v4.root")
    TH1.AddDirectory(0)

    infile_signal_v5 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.Signal_v5.root")
    TH1.AddDirectory(0)



    hist_folder = "EffPurity_output"
    #lista = infile_puppi_v4_inc_ptCut20.Get(hist_folder).GetListOfKeys()
    lista = infile_puppi_v13_newPR_16.Get(hist_folder).GetListOfKeys()
    nameX="NaN"

    print "start with eff and purity for DY"
    for el in lista:
        if "Eta" in el.GetName(): nameX = "#eta"
        if "NPV" in el.GetName(): nameX = "Number of pileup interactions"
        if "Pt" in el.GetName(): nameX = "p_{T}"
        postfix = "_matchedjet"

        if "GenJetEta_genJetPt" in el.GetName(): continue

        if "Efficiency" in el.GetName() and "matched" not in el.GetName():
            if "to" not in el.GetName(): eff_purity_dict[el.GetName()]={}
            print el.GetName()
            
            ##### puppi normal: 2016, 2017, 2018
#            eff_puppi_orig_2016_CMSSW102 =calculate_eff(infile_puppi_2016_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_puppi_orig_2017_CMSSW102 =calculate_eff(infile_puppi_2017_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
            eff_puppi_orig_2018_CMSSW102 =calculate_eff(infile_puppi_2018_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)

#            ##### puppi tuning:  2017
#            eff_puppi_v6_2017 =calculate_eff(infile_puppi_v6_inc,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_puppi_v4_2017 =calculate_eff(infile_puppi_v4_inc_2017,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            ############ puppi: charged particle protection
#            eff_puppi_2016_chprot_orig = calculate_eff(infile_puppi_2016_QCDorig,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            eff_puppi_2016_chprot_200GeV = calculate_eff(infile_puppi_2016_chprot_200GeV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            eff_puppi_2016_chprot_100GeV = calculate_eff(infile_puppi_2016_chprot_100GeV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            eff_puppi_2016_chprot_50GeV = calculate_eff(infile_puppi_2016_chprot_50GeV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            eff_puppi_2016_chprot_fromPV = calculate_eff(infile_puppi_2016_chprot_fromPV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)

            ##### CHS normal: 2016, 2017, 2018
#            eff_chs_2016_CMSSW102 =calculate_eff(infile_chs_2016_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_chs_2017_CMSSW102 =calculate_eff(infile_chs_2017_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
            eff_chs_2018_CMSSW102 =calculate_eff(infile_chs_2018_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)


#            ##### quark gluon comp
#            eff_puppi_orig_2016_quark =calculate_eff(infile_puppi_v4_inc_ptCut20,"quark_EffPurity_output",el.GetName(),el.GetName()+postfix)
#            eff_puppi_orig_2016_gluon =calculate_eff(infile_puppi_v4_inc_ptCut20,"gluon_EffPurity_output",el.GetName(),el.GetName()+postfix)
#
#            ##### puppi signal: v4 and v5
#            eff_signal_v4 =calculate_eff(infile_signal_v4,"jet_pt_8",el.GetName(),el.GetName()+postfix)
#            eff_signal_v5 =calculate_eff(infile_signal_v5,"jet_pt_8",el.GetName(),el.GetName()+postfix)
#            eff_signal_v4_VBF =calculate_eff(infile_signal_v4,"jet_pt_8_VBF",el.GetName(),el.GetName()+postfix)
#            eff_signal_v5_VBF =calculate_eff(infile_signal_v5,"jet_pt_8_VBF",el.GetName(),el.GetName()+postfix)
#
#            ##### puppi v4, v5, middle
#            eff_v4 =calculate_eff(infile_puppi_v4,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_v4_2016tune =calculate_eff(infile_puppi_v4_2016tune,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_v13 =calculate_eff(infile_puppi_v13,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_v5_middle =calculate_eff(infile_puppi_v5_middle,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_v4_middle =calculate_eff(infile_puppi_v4_middle,hist_folder,el.GetName(),el.GetName()+postfix)


            ##### puppi v13 new PR 2016
#            eff_v13_newPR_16 =calculate_eff(infile_puppi_v13_newPR_16,hist_folder,el.GetName(),el.GetName()+postfix)

#            ##### puppi v13 new PR 2017
#            eff_v13_newPR_17 =calculate_eff(infile_puppi_v13_newPR_17,hist_folder,el.GetName(),el.GetName()+postfix)

            ##### puppi v13 new PR 2018
            eff_v13_newPR_18 =calculate_eff(infile_puppi_v13_newPR_18,hist_folder,el.GetName(),el.GetName()+postfix)


#            ##### puppi v4 middle: 20 GeV pT cut, inc sample
#            eff_v4_inc_pt20 =calculate_eff(infile_puppi_v4_inc_ptCut20,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_v4_inc =calculate_eff(infile_puppi_v4_inc,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            eff_puppi_v4_quark =calculate_eff(infile_puppi_v4_inc_ptCut20,"quark_EffPurity_output",el.GetName(),el.GetName()+postfix)
#            eff_puppi_v4_gluon =calculate_eff(infile_puppi_v4_inc_ptCut20,"gluon_EffPurity_output",el.GetName(),el.GetName()+postfix)
#
#
#            ##### CHS samples
#            eff_CHS =calculate_eff(infile_CHS,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_CHS_PUIDTight =calculate_eff(infile_CHS_PUIDTight,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_CHS_PUIDMedium =calculate_eff(infile_CHS_PUIDMedium,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_CHS_PUIDLoose =calculate_eff(infile_CHS_PUIDLoose,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            ##### CHS samples pt20GeV
#            eff_CHS_pt20 =calculate_eff(infile_CHS_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_CHS_PUIDTight_pt20 =calculate_eff(infile_CHS_PUIDTight_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_CHS_PUIDMedium_pt20 =calculate_eff(infile_CHS_PUIDMedium_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#            eff_CHS_PUIDLoose_pt20 =calculate_eff(infile_CHS_PUIDLoose_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            ##### CHS samples quark gluon for tight PU jet ID
#            eff_CHS_PUIDTight_pt20_quark =calculate_eff(infile_CHS_PUIDTight_pt20,"quark_EffPurity_output",el.GetName(),el.GetName()+postfix)
#            eff_CHS_PUIDTight_pt20_gluon =calculate_eff(infile_CHS_PUIDTight_pt20,"gluon_EffPurity_output",el.GetName(),el.GetName()+postfix)
#            ##############################################   Starting plots   #################################################
#            ##### comparision CHS, puppi orig, puppi v4 and signal v4, v5
#            if b_comp_CHS_PUPPI_signal:
#                hists =[eff_CHS,eff_puppi_orig_2016,eff_CHS_PUIDTight,eff_CHS_PUIDLoose,eff_v4,eff_signal_v4,eff_signal_v5] 
#                legnames = ["CHS","PUPPI","CHS PUID Tight","CHS PUID Loose","PUPPI v4","Signal v4","Signal v5"]
#
#                plot(hists,legnames,folder+"/signal_v4/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.2,0.2,0.5,0.5)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/signal_v4/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.6,0.2,0.9,0.5)
#
#            ##### PUPPI TUNING
#            if b_puppituning:
#                hists =[eff_puppi_orig_2017_CMSSW102,eff_puppi_v4_2017,eff_puppi_v6_2017] 
#                legnames = ["PUPPI orig.","PUPPI v4","PUPPI v6"] 
#                plot(hists,legnames,folder+"/v6_forIrene_ptcut20GeV/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.15,0.15,0.45,0.45,1.1,True)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/v6_forIrene_ptcut20GeV/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.6,0.2,0.9,0.5,1.1,True)
#
#
#            if b_charged_particle_protection:
#                hists = [eff_puppi_2016_chprot_orig, eff_puppi_2016_chprot_200GeV, eff_puppi_2016_chprot_100GeV, eff_puppi_2016_chprot_50GeV, eff_puppi_2016_chprot_fromPV]
#                legnames = ["PUPPI orig.","PUPPI ch. prot. 200 GeV","PUPPI ch. prot. 100 GeV","PUPPI ch. prot. 50 GeV","PUPPI ch. prot. fromPV"]
#                plot(hists,legnames,folder+"/chargProtection_ptcut8GeV/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.15,0.15,0.45,0.45,1.1,True)


            ##### 2016 vs 2017 vs 2018
            if b_2016_vs_2017_vs_2018:
                #hists =[eff_v4_inc_pt20,eff_puppi_orig_2016_CMSSW102,eff_v4_2016tune,eff_v13,eff_v13ultimative] 
                #legnames = ["PUPPI 2016 (JME-18-001)","PUPPI 2016v3","PUPPI 2016 tune","PUPPI 2016 v13", "PUPPI 2016 v13ultimative"]
                hists =[eff_puppi_orig_2018_CMSSW102,eff_chs_2018_CMSSW102,eff_v13_newPR_18] 
                legnames = ["PUPPI default","CHS","PUPPI v13 beagle"]

                ymax = 1.15

                plot(hists,legnames,folder+"/2016vs2017vs2018_ptcut20GeV/PUPPI/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.2,0.2,0.5,0.5,ymax, True, marker_sel,31,True)

#                hists =[eff_CHS_pt20,eff_chs_2016_CMSSW102,eff_chs_2017_CMSSW102,eff_chs_2018_CMSSW102] 
#                legnames = ["CHS 2016 (JME-18-001)","CHS 2016v3","CHS 2017","CHS 2018"]
#
#                plot(hists,legnames,folder+"/2016vs2017vs2018_ptcut20GeV//CHS/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.2,0.2,0.5,0.5, ymax, True, marker_sel,31,True)
#
#
#            ##  test PUPPI HT binned, inc and pT 20
#            if b_pTcut_HTbinnend_inc:
##                hists =[eff_v4_middle,eff_v4_pt20,eff_v4_inc_pt20,eff_v4_inc] 
#                hists =[eff_v4_inc_pt20,eff_v4_inc, eff_v4_middle,eff_v4_HTinc ] 
#                legnames = ["PUPPI v4 middle pT>20","PUPPI v4 middle inc","PUPPI v4 middle HTbinned","PUPPI v4 middle HT inc"]
#
#                plot(hists,legnames,folder+"/pTcut_inc_comp/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.2,0.2,0.5,0.5)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/pTcut_inc_comp/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.6,0.2,0.9,0.5)
#
#            ##### paper plot: CHS + 3 PU Jet ID + PUPPI
#            if b_paper_plot:
#                hists =[eff_CHS_pt20,eff_CHS_PUIDTight_pt20,eff_CHS_PUIDMedium_pt20,eff_CHS_PUIDLoose_pt20,eff_v4_inc_pt20] 
#                legnames = ["CHS","CHS + tight PU jet ID","CHS + medium PU jet ID","CHS + loose PU jet ID","PUPPI"]
#
#                plot(hists,legnames,folder+"/paper_ptcut20GeV/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.65, 0.16,0.15,0.46,0.45,1.1,True)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/paper_ptcut20GeV/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.6,0.2,0.9,1.1,True)
#                if "GenJetNPV" in el.GetName() and "2p5to3" in el.GetName():             plot(hists,legnames,folder+"/paper_ptcut20GeV/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.4, 0.16,0.15,0.46,0.45,1.2,True)
#                if "GenJetNPV" in el.GetName() and "3to5" in el.GetName():             plot(hists,legnames,folder+"/paper_ptcut20GeV/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0, 0.16,0.15,0.46,0.45,1.3,True)
#
#                hists =[eff_CHS,eff_CHS_PUIDTight,eff_CHS_PUIDMedium,eff_CHS_PUIDLoose,eff_v4_inc] 
#                legnames = ["CHS","CHS + tight PU jet ID","CHS + medium PU jet ID","CHS + loose PU jet ID","PUPPI"]
#
#                plot(hists,legnames,folder+"/paper/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.55, 0.2,0.15,0.5,0.45,1.1,True)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/paper/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.6,0.2,0.9,1.1,True)
#                if "GenJetNPV" in el.GetName() and "2p5to3" in el.GetName():             plot(hists,legnames,folder+"/paper/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.5, 0.2,0.15,0.5,0.45,1.1,True)
#                if "GenJetNPV" in el.GetName() and "3to5" in el.GetName():             plot(hists,legnames,folder+"/paper/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0, 0.2,0.15,0.5,0.45,1.3,True)
#
#            if b_quarkgluon:
#                hists = [eff_v4_inc_pt20, eff_puppi_v4_quark,eff_puppi_v4_gluon]
#                legnames = ["PUPPI","PUPPI Quarks","PUPPI Gluons"]
#                
#                plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.7, 0.2,0.15,0.5,0.45,1.1,True)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.6,0.2,0.9,1.1,True)
#                if "GenJetNPV" in el.GetName() and "2p5to3" in el.GetName():             plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.4, 0.2,0.15,0.5,0.45,1.2,True)
#                if "GenJetNPV" in el.GetName() and "3to5" in el.GetName():             plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0, 0.2,0.15,0.5,0.45,1.3,True)
#
#                hists = [eff_CHS_PUIDTight_pt20, eff_CHS_PUIDTight_pt20_quark, eff_CHS_PUIDTight_pt20_gluon]
#                legnames = ["CHS + tight PU jet ID","CHS + tight PU jet ID Quarks","CHS + tight PU jet ID Gluons"]
#                
#                plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon_CHStight/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.7, 0.2,0.15,0.5,0.45,1.1,True)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon_CHStight/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.6, 0.6,0.2,0.9,1.1,True)
#                if "GenJetNPV" in el.GetName() and "2p5to3" in el.GetName():             plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon_CHStight/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0.4, 0.2,0.15,0.5,0.45,1.2,True)
#                if "GenJetNPV" in el.GetName() and "3to5" in el.GetName():             plot(hists,legnames,folder+"/paper_ptcut20GeV_quarkgluon_CHStight/","eff_alljets_"+el.GetName(),nameX,"Efficiency",0, 0.2,0.15,0.5,0.45,1.3,True)

                
                

        ###################################     PURITY   #################################################################################
        if "Purity" in el.GetName() and "matched" not in el.GetName():
            ##### puppi orig: 2016 and 2017 and 2018
#            purity_puppi_orig_2016_CMSSW102 =calculate_eff(infile_puppi_2016_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_puppi_orig_2017_CMSSW102 =calculate_eff(infile_puppi_2017_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
            purity_puppi_orig_2018_CMSSW102 =calculate_eff(infile_puppi_2018_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)

#            ##### puppi tuning:  2017
#            purity_puppi_v6_2017 =calculate_eff(infile_puppi_v6_inc,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_puppi_v4_2017 =calculate_eff(infile_puppi_v4_inc_2017,hist_folder,el.GetName(),el.GetName()+postfix)

            ##### CHS normal: 2016, 2017, 2018
#            purity_chs_2016_CMSSW102 =calculate_eff(infile_chs_2016_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_chs_2017_CMSSW102 =calculate_eff(infile_chs_2017_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)
            purity_chs_2018_CMSSW102 =calculate_eff(infile_chs_2018_CMSSW102,hist_folder,el.GetName(),el.GetName()+postfix)

#            ##### puppi orig:  quark vs gluon
#            purity_kosta_quark =calculate_eff(infile_puppi_v4_inc_ptCut20,"quark_EffPurity_output",el.GetName(),el.GetName()+postfix)            
#            purity_kosta_gluon =calculate_eff(infile_puppi_v4_inc_ptCut20,"gluon_EffPurity_output",el.GetName(),el.GetName()+postfix)
#
#            ##### puppi signal: v4 and v5
#            purity_signal_v4 =calculate_eff(infile_signal_v4,"jet_pt_8",el.GetName(),el.GetName()+postfix)
#            purity_signal_v5 =calculate_eff(infile_signal_v5,"jet_pt_8",el.GetName(),el.GetName()+postfix)
#
#            purity_signal_v4_VBF =calculate_eff(infile_signal_v4,"jet_pt_8_VBF",el.GetName(),el.GetName()+postfix)
#            purity_signal_v5_VBF =calculate_eff(infile_signal_v5,"jet_pt_8_VBF",el.GetName(),el.GetName()+postfix)
#            purity_v4 =calculate_eff(infile_puppi_v4,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_v4_inc_pt20 =calculate_eff(infile_puppi_v4_inc_ptCut20,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            purity_v4_quark =calculate_eff(infile_puppi_v4_inc_ptCut20,"quark_EffPurity_output",el.GetName(),el.GetName()+postfix)            
#            purity_v4_gluon =calculate_eff(infile_puppi_v4_inc_ptCut20,"gluon_EffPurity_output",el.GetName(),el.GetName()+postfix)
#
#
#            purity_v4_inc =calculate_eff(infile_puppi_v4_inc,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            purity_v4_2016tune =calculate_eff(infile_puppi_v4_2016tune,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            purity_v13 =calculate_eff(infile_puppi_v13,hist_folder,el.GetName(),el.GetName()+postfix)




            ##### puppi v13 new PR 2016
#            purity_v13_newPR_16 =calculate_eff(infile_puppi_v13_newPR_16,hist_folder,el.GetName(),el.GetName()+postfix)

#            ##### puppi v13 new PR 2017
#            purity_v13_newPR_17 =calculate_eff(infile_puppi_v13_newPR_17,hist_folder,el.GetName(),el.GetName()+postfix)

#            ##### puppi v13 new PR 2018
            purity_v13_newPR_18 =calculate_eff(infile_puppi_v13_newPR_18,hist_folder,el.GetName(),el.GetName()+postfix)


#            ##### puppi v4 and v5
#            purity_v5_middle =calculate_eff(infile_puppi_v5_middle,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_v4_middle =calculate_eff(infile_puppi_v4_middle,hist_folder,el.GetName(),el.GetName()+postfix)
#
#
#            ##### CHS samples
#            purity_CHS =calculate_eff(infile_CHS,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_CHS_PUIDTight =calculate_eff(infile_CHS_PUIDTight,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_CHS_PUIDMedium =calculate_eff(infile_CHS_PUIDMedium,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_CHS_PUIDLoose =calculate_eff(infile_CHS_PUIDLoose,hist_folder,el.GetName(),el.GetName()+postfix)
#
#            ##### CHS samples
#            purity_CHS_pt20 =calculate_eff(infile_CHS_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_CHS_PUIDTight_pt20 =calculate_eff(infile_CHS_PUIDTight_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_CHS_PUIDMedium_pt20 =calculate_eff(infile_CHS_PUIDMedium_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#            purity_CHS_PUIDLoose_pt20 =calculate_eff(infile_CHS_PUIDLoose_pt20,hist_folder,el.GetName(),el.GetName()+postfix)
#
#
#            ############ puppi: charged particle protection
#            purity_puppi_2016_chprot_orig = calculate_eff(infile_puppi_2016_QCDorig,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            purity_puppi_2016_chprot_200GeV = calculate_eff(infile_puppi_2016_chprot_200GeV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            purity_puppi_2016_chprot_100GeV = calculate_eff(infile_puppi_2016_chprot_100GeV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            purity_puppi_2016_chprot_50GeV = calculate_eff(infile_puppi_2016_chprot_50GeV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)
#            purity_puppi_2016_chprot_fromPV = calculate_eff(infile_puppi_2016_chprot_fromPV,"jet_pt_8/",el.GetName(),el.GetName()+postfix)


#            ##############################################   Starting plots   #################################################
#            ##### puppi tuning comparison
#            if b_comp_CHS_PUPPI_signal:
#                purities = [purity_CHS,purity_kosta,purity_CHS_PUIDTight,purity_CHS_PUIDLoose,purity_v4,purity_signal_v4,purity_signal_v5] 
#                names = ["CHS","PUPPI","CHS PUID Tight","CHS PUID Loose","PUPPI v4","Signal v4","Signal v5"]
#
#                plot(purities,names,folder+"/signal_v4/","purity_alljets_"+el.GetName(),nameX,"Purity",0.4, 0.2,0.2,0.5,0.5)
#                if "NPV" in el.GetName() and "forward" in el.GetName(): plot(purities,names,folder+"/signal_v4/","purity_alljets_"+el.GetName(),nameX,"Purity",0.7, 0.2,0.15,0.5,0.4)
#                if "Eta" in el.GetName() : plot(purities,names,folder+"/signal_v4/","purity_alljets_"+el.GetName(),nameX,"Purity",0.7, 0.35,0.15,0.65,0.4)
#
#            ##### PUPPI TUNING
#            if b_puppituning:
#                hists =[purity_puppi_orig_2017_CMSSW102,purity_puppi_v4_2017,purity_puppi_v6_2017] 
#                legnames = ["PUPPI orig.","PUPPI v4","PUPPI v6"] 
#                plot(hists,legnames,folder+"/v6_forIrene_ptcut20GeV/","purity_alljets_"+el.GetName(),nameX,"Purity",0, 0.15,0.15,0.45,0.45,1.1,True)
#                if "GenJetPt" in el.GetName(): plot(hists,legnames,folder+"/v6_forIrene_ptcut20GeV/","purity_alljets_"+el.GetName(),nameX,"Purity",0.6, 0.6,0.2,0.9,0.5)
#
#
#            if b_charged_particle_protection:
#                hists = [ purity_puppi_2016_chprot_orig,purity_puppi_2016_chprot_200GeV, purity_puppi_2016_chprot_100GeV, purity_puppi_2016_chprot_50GeV, purity_puppi_2016_chprot_fromPV]
#                legnames = ["PUPPI orig.","PUPPI ch. prot. 200 GeV","PUPPI ch. prot. 100 GeV","PUPPI ch. prot. 50 GeV","PUPPI ch. prot. fromPV"]
#                plot(hists,legnames,folder+"/chargProtection_ptcut8GeV/","purity_alljets_"+el.GetName(),nameX,"Purity",0, 0.15,0.15,0.45,0.45,1.1,True)

            ##### comparison 2016 vs 2017
            if b_2016_vs_2017_vs_2018:
                #hists =[purity_v4_inc_pt20,purity_puppi_orig_2016_CMSSW102,purity_v4_2016tune,purity_v13,purity_v13ultimative] 
                #legnames = ["PUPPI 2016 (JME-18-001)","PUPPI 2016v3","PUPPI 2016 tune","PUPPI 2016 v13","PUPPI 2016 v13ultimative"]
                hists =[purity_puppi_orig_2018_CMSSW102,purity_chs_2018_CMSSW102,purity_v13_newPR_18] 
                legnames = ["PUPPI default","CHS","PUPPI v13 beagle"]

                ymax = 1.1
                ymin = 0.8
                if "2p5" in el.GetName() or "3to5" in el.GetName():
                    ymax = 1.1
                    ymin = 0

                plot(hists,legnames,folder+"/2016vs2017vs2018_ptcut20GeV/PUPPI/","purity_alljets_"+el.GetName(),nameX,"Purity",ymin, 0.15,0.15,0.5,0.5,ymax, True, marker_sel,31,True)

#                hists =[purity_CHS_pt20,purity_chs_2016_CMSSW102,purity_chs_2017_CMSSW102,purity_chs_2018_CMSSW102] 
#                legnames = ["CHS 2016 (JME-18-001)","CHS 2016v3","CHS 2017","CHS 2018"]
#
#                plot(hists,legnames,folder+"/2016vs2017vs2018_ptcut20GeV/CHS/","purity_alljets_"+el.GetName(),nameX,"Purity",ymin, 0.15,0.15,0.5,0.5,ymax, True, marker_sel,31,True)
#
#
#
#            if b_pTcut_HTbinnend_inc:
#                purities = [purity_v4_inc_pt20,purity_v4_inc, purity_v4_middle,purity_v4_HTinc ] 
#                names = ["PUPPI v4 middle pT>20","PUPPI v4 middle inc ","PUPPI v4 middle HTbinned","PUPPI v4 middle HT inc"]
#
#                plot(purities,names,folder+"/pTcut_inc_comp/","purity_alljets_"+el.GetName(),nameX,"Purity",0.4, 0.2,0.2,0.5,0.5)
#                if "NPV" in el.GetName() and "forward" in el.GetName(): plot(purities,names,folder+"/pTcut_inc_comp/","purity_alljets_"+el.GetName(),nameX,"Purity",0.7, 0.2,0.15,0.5,0.4)
#                if "Eta" in el.GetName() : plot(purities,names,folder+"/pTcut_inc_comp/","purity_alljets_"+el.GetName(),nameX,"Purity",0.7, 0.35,0.15,0.65,0.4)
#
#
#            ##### PAPER PLOTS
#            if b_paper_plot:
#                purities = [purity_CHS_pt20,purity_CHS_PUIDTight_pt20,purity_CHS_PUIDMedium_pt20,purity_CHS_PUIDLoose_pt20,purity_v4_inc_pt20] 
#                names = ["CHS","CHS + tight PU jet ID","CHS + medium PU jet ID","CHS + loose PU jet ID","PUPPI"]
#
#                ymin = 0.4
#                ymax = 1.1
#                legxmin = 0.16
#                legxmax = 0.46
#                legymin = 0.15
#                legymax = 0.45
#                textalign =31
#                if "NPV" in el.GetName() and "central" in el.GetName():
#                    ymin = 0.5
#                    ymax = 1.2
#                if "NPV" in el.GetName() and "2p5to3" in el.GetName():
#                    ymin = 0
#                    # ymax = 1.6
#                    # legxmin = 0.45
#                    # legxmax = 0.75
#                    # legymin = 0.5
#                    # legymax = 0.8
#                    # textalign = 11
#                    ymax = 1.1
#                    legxmin = 0.16
#                    legxmax = 0.46
#                    legymin = 0.15
#                    legymax = 0.45
#
#
#
#                if "NPV" in el.GetName() and "3to5" in el.GetName():
#                    ymin = 0
#                    # ymax = 1.2
#                    # legxmin = 0.35
#                    # legxmax = 0.65
#                    # legymin = 0.5
#                    # legymax = 0.8
#                    ymax = 1.
#                    legxmin = 0.16
#                    legxmax = 0.46
#                    legymin = 0.15
#                    legymax = 0.45
#
#                plot(purities,names,folder+"/paper_ptcut20GeV/","purity_alljets_"+el.GetName(),nameX,"Purity",ymin,legxmin,legymin,legxmax,legymax,ymax,True,marker_sel,textalign)
#
#                purities = [purity_CHS,purity_CHS_PUIDTight,purity_CHS_PUIDMedium,purity_CHS_PUIDLoose,purity_v4_inc] 
#                names = ["CHS","CHS + tight PU jet ID","CHS + medium PU jet ID","CHS + loose PU jet ID","PUPPI"]
#
#                ymin = 0.4
#                ymax = 1.1
#                legxmin = 0.2
#                legxmax = 0.5
#                legymin = 0.2
#                legymax = 0.5
#
#                if "NPV" in el.GetName() and "central" in el.GetName():
#                    ymin = 0.6
#
#                if "NPV" in el.GetName() and "2p5to3" in el.GetName():
#                    ymin = 0.0
#                    ymax = 1.05
#                    legymin = 0.15
#                    legymax = 0.45
#                    legxmin = 0.15
#                    legxmax = 0.45
#
#                if "NPV" in el.GetName() and "3to5" in el.GetName():
#                    ymin = 0.1
#                    legymin = 0.5
#                    legymax = 0.8
#                    legxmin = 0.4
#                    legxmax = 0.7
#                    
#                plot(purities,names,folder+"/paper/","purity_alljets_"+el.GetName(),nameX,"Purity",ymin,legxmin,legymin,legxmax,legymax ,ymax,True)
#
#
#            if b_quarkgluon:
#                purities = [purity_v4_inc_pt20, purity_v4_quark, purity_v4_gluon]
#                names = ["PUPPI","PUPPI Quarks","PUPPI Gluons"]
#
#                ymin = 0.4
#                ymax = 1.1
#                legxmin = 0.2
#                legxmax = 0.5
#                legymin = 0.2
#                legymax = 0.5
#                if "NPV" in el.GetName() and "central" in el.GetName():
#                    ymin = 0.4
#                    ymax = 1.15
#                if "NPV" in el.GetName() and "2p5to3" in el.GetName():
#                    ymin = 0
#                    ymax = 1.6
#                    legxmin = 0.4
#                    legxmax = 0.7
#                    legymin = 0.5
#                    legymax = 0.8
#
#                if "NPV" in el.GetName() and "3to5" in el.GetName():
#                    ymin = 0
#                    ymax = 1.2
#                    legxmin = 0.35
#                    legxmax = 0.65
#                    legymin = 0.5
#                    legymax = 0.8
#
#                plot(purities,names,folder+"/paper_ptcut20GeV_quarkgluon/","purity_alljets_"+el.GetName(),nameX,"Purity",ymin,legxmin,legymin,legxmax,legymax,ymax,True)
#
#################################### Variables and Resolution
#    #plot_control_variables(infile_dict,hist_names,folder)
#    # hist_folder = "jet_output"
#    # plot_resolution(infile_dict,folder,"resolution/resolution_",hist_folder)
#
########################   QCD samples
#
#eff_purity_dict={}
#if b_QCD:
#    infile_dict={}
#
#    infile_QCD_ptg5 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_ptg5_koastas.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI ptg5"]=infile_QCD_ptg5
#
#    infile_puppi_kosta = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.KoastasSetup_DY.root")
#    TH1.AddDirectory(0)
#
#    infile_puppi_kosta_inc = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.KoastasSetup_inclusive_DY.root")
#    TH1.AddDirectory(0)
#
#    infile_puppi_inc = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_DY/uhh2.AnalysisModuleRunner.MC.DY_inc.root")
#    TH1.AddDirectory(0)
#
#    infile_QCD_v3 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_v3_neutralmultiplicity.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI v3"]=infile_QCD_v3
#
#    infile_QCD_v1 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_v1_neutralmultiplicity.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI v1"]=infile_QCD_v1
#
#    infile_QCD_v2 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_v2_neutralmultiplicity.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI v2"]=infile_QCD_v2
#
#    infile_QCD_v4 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_v4_neutralmultiplicity.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI v4"]=infile_QCD_v4
#
#    infile_QCD_orig = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_original.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI orig"]=infile_QCD_orig
#
#    infile_QCD_fix = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_highpTresolutionFix.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI fix"]=infile_QCD_fix
#
#    infile_QCD_orig_2016_102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison/uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2016_effmis.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI orig_2016_102"]=infile_QCD_orig_2016_102
#
#    infile_QCD_orig_2017_102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison/uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2017_effmis.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI orig_2017_102"]=infile_QCD_orig_2017_102
#
#    infile_QCD_orig_2018_102 = TFile("/nfs/dust/cms/user/abenecke/PUPPI/CMSSW_102X/rootfiles/CMSSW102vs80Comparison/uhh2.AnalysisModuleRunner.MC.PUPPI_QCD_2018_effmis.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI orig_2018_102"]=infile_QCD_orig_2018_102
#
#    infile_QCD_pt50 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_NJetpT50.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI pt50"]=infile_QCD_pt50
#
#    infile_QCD_2Jet_pTg50 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_2Jet_pTg50.root")
#    TH1.AddDirectory(0)
#    infile_dict["PUPPI 2Jet_pTg50"]=infile_QCD_2Jet_pTg50
#
#    infile_QCD_htsel = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_HTsel.root")
#    TH1.AddDirectory(0)
#
#    infile_QCD_htsel_pt50 = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_Koastas_NJetpT50_HTSel.root")
#    TH1.AddDirectory(0)
#
#
#    infile_QCD_CHS = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_CHS_koastas.root")
#    TH1.AddDirectory(0)
#
#    infile_QCD_CHS_withsel = TFile("/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/puppi_CMSSW_9_2_4/uhh2.AnalysisModuleRunner.MC.QCD_8_0_CHS_koastas_2njet_ptg50.root")
#    TH1.AddDirectory(0)
#
#
#
#    hist_folder = "jet_pt_8"
##    hist_folder = "jet_pt_8_wJEC"
#    lista = infile_QCD_CHS.Get(hist_folder).GetListOfKeys()
#    nameX="NaN"
#    folder = "PUPPI_tunning/"
#
#    for el in lista:
#        if "Eta" in el.GetName(): nameX = "#eta"
#        if "NPV" in el.GetName(): nameX = "NPV"
#        if "Pt" in el.GetName(): nameX = "p_{T}"
#        postfix = "_matchedjet"
#
#        if "Efficiency" in el.GetName() and "matched" not in el.GetName():
#            print str(("NPV" in el.GetName() and "to" in el.GetName() and "alljet" in el.GetName())) +"  or  "+str(("Pt" in el.GetName() and "to" not in el.GetName()))
#            if ("NPV" in el.GetName() and "to" in el.GetName() and "alljet" in el.GetName()) or ("Pt" in el.GetName() and "to" not in el.GetName()):
#                eff_purity_dict[el.GetName()]={}
#
#            print el.GetName()
#            # eff_QCD_ptg5 =calculate_eff(infile_QCD_ptg5,hist_folder,el.GetName(),el.GetName()+"_matchedjet","ptg5")
#            # eff_QCD_v3 =calculate_eff(infile_QCD_v3,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v3")
#            # eff_QCD_v1 =calculate_eff(infile_QCD_v1,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v1")
#            # eff_QCD_v2 =calculate_eff(infile_QCD_v2,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v2")
#            # eff_QCD_v4 =calculate_eff(infile_QCD_v4,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v4")
#            eff_QCD_orig =calculate_eff(infile_QCD_orig,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig")
#            eff_QCD_fix =calculate_eff(infile_QCD_fix,hist_folder,el.GetName(),el.GetName()+"_matchedjet","fix")
#
#            eff_QCD_orig_2016_102 =calculate_eff(infile_QCD_orig_2016_102,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig_2016_102")
#            eff_QCD_orig_2017_102 =calculate_eff(infile_QCD_orig_2017_102,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig_2017_102")
#            eff_QCD_orig_2018_102 =calculate_eff(infile_QCD_orig_2018_102,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig_2018_102")
#
##            eff_QCD_pt50 =calculate_eff(infile_QCD_pt50,hist_folder,el.GetName(),el.GetName()+"_matchedjet","pt50")
#            eff_QCD_2jet_pt50 =calculate_eff(infile_QCD_2Jet_pTg50,hist_folder,el.GetName(),el.GetName()+"_matchedjet","2Njet pt50")
##            eff_QCD_htsel =calculate_eff(infile_QCD_htsel,hist_folder,el.GetName(),el.GetName()+"_matchedjet","htsel")
##            eff_QCD_htsel_pt50 =calculate_eff(infile_QCD_htsel_pt50,hist_folder,el.GetName(),el.GetName()+"_matchedjet","htsel&pt50")
#            eff_QCD_quark =calculate_eff(infile_QCD_orig,"quark_pt_8",el.GetName(),el.GetName()+"_matchedjet","quark")
#            eff_QCD_gluon =calculate_eff(infile_QCD_orig,"gluon_pt_8",el.GetName(),el.GetName()+"_matchedjet","gluon")
#            eff_QCD_quark_sel =calculate_eff(infile_QCD_2Jet_pTg50,"quark_pt_8",el.GetName(),el.GetName()+"_matchedjet","quark")
#            eff_QCD_gluon_sel =calculate_eff(infile_QCD_2Jet_pTg50,"gluon_pt_8",el.GetName(),el.GetName()+"_matchedjet","gluon")
#            # eff_QCD_orig_wJEC =calculate_eff(infile_QCD_orig,"jet_pt_8_wJEC",el.GetName(),el.GetName()+"_matchedjet","orig w JEC")
#            # eff_QCD_orig_lepEvents =calculate_eff(infile_QCD_orig,"jet_pt_8_lepEvents",el.GetName(),el.GetName()+"_matchedjet","orig w lep cleaning")
#            eff_QCD_CHS =calculate_eff(infile_QCD_CHS,hist_folder,el.GetName(),el.GetName()+"_matchedjet","CHS")
#            eff_QCD_CHS_sel =calculate_eff(infile_QCD_CHS_withsel,hist_folder,el.GetName(),el.GetName()+"_matchedjet","CHS")
#            eff_puppi_orig_2016 =calculate_eff(infile_puppi_kosta,"EffPurity_output",el.GetName(),el.GetName()+postfix)
#            eff_puppi_orig_2016_inc =calculate_eff(infile_puppi_kosta_inc,"EffPurity_output",el.GetName(),el.GetName()+postfix)
#            eff_puppi_orig_2016_quark =calculate_eff(infile_puppi_kosta,"quark_EffPurity_output",el.GetName(),el.GetName()+postfix)
#            eff_puppi_orig_2016_gluon =calculate_eff(infile_puppi_kosta,"gluon_EffPurity_output",el.GetName(),el.GetName()+postfix)
#            # eff_inc =calculate_eff(infile_puppi_inc,"EffPurity_output",el.GetName(),el.GetName()+postfix)
#
#
##            plot([eff_QCD_v3,eff_QCD_v1,eff_QCD_v2,eff_QCD_v4,eff_QCD_orig,eff_QCD_ptg5,eff_QCD_CHS],["v3","v1","v2","v4","orig","ptg5","chs"],folder,"eff_"+el.GetName(),nameX,"Efficiency",0,0.7,0.2,0.9,0.4)
##            plot([eff_QCD_v3,eff_QCD_v1,eff_QCD_v2,eff_QCD_v4,eff_QCD_orig,eff_QCD_ptg5],["v3","v1","v2","v4","orig","ptg5","chs"],folder,"eff_"+el.GetName(),nameX,"Efficiency",0,0.7,0.7,0.9,0.9)
##            plot([eff_QCD_orig,eff_QCD_orig_wJEC,eff_QCD_orig_lepEvents],["orig","orig w jec","orig w lep cleaning"],folder+"/wJEC/","eff_"+el.GetName(),nameX,"Efficiency",0,0.7,0.7,0.9,0.9)
#            # plot([eff_QCD_orig,eff_QCD_fix,eff_puppi_orig_2016,eff_inc,eff_QCD_2jet_pt50,eff_QCD_CHS,eff_QCD_CHS_sel],["QCD","QCD PUPPI fix","DY","DY inc","QCD 2 jets w pt50","QCD CHS","QCD CHS 2 jets w pt50"],folder+"/inc/","eff_"+el.GetName(),nameX,"Efficiency",0,0.7,0.2,0.9,0.4)
#
#            plot([eff_QCD_orig,eff_puppi_orig_2016,eff_QCD_CHS,eff_QCD_orig_2016_102,eff_QCD_orig_2017_102,eff_QCD_orig_2018_102],["QCD PUPPI 2016 80X","DY PUPPI 2016 80X","QCD CHS 2016 80X","QCD PUPPI 2016 102","QCD PUPPI 2017 102","QCD PUPPI 2018 102"],folder+"/2016vs2017/","eff_"+el.GetName(),nameX,"Efficiency",0,0.5,0.2,0.9,0.4)
#
#
#            plot([eff_QCD_orig,eff_puppi_orig_2016,eff_QCD_CHS],["QCD PUPPI 2016","DY PUPPI 2016","QCD CHS 2016"],folder+"/DY_vs_QCD/","eff_"+el.GetName(),nameX,"Efficiency",0,0.5,0.2,0.7,0.4)
#
#            plot([eff_QCD_orig,eff_puppi_orig_2016,eff_QCD_quark,eff_QCD_gluon,eff_puppi_orig_2016_quark,eff_puppi_orig_2016_gluon,eff_QCD_quark_sel,eff_QCD_gluon_sel],["QCD","DY","QCD quark","QCD gluon","DY quark","DY gluon","QCD quark pT>50 GeV","QCD gluon pT>50 GeV"],folder+"/quark_gluon/","eff_"+el.GetName(),nameX,"Efficiency",0,0.7,0.2,0.9,0.4)
#
#
#        if "Purity" in el.GetName() and "matched" not in el.GetName():
#            # purity_QCD_ptg5 =calculate_eff(infile_QCD_ptg5,hist_folder,el.GetName(),el.GetName()+"_matchedjet","ptg5")
#            # purity_QCD_v3 =calculate_eff(infile_QCD_v3,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v3")
#            # purity_QCD_v1 =calculate_eff(infile_QCD_v1,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v1")
#            # purity_QCD_v2 =calculate_eff(infile_QCD_v2,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v2")
#            # purity_QCD_v4 =calculate_eff(infile_QCD_v4,hist_folder,el.GetName(),el.GetName()+"_matchedjet","v4")
#            purity_QCD_orig =calculate_eff(infile_QCD_orig,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig")
#            purity_QCD_fix =calculate_eff(infile_QCD_fix,hist_folder,el.GetName(),el.GetName()+"_matchedjet","fix")
#
#            purity_QCD_orig_2016_102 =calculate_eff(infile_QCD_orig_2016_102,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig_2016_102")
#            purity_QCD_orig_2017_102 =calculate_eff(infile_QCD_orig_2017_102,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig_2017_102")
#            purity_QCD_orig_2018_102 =calculate_eff(infile_QCD_orig_2018_102,hist_folder,el.GetName(),el.GetName()+"_matchedjet","orig_2018_102")
#
#            # purity_QCD_pt50 =calculate_eff(infile_QCD_pt50,hist_folder,el.GetName(),el.GetName()+"_matchedjet","pt50")
#            # purity_QCD_htsel =calculate_eff(infile_QCD_htsel,hist_folder,el.GetName(),el.GetName()+"_matchedjet","htsel")
#            # purity_QCD_htsel_pt50 =calculate_eff(infile_QCD_htsel_pt50,hist_folder,el.GetName(),el.GetName()+"_matchedjet","htsel&pt50")
#            # purity_QCD_quark =calculate_eff(infile_QCD_orig,"quark_pt_8",el.GetName(),el.GetName()+"_matchedjet","quark")
#            # purity_QCD_gluon =calculate_eff(infile_QCD_orig,"gluon_pt_8",el.GetName(),el.GetName()+"_matchedjet","gluon")
#            # purity_QCD_orig_wJEC =calculate_eff(infile_QCD_orig,"jet_pt_8_wJEC",el.GetName(),el.GetName()+"_matchedjet","orig w JEC")
#            # purity_QCD_orig_lepEvents =calculate_eff(infile_QCD_orig,"jet_pt_8_lepEvents",el.GetName(),el.GetName()+"_matchedjet","orig w lep cleaning")
#            purity_QCD_CHS =calculate_eff(infile_QCD_CHS,hist_folder,el.GetName(),el.GetName()+"_matchedjet","CHS")
#            purity_kosta =calculate_eff(infile_puppi_kosta,"EffPurity_output",el.GetName(),el.GetName()+postfix)
#            purity_kosta_inc =calculate_eff(infile_puppi_kosta_inc,"EffPurity_output",el.GetName(),el.GetName()+postfix)
#            purity_inc =calculate_eff(infile_puppi_inc,"EffPurity_output",el.GetName(),el.GetName()+postfix)
#
##            plot([purity_QCD_v3,purity_QCD_v1,purity_QCD_v2,purity_QCD_v4,purity_QCD_orig,purity_QCD_ptg5,purity_QCD_CHS],["v3","v1","v2","v4","orig","ptg5","chs"],folder,"purity_"+el.GetName(),nameX,"Purity",0,0.15,0.15,0.35,0.35)
##            plot([purity_QCD_v3,purity_QCD_v1,purity_QCD_v2,purity_QCD_v4,purity_QCD_orig,purity_QCD_ptg5],["v3","v1","v2","v4","orig","ptg5","chs"],folder,"purity_"+el.GetName(),nameX,"Purity",0,0.15,0.15,0.35,0.35)
##            plot([purity_QCD_orig,purity_QCD_orig_wJEC,purity_QCD_orig_lepEvents],["orig","orig w JEC","orig w lep cleaning"],folder+"/wJEC/","purity_"+el.GetName(),nameX,"Purity",0,0.15,0.15,0.35,0.35)
#
##            plot([purity_QCD_orig,purity_QCD_fix,purity_kosta,purity_inc,purity_QCD_pt50,purity_QCD_htsel,purity_kosta_inc,purity_QCD_htsel_pt50],["QCD","QCD fix","DY","DY inc","QCD pT > 50","QCD HT>140","DY HT inc","QCD HT&pt50"],folder+"/inc/","purity_"+el.GetName(),nameX,"Purity",0,0.15,0.15,0.35,0.35)
#
#            plot([purity_QCD_orig,purity_kosta,purity_QCD_CHS,purity_QCD_orig_2016_102,purity_QCD_orig_2017_102,purity_QCD_orig_2018_102],["QCD PUPPI 2016 80X","DY PUPPI 2016 80X","QCD CHS 2016 80X","QCD PUPPI 2016 102","QCD PUPPI 2017 102","QCD PUPPI 2018 102"],folder+"/2016vs2017/","purity_"+el.GetName(),nameX,"Purity",0,0.35,0.15,0.55,0.35)
#
#
#
##    plot_resolution(infile_dict,folder,"resolution_")
##    plot_signal_over_bkg(eff_purity_dict, folder, "soverb_")
