#!/usr/bin/python
import os
import re
import math
import collections
from ROOT import *
from array import array

## ---------------------------------------------------------------
## card reader class
## ---------------------------------------------------------------

class inputReader:

    def __init__(self, inputTextFile):

        if not os.path.exists(inputTextFile):
            raise RuntimeError, "File {0} does not exist!!!".format(inputTextFile)
        
        # input file
        self.theInput = inputTextFile
        # model
        self.model = ""
        # decay channel
        self.decayChan = -999.9
        # lumi
        self.lumi = -999.9
        # sqrts
        self.sqrts = -999.9
        # channels
        self.all_chan = False
	self.ggH_chan = False
        self.qqH_chan = False
        self.WH_chan = False
        self.ZH_chan = False
        self.ttH_chan = False
        self.bbH_chan = False
	self.tHq_chan = False
	self.qqZZ_chan = False
        self.ggZZ_chan = False
        self.zjets_chan = False
       	self.ttbar_chan = False
	self.zbb_chan = False
	self.tHq_chan = False
	 
        # systematics 
        self.zjetsKappaLow = -999.9
        self.zjetsKappaHigh = -999.9
        self.lumiUnc = -999.9
        self.lumiUncCorrelated161718 = -999.9
	self.lumiUncCorrelated1718 = -999.9
	self.mu_signal_low = -999.9
	self.mu_signal_high = -999.9
        self.mu_ggzz_low = -999.9
	self.mu_ggzz_high = -999.9
        self.mu_qqzz_low = -999.9
	self.mu_qqzz_high = -999.9
        self.ele_signal_low = -999.9
	self.ele_signal_high = -999.9
        self.ele_qqzz_low = -999.9
	self.ele_qqzz_high = -999.9
        self.ele_ggzz_low = -999.9
	self.ele_ggzz_high = -999.9

        self.CMS_zz4l_mean_m_sig = -999.9
        self.CMS_zz4l_sigma_m_sig = -999.9
        self.CMS_zz4l_mean_e_sig = -999.9
        self.CMS_zz4l_sigma_e_sig = -999.9


    def readInputs(self):
        
        for line in open(self.theInput,'r'):
            f = line.split()
            if len(f) < 1: continue

            if f[0].startswith("#"): continue
            
            if f[0].lower().startswith("model"):
                if f[1].upper() == "SM": self.model = "SM"
                elif f[1].upper() == "SM4": self.model = "SM4"
                elif f[1].upper() == "FF" or f[1].upper() == "FP": self.model = "FF"
                else : raise RuntimeError, "Unknow model {0}, choices are SM, SM4, FF".format(f[1].upper()) 

            if f[0].lower().startswith("decay"):
                if f[1] == "4mu": self.decayChan = 1
                elif f[1] == "4e": self.decayChan = 2
                elif f[1] == "2e2mu": self.decayChan = 3
                elif f[1] == "2mu2e": self.decayChan = 4
                else : raise RuntimeError, "Unknown decay channel {0}, choices are 4mu, 4e, or 2e2mu".format(f[1])
                
            if f[0].lower().startswith("channels"):
                for chan in f:
                    if chan == f[0]: continue
                    if chan.lower().startswith("ggh"):     self.ggH_chan = True#True
                    elif chan.lower().startswith("qqh"):   self.qqH_chan = True
                    elif chan.lower().startswith("wh"):    self.WH_chan = True
                    elif chan.lower().startswith("zh"):    self.ZH_chan = True
                    elif chan.lower().startswith("tth"):   self.ttH_chan = True
		    elif chan.lower().startswith("bbh"):   self.bbH_chan = True
		    elif chan.lower().startswith('thq'):   self.tHq_chan = True
                    elif chan.lower().startswith("qqzz"):  self.qqZZ_chan = True
                    elif chan.lower().startswith("ggzz"):  self.ggZZ_chan = True
                    elif chan.lower().startswith("zjets"): self.zjets_chan = True
                    elif chan.lower().startswith("ttbar"): self.ttbar_chan = False
                    elif chan.lower().startswith("zbb"):   self.zbb_chan = False
                    elif chan.lower().startswith("all"):   self.all_chan = False
		    else : raise RuntimeError, "Unknown channel {0}, choices are ggH, qqH, WH, ZH, ttH, qqZZ, ggZZ, zjets".format(chan)

            if f[0].lower().startswith("systematic"):
                
		if f[1].lower().startswith("zjet") and f[1].lower().find("kappalow") >= 0 :
                    self.zjetsKappaLow = f[2]
                if f[1].lower().startswith("zjet") and f[1].lower().find("kappahigh") >= 0 :
                    self.zjetsKappaHigh = f[2]
                
		if f[1].lower() == "lumiunc":
                    self.lumiUnc = f[2]
		
		if f[1].lower().startswith('lumiunccorrelated_16_17_18'):
		    self.lumiUncCorrelated161718 = f[2]
		if f[1].lower().startswith('lumiunccorrelated_17_18'):
		    self.lumiUncCorrelated1718 = f[2]
                
		if f[1].lower().startswith("mu_signal"):
                    self.mu_signal_low = f[2]
		    self.mu_signal_high = f[3]
		if f[1].lower().startswith('mu_ggzz'):
                    self.mu_ggzz_low = f[2]
		    self.mu_ggzz_high = f[3]
		if f[1].lower().startswith('mu_qqzz'):
		    self.mu_qqzz_low = f[2]
		    self.mu_qqzz_high = f[3]

		if f[1].lower().startswith('ele_signal'):
		    self.ele_signal_low = f[2]
		    self.ele_signal_high = f[3]
		if f[1].lower().startswith('ele_ggzz'):
		    self.ele_ggzz_low = f[2]
		    self.ele_ggzz_high = f[3]
		if f[1].lower().startswith('ele_qqzz'):
		    self.ele_qqzz_low = f[2]
		    self.ele_qqzz_high = f[3]

                if f[1].lower().startswith("param"):
                    if f[2].lower().startswith("cms_zz4l_mean_m_sig"):
                        self.CMS_zz4l_mean_m_sig = f[3]
                    if f[2].lower().startswith("cms_zz4l_sigma_m_sig"):
                        self.CMS_zz4l_sigma_m_sig = f[3]
                    if f[2].lower().startswith("cms_zz4l_mean_e_sig"):
                        self.CMS_zz4l_mean_e_sig = f[3]
                    if f[2].lower().startswith("cms_zz4l_sigma_e_sig"):
                        self.CMS_zz4l_sigma_e_sig = f[3]
                    
            if f[0].lower().startswith("lumi"):
                self.lumi = float(f[1])

            if f[0].lower().startswith("sqrts"):
                self.sqrts = float(f[1])
    
    def getInputs(self):

        dict = {}

        dict['decayChannel'] = int(self.decayChan)
        dict['model'] = str(self.model)
        dict['lumi'] = float(self.lumi)
        dict['sqrts'] = float(self.sqrts)

        dict['all'] = self.all_chan
        dict['ggH'] = self.ggH_chan
        dict['qqH'] = self.qqH_chan
        dict['WH'] = self.WH_chan
        dict['ZH'] = self.ZH_chan
        dict['ttH'] = self.ttH_chan
        dict['bbH'] = self.bbH_chan
	dict['tHq'] = self.tHq_chan
	dict['ggH_SM'] = self.ggH_chan
        dict['qqH_SM'] = self.qqH_chan
        dict['WH_SM'] = self.WH_chan
        dict['ZH_SM'] = self.ZH_chan
        dict['ttH_SM'] = self.ttH_chan
        dict['qqZZ'] = self.qqZZ_chan
        dict['ggZZ'] = self.ggZZ_chan
        dict['zjets'] = self.zjets_chan
        dict['ttbar'] = self.ttbar_chan
        dict['zbb'] = self.zbb_chan
        
	dict['zjetsKappaLow'] = float(self.zjetsKappaLow)
        dict['zjetsKappaHigh'] = float(self.zjetsKappaHigh)
        
        dict['lumiUnc'] = self.lumiUnc
        dict['lumiUncCorrelated_16_17_18'] = self.lumiUncCorrelated161718
	dict['lumiUncCorrelated_17_18'] = self.lumiUncCorrelated1718
	
	dict['mu_signal_low'] = float(self.mu_signal_low)
        dict['mu_ggzz_low'] = float(self.mu_ggzz_low) 
        dict['mu_qqzz_low'] = float(self.mu_qqzz_low)
        dict['ele_signal_low'] = float(self.ele_signal_low) 
        dict['ele_ggzz_low'] = float(self.ele_ggzz_low) 
        dict['ele_qqzz_low'] = float(self.ele_qqzz_low)

	dict['mu_signal_high'] = float(self.mu_signal_high)
        dict['mu_ggzz_high'] = float(self.mu_ggzz_high) 
        dict['mu_qqzz_high'] = float(self.mu_qqzz_high)
        dict['ele_signal_high'] = float(self.ele_signal_high) 
        dict['ele_ggzz_high'] = float(self.ele_ggzz_high) 
        dict['ele_qqzz_high'] = float(self.ele_qqzz_high)

        dict['CMS_zz4l_mean_m_sig'] = float(self.CMS_zz4l_mean_m_sig) 
        dict['CMS_zz4l_sigma_m_sig'] = float(self.CMS_zz4l_sigma_m_sig) 
        dict['CMS_zz4l_mean_e_sig'] = float(self.CMS_zz4l_mean_e_sig) 
        dict['CMS_zz4l_sigma_e_sig'] = float(self.CMS_zz4l_sigma_e_sig)

        return dict
