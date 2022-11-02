#! /usr/bin/env python
import os
import re
import math
#from ROOT import *
import ROOT
from array import array


## ------------------------------------
##  systematics class
## ------------------------------------

class systematicsClass:

    def __init__(self,theMass,theForXSxBR,theisFSR,theInputs,year):
	
	self.year = year

        self.ID_4mu = 1
        self.ID_4e = 2
        self.ID_2e2mu = 3
	self.ID_2mu2e = 4

        self.sqrts = theInputs['sqrts']
        self.channel = theInputs['decayChannel']
        self.mH = theMass
        self.isForXSxBR = theForXSxBR
        self.isFSR = theisFSR
        self.model = theInputs['model']

        self.lumiUncertainty = theInputs['lumiUnc']
	self.lumiUncertainty_correlated = theInputs['lumiUncCorrelated_16_17_18']
	self.lumiUncertainty_correlated1718 = theInputs['lumiUncCorrelated_17_18']

        self.mu_signal_low = theInputs['mu_signal_low']
        self.mu_signal_high = theInputs['mu_signal_high']
	self.mu_ggzz_low = theInputs['mu_ggzz_low']
	self.mu_ggzz_high = theInputs['mu_ggzz_high']
	self.mu_qqzz_low = theInputs['mu_qqzz_low']
	self.mu_qqzz_high = theInputs['mu_qqzz_high']

        self.ele_signal_low = theInputs['ele_signal_low']
        self.ele_signal_high = theInputs['ele_signal_high']
	self.ele_ggzz_low = theInputs['ele_ggzz_low']
	self.ele_ggzz_high = theInputs['ele_ggzz_high']
	self.ele_qqzz_low = theInputs['ele_qqzz_low']
	self.ele_qqzz_high = theInputs['ele_qqzz_high']

        self.zjetKappaLow = theInputs['zjetsKappaLow']
        self.zjetKappaHigh = theInputs['zjetsKappaHigh']


    def setSystematics(self,theRateBkg_qqZZ,theRateBkg_ggZZ,theRateBkg_zjets ):
    	pass

    def Write_Systematics_Line(self,systLine,theFile,theInputs):
        print "~~~~~~~~~~~~~~~~~"
        channelList=['ggH','qqH','WH','ZH','ttH','bbH','tHq','qqZZ','ggZZ','zjets','ttbar','zbb']
        if theInputs["all"]:
            channelList=['ggH','qqZZ','ggZZ','zjets','ttbar','zbb']
        
        for chan in channelList:
            if theInputs[chan] or (chan.startswith("ggH") and theInputs["all"]):
                print chan, systLine[chan]
                theFile.write(systLine[chan])
        theFile.write("\n")
        
    def Build_lumi(self,theFile,theInputs):
        if(self.sqrts == 7):
            theFile.write("lumi_7TeV lnN ")
        elif (self.sqrts == 8):
            theFile.write("lumi_8TeV lnN ")
        elif (self.sqrts == 13):
	    tmpyear = self.year
	    if self.year=='20160' or self.year=='20165':
		    tmpyear = '2016'
            theFile.write("lumi_13TeV_"+tmpyear+" lnN ")
        else:
            raise RuntimeError, "Unknown sqrts in systematics!"

        systLine={'ggH':"{0} ".format(self.lumiUncertainty)}
        systLine['qqH']  = "{0} ".format(self.lumiUncertainty)
        systLine['WH']   = "{0} ".format(self.lumiUncertainty)
        systLine['ZH']   = "{0} ".format(self.lumiUncertainty)
        systLine['ttH']  = "{0} ".format(self.lumiUncertainty)
        systLine['bbH'] = "{0} ".format(self.lumiUncertainty)
	systLine['tHq'] = "{0} ".format(self.lumiUncertainty)
	systLine['qqZZ'] = "{0} ".format(self.lumiUncertainty)
        systLine['ggZZ'] = "{0} ".format(self.lumiUncertainty)
        systLine['zjets']= "- "
        systLine['ttbar']= "{0} ".format(self.lumiUncertainty)
        systLine['zbb']  = "{0} ".format(self.lumiUncertainty)
        self.Write_Systematics_Line(systLine,theFile,theInputs)

        theFile.write('lumi_13TeV_correlated lnN ')
	systLine={'ggH':"{0} ".format(self.lumiUncertainty_correlated)}
        systLine['qqH']  = "{0} ".format(self.lumiUncertainty_correlated)
        systLine['WH']   = "{0} ".format(self.lumiUncertainty_correlated)
        systLine['ZH']   = "{0} ".format(self.lumiUncertainty_correlated)
        systLine['ttH']  = "{0} ".format(self.lumiUncertainty_correlated)
        systLine['bbH'] = "{0} ".format(self.lumiUncertainty_correlated)
	systLine['tHq'] = "{0} ".format(self.lumiUncertainty_correlated)
	systLine['qqZZ'] = "{0} ".format(self.lumiUncertainty_correlated)
        systLine['ggZZ'] = "{0} ".format(self.lumiUncertainty_correlated)
        systLine['zjets']= "- "
        systLine['ttbar']= "{0} ".format(self.lumiUncertainty_correlated)
        systLine['zbb']  = "{0} ".format(self.lumiUncertainty_correlated)
        self.Write_Systematics_Line(systLine,theFile,theInputs)
	
	if self.year == '2018' or self.year == '2017':
		theFile.write('lumi_13TeV_correlated1718 lnN ')
		systLine={'ggH':"{0} ".format(self.lumiUncertainty_correlated1718)}
		systLine['qqH']  = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['WH']   = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['ZH']   = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['ttH']  = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['bbH'] = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['tHq'] = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['qqZZ'] = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['ggZZ'] = "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['zjets']= "- "
		systLine['ttbar']= "{0} ".format(self.lumiUncertainty_correlated1718)
		systLine['zbb']  = "{0} ".format(self.lumiUncertainty_correlated1718)
		self.Write_Systematics_Line(systLine,theFile,theInputs)
        
    def Write_pdf_qqbar(self,theFile,theInputs):

        theFile.write("pdf_qqbar lnN ")
            
        if not self.isForXSxBR:
            systLine={'ggH':"- "}
            systLine['qqH']  = "1.021/0.979 "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_vbf-self.CSpdfErrMinus_vbf)/2.)
            systLine['WH']   = "1.021/0.979 "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_wh-self.CSpdfErrMinus_wh)/2.)
            systLine['ZH']   = "1.021/0.979 "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_zh-self.CSpdfErrMinus_zh)/2.)
            systLine['ttH']  = "- "
	    systLine['bbH'] = "- "
	    systLine['tHq'] = "- "
            systLine['qqZZ'] = "1.031/0.996 "#"{0:.4f} ".format(self.qqVV_pdfSys)
            systLine['ggZZ'] = "- "
            systLine['zjets']= "- "
            systLine['ttbar']= "- "
            systLine['zbb']  = "- "
            
            self.Write_Systematics_Line(systLine,theFile,theInputs)

    def Write_pdf_Higgs_qq(self,theFile,theInputs):

        theFile.write("pdf_Higgs_qq lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "1.021/0.979 "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_vbf-self.CSpdfErrMinus_vbf)/2.)
        systLine['WH']   = "1.021/0.979 "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_wh-self.CSpdfErrMinus_wh)/2.)
        systLine['ZH']   = "1.021/0.979 "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_zh-self.CSpdfErrMinus_zh)/2.)
        systLine['ttH']  = "- "
	systLine['bbH']  = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- "#"{0:.4f} ".format(self.qqVV_pdfSys)
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- "
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "

        self.Write_Systematics_Line(systLine,theFile,theInputs)

    def Write_pdf_qq(self,theFile,theInputs):

        theFile.write("pdf_qq lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_vbf-self.CSpdfErrMinus_vbf)/2.)
        systLine['WH']   = "- "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_wh-self.CSpdfErrMinus_wh)/2.)
        systLine['ZH']   = "- "#"{0:.4f} ".format(1. + (self.CSpdfErrPlus_zh-self.CSpdfErrMinus_zh)/2.)
        systLine['ttH']  = "- "
	systLine['bbH']  = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "1.021/0.979 "#"{0:.4f} ".format(self.qqVV_pdfSys)
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- "
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "

        self.Write_Systematics_Line(systLine,theFile,theInputs)


                            
    def Write_pdf_gg(self,theFile,theInputs):

        theFile.write("pdf_gg lnN ")

        if not self.isForXSxBR and not self.model=="FF":
            #ttHSyst = 1 - (self.CSpdfErrPlus_tth-self.CSpdfErrMinus_tth)/2
            #if self.mH > 200 : ttHSyst = 1 + (self.CSpdfErrPlus_tth-self.CSpdfErrMinus_tth)/2

#            systLine={'ggH':"{0:.4f} ".format(1. + (self.CSpdfErrPlus_gg-self.CSpdfErrMinus_gg)/2.)}
            systLine = {'ggH':"1.0720 "}
            systLine['qqH']  = "- "
            systLine['WH']   = "- " 
            systLine['ZH']   = "- "
            systLine['ttH']  = "0.9220 "#.format(ttHSyst)
            systLine['bbH'] = "-"
	    systLine['tHq'] = "- "
	    systLine['qqZZ'] = "- " 
            systLine['ggZZ'] = "1.031 "#"{0:.4f} ".format(self.ggVV_pdfSys)
            systLine['zjets']= "- " 
            systLine['ttbar']= "- "
            systLine['zbb']  = "- "
            
            self.Write_Systematics_Line(systLine,theFile,theInputs)
            
        elif self.isForXSxBR:
            systLine={'ggH':"- "}
            systLine['qqH']  = "- "
            systLine['WH']   = "- " 
            systLine['ZH']   = "- "
            systLine['ttH']  = "- " 
	    systLine['bbH'] = "- "
	    systLine['tHq'] = "- "
            systLine['qqZZ'] = "- " 
            systLine['ggZZ'] = "{0:.4f} ".format(self.ggVV_pdfSys)
            systLine['zjets']= "- " 
            systLine['ttbar']= "- "
            systLine['zbb']  = "- "
            
            self.Write_Systematics_Line(systLine,theFile,theInputs)            


    def Write_pdf_Higgs_gg(self,theFile,theInputs):

        theFile.write("pdf_Higgs_gg lnN ")

        systLine = {'ggH':"1.032/0.968 "}
        systLine['qqH']  = "- "
        systLine['WH']   = "- "
        systLine['ZH']   = "- "
        systLine['ttH']  = "- "
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- "
        systLine['ggZZ'] = "1.032/0.968 "#"{0:.4f} ".format(self.ggVV_pdfSys)
        systLine['zjets']= "- "
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "

        self.Write_Systematics_Line(systLine,theFile,theInputs)


    def Write_pdf_Higgs_ttH(self,theFile,theInputs):
            
        theFile.write("pdf_Higgs_ttH lnN ")

        systLine = {'ggH':"- "}
        systLine['qqH']  = "- "
        systLine['WH']   = "- "
        systLine['ZH']   = "- "
        systLine['ttH']  = "1.036/0.964 "
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- "
        systLine['ggZZ'] = "- "#"{0:.4f} ".format(self.ggVV_pdfSys)
        systLine['zjets']= "- "
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "

        self.Write_Systematics_Line(systLine,theFile,theInputs)

    def Write_QCDscale_ggH(self,theFile,theInputs):
        
        theFile.write("QCDscale_ggH lnN ")
        systLine = {'ggH':"1.039/0.961 " }
        systLine['qqH']  = "- "
        systLine['WH']   = "- " 
        systLine['ZH']   = "- "
        systLine['ttH']  = "- "
	systLine['bbH'] = "- "
	systLine['tHq'] = "- " 
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- " 
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)            

    def Write_QCDscale_qqH(self,theFile,theInputs):

        theFile.write("QCDscale_qqH lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "1.004/0.997 "
        systLine['WH']   = "- " 
        systLine['ZH']   = "- "
        systLine['ttH']  = "- " 
	systLine['bbH'] = "- "
	systLine["tHq"] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- " 
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)            
        
    def Write_QCDscale_VH(self,theFile,theInputs):
        theFile.write("QCDscale_VH lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- "
        systLine['WH']   = "1.005/0.993 "#.format(1. + (self.CSscaleErrPlus_wh-self.CSscaleErrMinus_wh)/2.)
        systLine['ZH']   = "1.038/0.969 "#.format(1. + (self.CSscaleErrPlus_zh-self.CSscaleErrMinus_zh)/2.)
        systLine['ttH']  = "- "
	systLine['bbH'] = "- " 
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- " 
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)                    
        
    def Write_QCDscale_ttH(self,theFile,theInputs):

        theFile.write("QCDscale_ttH lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- "
        systLine['WH']   = "- " 
        systLine['ZH']   = "- "
        systLine['ttH']  = "1.058/0.908 "#.format(1. + (self.CSscaleErrPlus_tth-self.CSscaleErrMinus_tth)/2.)
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- " 
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)                    
        
    def Write_QCDscale_ggVV(self,theFile,theInputs):

        theFile.write("QCDscale_ggVV lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- "
        systLine['WH']   = "- " 
        systLine['ZH']   = "- "
        systLine['ttH']  = "- "
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "1.039/0.961 "#"{0:.4f} ".format(self.ggVV_scaleSys)
        systLine['zjets']= "- " 
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)                         

    def Write_QCDscale_VV(self,theFile,theInputs):

        theFile.write("QCDscale_VV lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- "
        systLine['WH']   = "- " 
        systLine['ZH']   = "- "
        systLine['ttH']  = "- "
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "1.032/0.958 "#"{0:.4f} ".format(self.qqVV_scaleSys) 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- " 
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)                         
    
    def Write_BRhiggs_hzz4l(self,theFile,theInputs):

        theFile.write("hzz_br lnN ")

        systLine={'ggH':"1.02 "}
        systLine['qqH']  = "1.02 " 
        systLine['WH']   = "1.02 " 
        systLine['ZH']   = "1.02 " 
        systLine['ttH']  = "1.02 " 
	systLine['bbH'] = "1.02 "
	systLine['tHq'] = "1.02 "
        systLine['qqZZ'] = "- "
        systLine['ggZZ'] = "- "
        systLine['zjets']= "- "
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)
    
    def Write_eff_e(self,theFile,theInputs):
    
        if self.channel == self.ID_4e or self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e:
            theFile.write("CMS_eff_e lnN ")

	    sig4edn = self.ele_signal_low
	    sig4eup = self.ele_signal_high
	    qqzz4edn = self.ele_qqzz_low
	    qqzz4eup = self.ele_qqzz_high
	    ggzz4edn = self.ele_ggzz_low
	    ggzz4eup = self.ele_ggzz_high

	    systLine={'ggH':"{0:.3f}/{1:.3f} ".format(sig4edn,sig4eup)}
            systLine['qqH']  = "{0:.3f}/{1:.3f} ".format(sig4edn,sig4eup)
            systLine['WH']   = "{0:.3f}/{1:.3f} ".format(sig4edn,sig4eup)
            systLine['ZH']   = "{0:.3f}/{1:.3f} ".format(sig4edn,sig4eup)
            systLine['ttH']  = "{0:.3f}/{1:.3f} ".format(sig4edn,sig4eup)
	    systLine['bbH']  = "{0:.3f}/{1:.3f} ".format(sig4edn,sig4eup)
	    systLine['tHq'] = "{0:.3f}/{1:.3f} ".format(sig4edn,sig4eup)
            systLine['qqZZ'] = "{0:.3f}/{1:.3f} ".format(qqzz4edn,qqzz4eup)
            systLine['ggZZ'] = "{0:.3f}/{1:.3f} ".format(ggzz4edn,ggzz4eup)
            systLine['zjets']= "- "
            systLine['ttbar']= "- "
            systLine['zbb']  = "- "
            
            self.Write_Systematics_Line(systLine,theFile,theInputs)
    
    def Write_eff_m(self,theFile,theInputs):
        if self.channel == self.ID_4mu or self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e:
            theFile.write("CMS_eff_m lnN ")

            sig4mudn = self.mu_signal_low
	    sig4muup = self.mu_signal_high
	    ggzz4mudn = self.mu_ggzz_low
	    ggzz4muup = self.mu_ggzz_high
	    qqzz4mudn = self.mu_qqzz_low
	    qqzz4muup = self.mu_qqzz_high

	    systLine={'ggH':"{0:.3f}/{1:.3f} ".format(sig4mudn,sig4muup)}
            systLine['qqH']  = "{0:.3f}/{1:.3f} ".format(sig4mudn,sig4muup)
            systLine['WH']   = "{0:.3f}/{1:.3f} ".format(sig4mudn,sig4muup)
            systLine['ZH']   = "{0:.3f}/{1:.3f} ".format(sig4mudn,sig4muup)
            systLine['ttH']  = "{0:.3f}/{1:.3f} ".format(sig4mudn,sig4muup)
	    systLine['bbH']  = "{0:.3f}/{1:.3f} ".format(sig4mudn,sig4muup)
	    systLine['tHq'] = "{0:.3f}/{1:.3f} ".format(sig4mudn,sig4muup)
            systLine['qqZZ'] = "{0:.3f}/{1:.3f} ".format(qqzz4mudn,qqzz4muup)
            systLine['ggZZ'] = "{0:.3f}/{1:.3f} ".format(ggzz4mudn,ggzz4muup)
            systLine['zjets']= "- "
            systLine['ttbar']= "- "
            systLine['zbb']  = "- "
            
            self.Write_Systematics_Line(systLine,theFile,theInputs)
    
    def Write_CMS_hzz2mu2e_Zjets(self,theFile,theInputs):

        theFile.write("CMS_hzz2mu2e_FakeRate_"+self.year+" lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- " 
        systLine['WH']   = "- " 
        systLine['ZH']   = "- " 
        systLine['ttH']  = "- " 
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "{0:.3f}/{1:.3f} ".format(self.zjetKappaLow,self.zjetKappaHigh)
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)
    
    
    def Write_CMS_hzz2e2mu_Zjets(self,theFile,theInputs):

        theFile.write("CMS_hzz2e2mu_FakeRate_"+self.year+" lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- " 
        systLine['WH']   = "- " 
        systLine['ZH']   = "- " 
        systLine['ttH']  = "- " 
	systLine['tHq'] = "- "
	systLine['bbH'] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "{0:.3f}/{1:.3f} ".format(self.zjetKappaLow,self.zjetKappaHigh)
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)

    def Write_CMS_hzz4mu_Zjets(self,theFile,theInputs):

        theFile.write("CMS_hzz4mu_FakeRate_"+self.year+" lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- " 
        systLine['WH']   = "- " 
        systLine['ZH']   = "- " 
        systLine['ttH']  = "- " 
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "{0:.3f}/{1:.3f} ".format(self.zjetKappaLow,self.zjetKappaHigh)
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)

    def Write_CMS_hzz4e_Zjets(self,theFile,theInputs):
        
        theFile.write("CMS_hzz4e_FakeRate_"+self.year+" lnN ")

        systLine={'ggH':"- "}
        systLine['qqH']  = "- " 
        systLine['WH']   = "- " 
        systLine['ZH']   = "- " 
        systLine['ttH']  = "- "
	systLine['bbH'] = "- " 
	systLine['tHq'] = "- "
        systLine['qqZZ'] = "- " 
        systLine['ggZZ'] = "- "
        systLine['zjets']= "{0:.3f}/{1:.3f} ".format(self.zjetKappaLow,self.zjetKappaHigh)
        systLine['ttbar']= "- "
        systLine['zbb']  = "- "
        
        self.Write_Systematics_Line(systLine,theFile,theInputs)

    def Write_CMS_kfactor_ggzz(self,theFile,theInputs):
    	theFile.write("kfactor_ggzz lnN ")
	systLine={'ggH':"- "}
	systLine['qqH'] = "- "
	systLine['WH'] = "- "
	systLine['ZH'] = "- "
	systLine['ttH'] = "- "
	systLine['bbH'] = "- "
	systLine['tHq'] = "- "
	systLine['qqZZ'] = "- "
	systLine['ggZZ'] = "1.1 "
	systLine['zjets'] = "- "
	systLine['ttbar'] = "- "
	systLine['zbb'] = "- "
	self.Write_Systematics_Line(systLine,theFile,theInputs)

    def Write_CMS_zz4l_bkgMELA(self,theFile,theInputs):
        theFile.write("#CMS_zz4l_bkgMELA param 0  1  [-3,3]\n")

    def Write_CMS_zz4l_sigMELA(self,theFile,theInputs):
        theFile.write("CMS_zz4l_sigMELA param 0  1  [-3,3]\n")

    
    def WriteSystematics(self,theFile,theInputs, theVBFcat=False):										######

            self.Build_lumi(theFile,theInputs)

            self.Write_pdf_Higgs_gg(theFile,theInputs)												#true, no channel dependence	
            self.Write_pdf_Higgs_ttH(theFile,theInputs)												#true, no channel dependence

            self.Write_pdf_qqbar(theFile,theInputs)												
		
            self.Write_QCDscale_ggH(theFile,theInputs)											#true, no channal dependence

            self.Write_QCDscale_qqH(theFile,theInputs)											#true, no channel dependence

            self.Write_QCDscale_VH(theFile,theInputs)											#true, no channel dependence	
	    
            self.Write_QCDscale_ttH(theFile,theInputs)											#true, no channel dependence

            self.Write_QCDscale_ggVV(theFile,theInputs)

            self.Write_QCDscale_VV(theFile,theInputs)												#true, no channel dependence
	
            self.Write_BRhiggs_hzz4l(theFile,theInputs)												#true, no channel dependence
	    
	    self.Write_CMS_kfactor_ggzz(theFile,theInputs)
	     
            self.Write_eff_m(theFile,theInputs)													#true, channel dependent, add 2mu2e
            self.Write_eff_e(theFile,theInputs)
	    
	    if self.channel == self.ID_4mu:
	            self.Write_CMS_hzz4mu_Zjets(theFile,theInputs)
		    
	    if self.channel == self.ID_4e:	  
	            self.Write_CMS_hzz4e_Zjets(theFile,theInputs)
            
	    if self.channel == self.ID_2e2mu:
	            self.Write_CMS_hzz2e2mu_Zjets(theFile,theInputs)
	    
	    if self.channel == self.ID_2mu2e:
		    self.Write_CMS_hzz2mu2e_Zjets(theFile,theInputs)
	    '''
            if theInputs['useCMS_zz4l_bkgMELA']:													#true, no channel dependence
               self.Write_CMS_zz4l_bkgMELA(theFile,theInputs)
            
            if theInputs['useCMS_zz4l_sigMELA']:													#false, no channel dependence
               self.Write_CMS_zz4l_sigMELA(theFile,theInputs)
	    '''


    def WriteShapeSystematics(self,theFile,theInputs):
  
        meanCB_e_errPerCent = theInputs['CMS_zz4l_mean_e_sig']
        sigmaCB_e_errPerCent = theInputs['CMS_zz4l_sigma_e_sig']
        meanCB_m_errPerCent = theInputs['CMS_zz4l_mean_m_sig']
        sigmaCB_m_errPerCent = theInputs['CMS_zz4l_sigma_m_sig']
        

        if( self.channel == self.ID_4mu):

                theFile.write("CMS_zz4l_mean_m_sig param 0.0 1.0 \n")
                theFile.write("CMS_zz4l_sigma_m_sig param 0.0 {0} \n".format(sigmaCB_m_errPerCent))
            
        if( self.channel == self.ID_4e):

                theFile.write("CMS_zz4l_mean_e_sig param 0.0 1.0 \n")
                theFile.write("CMS_zz4l_sigma_e_sig param 0.0 {0} \n".format(sigmaCB_e_errPerCent))
            
        if( self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ):

                theFile.write("CMS_zz4l_mean_m_sig param 0.0 1.0 \n")
                theFile.write("CMS_zz4l_mean_e_sig param 0.0 1.0 \n".format(meanCB_e_errPerCent))
                theFile.write("CMS_zz4l_sigma_m_sig param 0.0 {0} \n".format(sigmaCB_m_errPerCent))
                theFile.write("CMS_zz4l_sigma_e_sig param 0.0 {0} \n".format(sigmaCB_e_errPerCent))

