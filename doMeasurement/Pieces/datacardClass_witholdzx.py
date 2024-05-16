#! /usr/bin/env python
from scipy.special import erf
import ROOT
from array import array
import sys, os, pwd, commands
from subprocess import *
import optparse, shlex, re
import math
import time
from decimal import *
import json
from ROOT import *
sys.path.append('./include')
from systematicsClass import *

## ------------------------------------
##  card and workspace class
## ------------------------------------

class datacardClass:

    def __init__(self):
        
	self.ID_4mu = 1
        self.ID_4e  = 2
        self.ID_2e2mu = 3
	self.ID_2mu2e = 4 
        self.isFSR = True
        self.low_M = 105
        self.high_M = 140
    def loadIncludes(self):
        
        ROOT.gSystem.AddIncludePath('-I $ROOFITSYS/include/')
        ROOT.gSystem.AddIncludePath('-Iinclude/')
        #ROOT.gROOT.ProcessLine('.L include/tdrstyle.cc')
        ROOT.gSystem.Load('libRooFit')
        ROOT.gSystem.Load('libHiggsAnalysisCombinedLimit.so')
        ROOT.gSystem.Load('include/HiggsCSandWidth_cc.so')
        ROOT.gSystem.Load('include/HiggsCSandWidthSM4_cc.so')

    # return trueVar if testStatement else return falseVar
    def getVariable(self,trueVar,falseVar,testStatement):

        if (testStatement): 
            return trueVar
        else:
            return falseVar
   

    def histogramBinFix(self,hist,histname):

        dBinsX = hist.GetXaxis().GetNbins()
        dBinsY = hist.GetYaxis().GetNbins()
        dLowY = hist.GetYaxis().GetXmin()
        dHighY = hist.GetYaxis().GetXmax()

        xBins = array('d', [self.low_M]) # e.g. self.low_M =105, self.high_M=140
        nBinsNew = 0
        for nxbin in range(1,dBinsX+1):
            if self.low_M<hist.GetXaxis().GetBinLowEdge(nxbin):
                if self.high_M>hist.GetXaxis().GetBinLowEdge(nxbin):
                    xBins.append(hist.GetXaxis().GetBinLowEdge(nxbin))
                    nBinsNew+=1

        xBins.append(self.high_M)

        print xBins

        hist_fix = ROOT.TH2F(histname,histname,nBinsNew+1,xBins,dBinsY,dLowY,dHighY)

        print hist.GetXaxis().GetNbins(), hist.GetXaxis().GetXmin(), hist.GetXaxis().GetXmax()
        print hist.GetYaxis().GetNbins(), hist.GetYaxis().GetXmin(), hist.GetYaxis().GetXmax()
        print hist_fix.GetXaxis().GetNbins(), hist_fix.GetXaxis().GetXmin(), hist_fix.GetXaxis().GetXmax()
        print hist_fix.GetYaxis().GetNbins(), hist_fix.GetYaxis().GetXmin(), hist_fix.GetYaxis().GetXmax()

        for nybin in range(1,hist_fix.GetYaxis().GetNbins()+1):
            for nxbin in range(1,hist_fix.GetXaxis().GetNbins()+1):
                binnum = hist.FindBin(hist_fix.GetXaxis().GetBinCenter(nxbin),hist_fix.GetYaxis().GetBinCenter(nybin))
                binval = hist.GetBinContent(binnum)
                currentbin = hist_fix.FindBin(hist_fix.GetXaxis().GetBinCenter(nxbin),hist_fix.GetYaxis().GetBinCenter(nybin))
                hist_fix.SetBinContent(currentbin,binval)
        return hist_fix
 
 
    def makeCardsWorkspaces(self, theMH, theYear, theInputs, theTotCat, theCat, theis2D, theIncludingError, theType, theInputDir, theOutputDir, theVBF=False, theVBFcat=0, theMassOnly=True, theFFT=True,theInterp=True):

        ## ------------------ SETTINGS AND DECLARATIONS --------------- ##
        DEBUG = False
        
	self.mH = theMH						#float
        self.year = theYear					#string
        self.totcat = theTotCat					#int
	self.cat = theCat					#int
	self.is2D = theis2D					#bool
	self.bIncludingError = theIncludingError		#bool
        self.type_ = theType					#bool------------------------>update to string for reco refit bs bsrefit
	self.inputDir = theInputDir				#string
	self.outputDir = theOutputDir				#string
	self.bVBF = theVBF					#bool
	self.VBFcat = theVBFcat					#int	if theVBF is True, theVBFcat==0-->Untag, theVBFcat==1-->VBFtag
	self.MassOnly = theMassOnly				#bool
	self.FFT = theFFT
	if theInterp == 'False':
		self.Interp = False
	else:
		self.Interp = True

	self.lumi = theInputs['lumi']
	self.sqrts = theInputs['sqrts']
	self.channel = theInputs['decayChannel']

	self.sigMorph = False#theInputs['useCMS_zz4l_sigMELA']
        self.bkgMorph = False#theInputs['useCMS_zz4l_bkgMELA']

        self.all_chan = theInputs['all']
        self.ggH_chan = theInputs['ggH']
        self.qqH_chan = theInputs['qqH']
        self.WH_chan = theInputs['WH']
        self.ZH_chan = theInputs['ZH']
        self.ttH_chan = theInputs['ttH']
        self.qqZZ_chan = theInputs['qqZZ']
        self.ggZZ_chan = theInputs['ggZZ']
        self.zjets_chan = theInputs['zjets']
        self.ttbar_chan = theInputs['ttbar']
        

        ## ----------------------- SET PLOTTING STYLE ----------------- ## 
        #ROOT.setTDRStyle(True)
        #ROOT.gStyle.SetPalette(1)
        #ROOT.gStyle.SetPadLeftMargin(0.16)        
        

	##------------------------ CONTRAL VARIABLES ------------------ ##
	name = ''						#a key string, it will be used, together with variables below, to control what info need to be pumped into datacard or workspace
	#final state name
	if (self.channel == self.ID_4mu): self.fs_name = '4mu'
        elif (self.channel == self.ID_4e): self.fs_name = '4e'
        elif (self.channel == self.ID_2e2mu): self.fs_name = '2e2mu'
	elif (self.channel == self.ID_2mu2e): self.fs_name = '2mu2e'
        else: print "Input Error: Unknown channel! (4mu = 1, 4e = 2, 2e2mu = 3, 2mu2e = 4)"
	#vbf tag name
	if(not self.bVBF): self.tag_name = 'notag'
	elif(self.bVBF and self.VBFcat == 0): self.tag_name = 'untag'
	elif(self.bVBF and self.VBFcat == 1): self.tag_name = 'vbftag'
	else: print 'Input Error: Unknown tag name! (notag, untag, vbftag)'
	#type name(reco, refit, bs, bsrefit)
	if(self.type_ == 'refit'): self.type_name = 'refit'
	elif(self.type_ == 'reco' ): self.type_name = 'reco'
	elif(self.type_ == 'bs'): self.type_name = 'bs'
	elif(self.type_ == 'bsrefit'): self.type_name = 'bsrefit'
	else: print 'Input Error: Unknown tyep name! (reco, refit, bs, bsrefit)'
	#mass err category name
	self.cate_name = str(self.cat)
	self.totcate_name = str(self.totcat)
	
	##------------------------ inputs params and templates -------- ##
	with open(self.inputDir + '/' + self.year + '_' + self.totcate_name + 'categs_' +  self.type_name + '_' + self.tag_name + '_params.json','r') as f:
		params = json.load(f)
	fi = ROOT.TFile.Open(self.inputDir + '/' + self.year + '_' + self.totcate_name + 'categs_' + self.type_name + '_' + self.tag_name + '_templates.root','READ')
	print 'Loading completed..................'

        ## ------------------------- SYSTEMATICS CLASSES ----------------------------- ##
    
        systematics = systematicsClass( self.mH, False, self.isFSR, theInputs)
        
	## ------------------------- GENERAL VARIABLES ------------------------------- ##					#general variables represent the same things across different models, they don't have unique name
        bins = 700#the old number is 700
        #if(self.bIncludingError) :
        #  bins = 1000

        CMS_zz4l_mass_name = "CMS_zz4l_mass"											#first dimentional
        CMS_zz4l_mass = ROOT.RooRealVar(CMS_zz4l_mass_name,CMS_zz4l_mass_name,self.low_M,self.high_M)    
        CMS_zz4l_mass.setBins(bins);
	CMS_zz4l_mass.setBins(20000,"cache")

        	
        merrVarName = "CMS_zz4l_massErr"											#second dimentional
        Template = fi.Get('ggh_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_dm')
        Template.SetDirectory(0)
	dmBins = Template.GetXaxis().GetNbins()
        dmLow = Template.GetXaxis().GetXmin()
        dmHigh = Template.GetXaxis().GetXmax()
        MassErr = ROOT.RooRealVar(merrVarName,merrVarName,dmLow,dmHigh)
        MassErr.setBins(dmBins)

        discVarName = "melaLD"													#third dimentional
        sigTemplate_beforeFix = fi.Get('ggh_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_kd')
        sigTemplate_beforeFix.SetDirectory(0)
	sigTemplate_Up_beforeFix = fi.Get('ggh_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_kdup')
        sigTemplate_Up_beforeFix.SetDirectory(0)
	sigTemplate_Down_beforeFix = fi.Get('ggh_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_kddn')
        sigTemplate_Down_beforeFix.SetDirectory(0)

	sigTemplate = self.histogramBinFix(sigTemplate_beforeFix, sigTemplate_beforeFix.GetName())
        sigTemplate_Up = self.histogramBinFix(sigTemplate_Up_beforeFix, sigTemplate_Up_beforeFix.GetName())
        sigTemplate_Down = self.histogramBinFix(sigTemplate_Down_beforeFix, sigTemplate_Down_beforeFix.GetName())
        
	dBins = sigTemplate.GetYaxis().GetNbins()
        dLow = sigTemplate.GetYaxis().GetXmin()
        dHigh = sigTemplate.GetYaxis().GetXmax()
        D = ROOT.RooRealVar(discVarName,discVarName,dLow,dHigh)
        D.setBins(dBins)
        print "discVarName ", discVarName, " dLow ", dLow, " dHigh ", dHigh, " dBins ", dBins


        myCSW = HiggsCSandWidth()
        self.widthHVal =  myCSW.HiggsWidth(0,self.mH)
	print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
	print self.widthHVal
	print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
	self.LUMI = ROOT.RooRealVar("LUMI_{0:.0f}".format(self.sqrts),"LUMI_{0:.0f}".format(self.sqrts),self.lumi)
        self.LUMI.setConstant(True)

        #POI
        self.MH = ROOT.RooRealVar("MH","MH",self.mH)
        self.MH.setConstant(True)	#right now, it is const and can be re-set to be float again within combinelimit tool
        self.HiggsDecayWidth = ROOT.RooRealVar("HiggsDecayWidth","HiggsDecayWidth",self.widthHVal)
        self.HiggsDecayWidth.setConstant(True)

        self.low_M = 105.0
        self.high_M = 140.0
        
	##-------------------------- NUISANCES VARIABLES ----------------------------- ##					#different models can and need to share same nuisances, so they aren't assigned with distinc name
        name = "CMS_zz4l_mean_e_sig"
        CMS_zz4l_mean_e_sig = ROOT.RooRealVar(name,"CMS_zz4l_mean_e_sig",0.0,-10.0,10.0)
        name = "CMS_zz4l_mean_e_err_{0}_{1:.0f}".format(self.channel,self.sqrts)
        CMS_zz4l_mean_e_err = ROOT.RooRealVar(name,"CMS_zz4l_mean_e_err",float(theInputs['CMS_zz4l_mean_e_sig']))
        name = "CMS_zz4l_sigma_e_sig"
        CMS_zz4l_sigma_e_sig = ROOT.RooRealVar(name,"CMS_zz4l_sigma_e_sig",0.0,-0.99,0.99)

        name = "CMS_zz4l_mean_m_sig"
        CMS_zz4l_mean_m_sig = ROOT.RooRealVar(name,"CMS_zz4l_mean_sig",0.0,-10.0,10.0)
        name = "CMS_zz4l_mean_m_err_{0}_{1:.0f}".format(self.channel,self.sqrts)
        CMS_zz4l_mean_m_err = ROOT.RooRealVar(name,"CMS_zz4l_mean_m_err",float(theInputs['CMS_zz4l_mean_m_sig']))
        name = "CMS_zz4l_sigma_m_sig"
        CMS_zz4l_sigma_m_sig = ROOT.RooRealVar(name,"CMS_zz4l_sigma_m_sig",0.0,-0.99,0.99)        

        name = "CMS_zz4l_alpha2_{0}_{1:.0f}".format(self.channel,self.sqrts)
        CMS_zz4l_alpha2 = ROOT.RooRealVar(name,"CMS_zz4l_alpha2",0.0,-0.99,0.99)
        name = "CMS_zz4l_n2_sig_{0}_{1:.0f}".format(self.channel,self.sqrts)
        CMS_zz4l_n2 = ROOT.RooRealVar(name,"CMS_zz4l_n2",0.0,-0.99,0.99)
        name = "CMS_zz4l_alpha_{0}_{1:.0f}".format(self.channel,self.sqrts)
        CMS_zz4l_alpha = ROOT.RooRealVar(name,"CMS_zz4l_alpha",0.0,-0.99,0.99)
        name = "CMS_zz4l_n_sig_{0}_{1:.0f}".format(self.channel,self.sqrts)
        CMS_zz4l_n = ROOT.RooRealVar(name,"CMS_zz4l_n",0.0,-0.99,0.99)
    
        CMS_zz4l_mean_e_sig.setVal(0)
        CMS_zz4l_mean_e_err.setConstant(kTRUE)											#electron scale uncertainty, in current model, it is a const.
        CMS_zz4l_sigma_e_sig.setVal(0)
        CMS_zz4l_mean_m_sig.setVal(0)
        CMS_zz4l_mean_m_err.setConstant(kTRUE)											#muon scale uncertainty, in current model, it is a const
        CMS_zz4l_sigma_m_sig.setVal(0)
        CMS_zz4l_alpha.setVal(0)
        CMS_zz4l_n.setVal(0)
        CMS_zz4l_alpha2.setVal(0)
        CMS_zz4l_n2.setVal(0)

        print "signal shape nuisances"
        print "mean_e_sig ", CMS_zz4l_mean_e_sig.getVal()
        print "mean_e_err ", CMS_zz4l_mean_e_err.getVal()
        print "sigma_e ", CMS_zz4l_sigma_e_sig.getVal()
        print "mean_m_sig ",CMS_zz4l_mean_m_sig.getVal()
        print "mean_m_err ", CMS_zz4l_mean_m_err.getVal()
        print "sigma_m ", CMS_zz4l_sigma_m_sig.getVal()
        print "alpha ", CMS_zz4l_alpha.getVal()
        print "n ", CMS_zz4l_n.getVal()
        print "alpha2 ", CMS_zz4l_alpha2.getVal()
        print "n2 ", CMS_zz4l_n2.getVal()

	## -------------------- Variables for Shapes -------------------- ##
        ## ------------------------------------------ sig
	rfv_n_CB = ROOT.RooFormulaVar()
        rfv_alpha_CB = ROOT.RooFormulaVar()
        rfv_n2_CB = ROOT.RooFormulaVar()
        rfv_alpha2_CB = ROOT.RooFormulaVar()
        rfv_sigma_CB = ROOT.RooFormulaVar()
        rfv_mean_CB = ROOT.RooFormulaVar()
	
	name = ''
        name = "n_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)				#type name is not here, because diff type(reco, refit, bs, bsrefit) never be combined			#
        rfv_n_CB = ROOT.RooFormulaVar(name,"("+params[name]+")*(1+@1)",ROOT.RooArgList(self.MH,CMS_zz4l_n))

        name = "alpha_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_alpha_CB = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))
        
	if not self.MassOnly and not self.FFT:
		name = "alpha_approx_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		alpha_approx = ROOT.RooFormulaVar(name,"("+params[name]+")",ROOT.RooArgList(self.MH))

        name = "n2_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_n2_CB = ROOT.RooFormulaVar(name,"("+params[name]+")",ROOT.RooArgList(self.MH))#, CMS_zz4l_n2))

        name = "alpha2_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_alpha2_CB = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

	if not self.MassOnly and not self.FFT:
		name = "alpha2_approx_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		alpha2_approx = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

        name = "whp0_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_WHp0 = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

        name = "whp1_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_WHp1 = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

        name = "whfrac_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_WHfrac = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

        name = "zhp0_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_ZHp0 = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

        name = "zhp1_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_ZHp1 = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

        name = "zhfrac_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfv_ZHfrac = ROOT.RooFormulaVar(name,"("+params[name]+")", ROOT.RooArgList(self.MH))

	### mean of CB
        name = "mean_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        if (self.channel == self.ID_4mu) :
             CMS_zz4l_mean_sig_NoConv = ROOT.RooFormulaVar(name,"("+params[name]+")"+"+@0*@1*@2", ROOT.RooArgList(self.MH, CMS_zz4l_mean_m_sig, CMS_zz4l_mean_m_err))
        elif (self.channel == self.ID_4e) :
             CMS_zz4l_mean_sig_NoConv = ROOT.RooFormulaVar(name,"("+params[name]+")"+"+@0*@1*@2", ROOT.RooArgList(self.MH, CMS_zz4l_mean_e_sig, CMS_zz4l_mean_e_err))
        elif (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ) :
             CMS_zz4l_mean_sig_NoConv = ROOT.RooFormulaVar(name,"("+params[name]+")"+"+((@0*@1*@3+@0*@2*@4))/2.0", ROOT.RooArgList(self.MH, CMS_zz4l_mean_m_sig, CMS_zz4l_mean_e_sig, CMS_zz4l_mean_m_err, CMS_zz4l_mean_e_err))        
	             
        if (self.channel == self.ID_4mu) :
            CMS_zz4l_mean_sig_Conv = ROOT.RooFormulaVar(name,"("+params[name]+")"+"-@0+@0*@1*@2", ROOT.RooArgList(self.MH, CMS_zz4l_mean_m_sig, CMS_zz4l_mean_m_err))
        elif (self.channel == self.ID_4e) :
            CMS_zz4l_mean_sig_Conv = ROOT.RooFormulaVar(name,"("+params[name]+")"+"-@0+@0*@1*@2", ROOT.RooArgList(self.MH, CMS_zz4l_mean_e_sig, CMS_zz4l_mean_e_err))
        elif (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ) :
            CMS_zz4l_mean_sig_Conv = ROOT.RooFormulaVar(name,"("+params[name]+")"+"-@0+(@0*@1*@3+@0*@2*@4)/2.0", ROOT.RooArgList(self.MH, CMS_zz4l_mean_m_sig, CMS_zz4l_mean_e_sig, CMS_zz4l_mean_m_err, CMS_zz4l_mean_e_err))
        
	name_approx = "mean_approx_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)##############################################
	if (not self.MassOnly and not self.FFT and self.channel == self.ID_4mu) :
		CMS_zz4l_mean_sig_Approx = ROOT.RooFormulaVar(name_approx,"("+params[name_approx]+")"+"+(@0*@1)",ROOT.RooArgList(self.MH,CMS_zz4l_mean_m_sig))
	if (not self.MassOnly and not self.FFT and self.channel == self.ID_4e) :
		CMS_zz4l_mean_sig_Approx = ROOT.RooFormulaVar(name_approx,"("+params[name_approx]+")"+"+(@0*@1)",ROOT.RooArgList(self.MH,CMS_zz4l_mean_e_sig))
	elif (not self.MassOnly and not self.FFT and (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e)) :
		CMS_zz4l_mean_sig_Approx = ROOT.RooFormulaVar(name_approx,"("+params[name_approx]+")"+"+(0.5*(@0*@1+@0*@2))",ROOT.RooArgList(self.MH,CMS_zz4l_mean_m_sig,CMS_zz4l_mean_e_sig))

        # sigma of CB 
	name = "sigma_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	if (self.channel == self.ID_4mu) :
            rfv_sigma_CB = ROOT.RooFormulaVar(name,"("+params[name]+")"+"*(1+@1)", ROOT.RooArgList(self.MH, CMS_zz4l_sigma_m_sig))
        elif (self.channel == self.ID_4e) :
            rfv_sigma_CB = ROOT.RooFormulaVar(name,"("+params[name]+")"+"*(1+@1)", ROOT.RooArgList(self.MH, CMS_zz4l_sigma_e_sig))
        elif (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ) :
            rfv_sigma_CB = ROOT.RooFormulaVar(name,"("+params[name]+")"+"*TMath::Sqrt((1+@1)*(1+@2))", ROOT.RooArgList(self.MH, CMS_zz4l_sigma_m_sig, CMS_zz4l_sigma_e_sig))
	
	name_approx = "sigma_approx_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)############################################
	if (not self.MassOnly and not self.FFT and self.channel == self.ID_4mu) :
		rfv_sigma_approx = ROOT.RooFormulaVar(name,"("+params[name_approx]+")"+"*(1+@1)",ROOT.RooArgList(self.MH,CMS_zz4l_sigma_m_sig))
	if (not self.MassOnly and not self.FFT and self.channel == self.ID_4e) :
		rfv_sigma_approx = ROOT.RooFormulaVar(name,"("+params[name_approx]+")"+"*(1+@1)",ROOT.RooArgList(self.MH,CMS_zz4l_sigma_e_sig))
	elif (not self.MassOnly and not self.FFT and (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e)) :
		rfv_sigma_approx = ROOT.RooFormulaVar(name,"("+params[name_approx]+")"+"*(1+0.5*(@1 + @2))", ROOT.RooArgList(self.MH,CMS_zz4l_sigma_m_sig,CMS_zz4l_sigma_e_sig))

        print "fs ", self.channel
        print "n_CB ", rfv_n_CB.getVal()
        print "alpha_CB ", rfv_alpha_CB.getVal()
        print "n2_CB ", rfv_n2_CB.getVal()
        print "alpha2_CB ", rfv_alpha2_CB.getVal()
        print "sigma_CB ", rfv_sigma_CB.getVal()
        print "mean_NoConv ", CMS_zz4l_mean_sig_NoConv.getVal()
        
        ## -------------------------------------------------------------- bkgs


	##------------------------------- MassErr ------------------------------ ##
	rfv_MassErr = ROOT.RooFormulaVar()
	if dmHigh >= 1 :   # use absolute mass error as observable
        	if (self.channel == self.ID_4mu) :
        	    rfv_MassErr = ROOT.RooFormulaVar("rfv_MassErr_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"1.0*@0*(1+@1)",ROOT.RooArgList(MassErr,CMS_zz4l_sigma_m_sig))	#muon reslostion unc ~ 20%
        	elif (self.channel == self.ID_4e) :
        	    rfv_MassErr = ROOT.RooFormulaVar("rfv_MassErr_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"1.0*@0*(1+@1)",ROOT.RooArgList(MassErr,CMS_zz4l_sigma_e_sig))
        	elif (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ) :
        	    rfv_MassErr = ROOT.RooFormulaVar("rfv_MassErr_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"1.0*@0*TMath::Sqrt((1+@1)*(1+@2))",ROOT.RooArgList(MassErr,CMS_zz4l_sigma_m_sig,CMS_zz4l_sigma_e_sig))
	else: 		   # use relative mass error as observable   --> need to calculate the absolute mass error
        	if (self.channel == self.ID_4mu) :
        	    rfv_MassErr = ROOT.RooFormulaVar("rfv_MassErr_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"@1*@0*(1+@2)",ROOT.RooArgList(MassErr, self.MH, CMS_zz4l_sigma_m_sig))
        	elif (self.channel == self.ID_4e) :
        	    rfv_MassErr = ROOT.RooFormulaVar("rfv_MassErr_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"@1*@0*(1+@2)",ROOT.RooArgList(MassErr, self.MH, CMS_zz4l_sigma_e_sig))
        	elif (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ) :
        	    rfv_MassErr = ROOT.RooFormulaVar("rfv_MassErr_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"@1*@0*TMath::Sqrt((1+@2)*(1+@3))",ROOT.RooArgList(MassErr, self.MH, CMS_zz4l_sigma_m_sig,CMS_zz4l_sigma_e_sig))
        
        n1_ = 3
	n2_ = 3
	name = "theta1_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	theta1 = ROOT.RooRealVar(name,name,2.3)
	name = "theta2_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	theta2 = ROOT.RooRealVar(name,name,2.3)

	### sig
        print "pdferr sig"
        DmSigTemplate = fi.Get('ggh_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_dm')
	DmSigTemplate.SetDirectory(0)
        TemplateName = "HistPdf_SigDm_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	DmSigTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(MassErr),DmSigTemplate)
        name = "pdfErrS_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        pdfErrS = ROOT.RooHistPdf(name,name,ROOT.RooArgSet(MassErr),DmSigTempDataHist)
        
	### qqzz
        print "pdferr qqZZ"
        DmqqzzTemplate = fi.Get('qqzz_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_dm')
	DmqqzzTemplate.SetDirectory(0)
        TemplateName = "HistPdf_qqzzDm_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	DmqqzzTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(MassErr),DmqqzzTemplate)
        name = "pdfErr_qqzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        pdfErrqqZZ = ROOT.RooHistPdf(name,name,ROOT.RooArgSet(MassErr),DmqqzzTempDataHist)

        ### ggzz
        print "pdferr ggZZ"
        DmggzzTemplate = fi.Get('ggzz_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_dm')
	DmqqzzTemplate.SetDirectory(0)
        TemplateName = "HistPdf_ggzzDm_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        DmggzzTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(MassErr),DmggzzTemplate)
        name = "pdfErr_ggzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        pdfErrggZZ = ROOT.RooHistPdf(name,name,ROOT.RooArgSet(MassErr),DmggzzTempDataHist)

        ##Z+X using template										#something need to be considered
        print "pdferr ZX"
        DmtemplateBkgName = 'commoninputs/pdfErrZX_'+self.fs_name+'.root'
        DmzjetsTempFile = ROOT.TFile(DmtemplateBkgName)
	tmp_name = self.fs_name
	if self.fs_name == '2mu2e':
		tmp_name = '2e2mu'
        DmzjetsTemplate = DmzjetsTempFile.Get("pdfErrZX_"+tmp_name)
        TemplateName = "HistPdf_zjetsDm_{0}_{1}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        DmzjetsTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(MassErr),DmzjetsTemplate)
        name = "pdfErr_zjets_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        pdfErrZX = ROOT.RooHistPdf(name,name,ROOT.RooArgSet(MassErr),DmzjetsTempDataHist)

	name = "BreitWigner_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name
	HiggsBreitWigner = ROOT.RooBreitWigner(name,name,CMS_zz4l_mass,self.MH,self.HiggsDecayWidth)

	## --------------------------------------------- SHAPE FUNCTIONS ------------------------------------------ ## 
        ## --------------------------------------------------------------sigs 
	## --------------------------------------------------------------GGH
        name = "signalCB_ggH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        if (self.MassOnly) : signalCB_ggH = ROOT.RooDoubleCB(name, name, CMS_zz4l_mass, CMS_zz4l_mean_sig_NoConv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError),rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)       
        if (not self.MassOnly and self.FFT) : 
		DetectorReso_ggH = ROOT.RooDoubleCB(name+'_reso',name,CMS_zz4l_mass, CMS_zz4l_mean_sig_Conv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError), rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
		signalCB_ggH = ROOT.RooFFTConvPdf(name,name,CMS_zz4l_mass,HiggsBreitWigner,DetectorReso_ggH,2)
	if (not self.MassOnly and not self.FFT) :
		signalCB_ggH = ROOT.RooaDoubleCBxBW(name,name,CMS_zz4l_mass, CMS_zz4l_mean_sig_Approx, self.getVariable(rfv_MassErr,rfv_sigma_approx, self.bIncludingError), alpha_approx, alpha2_approx, self.MH, self.HiggsDecayWidth, n1_, n2_, theta1, theta2, False)
	## --------------------------------------------------------------VBF	 
	name = "signalCB_qqH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        if (self.MassOnly) : signalCB_VBF = ROOT.RooDoubleCB(name, name, CMS_zz4l_mass, CMS_zz4l_mean_sig_NoConv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError),rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
        if (not self.MassOnly and self.FFT) :
		DetectorReso_VBF = ROOT.RooDoubleCB(name+'_reso',name,CMS_zz4l_mass, CMS_zz4l_mean_sig_Conv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError), rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
		signalCB_VBF = ROOT.RooFFTConvPdf(name,name,CMS_zz4l_mass,HiggsBreitWigner,DetectorReso_VBF,2)
	if (not self.MassOnly and not self.FFT) :
		signalCB_VBF = ROOT.RooaDoubleCBxBW(name,name, CMS_zz4l_mass, CMS_zz4l_mean_sig_Approx, self.getVariable(rfv_MassErr,rfv_sigma_approx, self.bIncludingError), alpha_approx, alpha2_approx, self.MH, self.HiggsDecayWidth, n1_, n2_, theta1, theta2, False)
	## --------------------------------------------------------------WH
        if (self.MassOnly) : 
		name = "resWH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		resWH = ROOT.RooDoubleCB(name, name, CMS_zz4l_mass, CMS_zz4l_mean_sig_NoConv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError),rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
		name = "nonresWH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		nonresWH = ROOT.RooLandau(name, name, CMS_zz4l_mass, rfv_WHp0, rfv_WHp1)
		name = "signalCB_WH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		signalCB_WH = ROOT.RooAddPdf(name, name, resWH, nonresWH, rfv_WHfrac)
        if (not self.MassOnly and self.FFT) :
		name = "resWH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		DetectorReso_WH = ROOT.RooDoubleCB(name+'_reso',name,CMS_zz4l_mass, CMS_zz4l_mean_sig_Conv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError), rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
        	resWH = ROOT.RooFFTConvPdf(name,name,CMS_zz4l_mass,HiggsBreitWigner,DetectorReso_WH,2)
		name = "nonresWH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		nonresWH = ROOT.RooLandau(name, name, CMS_zz4l_mass, rfv_WHp0, rfv_WHp1)
		name = "signalCB_WH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		signalCB_WH = ROOT.RooAddPdf(name, name, resWH, nonresWH, rfv_WHfrac)
	if (not self.MassOnly and not self.FFT) :
		name = "resWH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		resWH = ROOT.RooaDoubleCBxBW(name,name,CMS_zz4l_mass, CMS_zz4l_mean_sig_Approx, self.getVariable(rfv_MassErr,rfv_sigma_approx, self.bIncludingError), alpha_approx, alpha2_approx, self.MH, self.HiggsDecayWidth, n1_, n2_, theta1, theta2, False)
		name = "nonresWH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		nonresWH = ROOT.RooLandau(name, name, CMS_zz4l_mass, rfv_WHp0, rfv_WHp1)
		name = "signalCB_WH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		signalCB_WH = ROOT.RooAddPdf(name, name, resWH, nonresWH, rfv_WHfrac)
	## --------------------------------------------------------------ZH
        if (self.MassOnly) : 
		name = "resZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		resZH = ROOT.RooDoubleCB(name, name, CMS_zz4l_mass, CMS_zz4l_mean_sig_NoConv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError), rfv_alpha_CB, rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
		name = "nonresZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		nonresZH = ROOT.RooLandau(name, name, CMS_zz4l_mass, rfv_ZHp0, rfv_ZHp1)
		name = "signalCB_ZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		signalCB_ZH = ROOT.RooAddPdf(name, name, resZH, nonresZH, rfv_ZHfrac)
        if (not self.MassOnly and self.FFT) :
		name = "resZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		DetectorReso_ZH = ROOT.RooDoubleCB(name+'_reso',name,CMS_zz4l_mass, CMS_zz4l_mean_sig_Conv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError), rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
        	resZH = ROOT.RooFFTConvPdf(name,name,CMS_zz4l_mass,HiggsBreitWigner,DetectorReso_ZH,2)
		name = "nonresZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		nonresZH = ROOT.RooLandau(name, name, CMS_zz4l_mass, rfv_ZHp0, rfv_ZHp1)
		name = "signalCB_ZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		signalCB_ZH = ROOT.RooAddPdf(name, name, resZH, nonresZH, rfv_ZHfrac)
	if (not self.MassOnly and not self.FFT) :
		name = "resZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		resZH = ROOT.RooaDoubleCBxBW(name,name, CMS_zz4l_mass, CMS_zz4l_mean_sig_Approx, self.getVariable(rfv_MassErr,rfv_sigma_approx, self.bIncludingError), alpha_approx, alpha2_approx, self.MH, self.HiggsDecayWidth, n1_, n2_, theta1, theta2, False)
		name = "nonresZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		nonresZH = ROOT.RooLandau(name, name, CMS_zz4l_mass, rfv_ZHp0, rfv_ZHp1)
		name = "signalCB_ZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
		signalCB_ZH = ROOT.RooAddPdf(name, name, resZH, nonresZH, rfv_ZHfrac)
	## --------------------------------------------------------------ttH
	name = "signalCB_ttH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        if (self.MassOnly) : signalCB_ttH = ROOT.RooDoubleCB(name, name, CMS_zz4l_mass, CMS_zz4l_mean_sig_NoConv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError),rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
	if (not self.MassOnly and self.FFT) :
		DetectorReso_ttH = ROOT.RooDoubleCB(name+'_reso',name,CMS_zz4l_mass, CMS_zz4l_mean_sig_Conv, self.getVariable(rfv_MassErr,rfv_sigma_CB, self.bIncludingError), rfv_alpha_CB,rfv_n_CB, rfv_alpha2_CB, rfv_n2_CB)
		signalCB_ttH = ROOT.RooFFTConvPdf(name,name,CMS_zz4l_mass,HiggsBreitWigner,DetectorReso_ttH,2)
	if (not self.MassOnly and not self.FFT) :
		signalCB_ttH = ROOT.RooaDoubleCBxBW(name, name, CMS_zz4l_mass, CMS_zz4l_mean_sig_Approx, self.getVariable(rfv_MassErr,rfv_sigma_approx, self.bIncludingError), alpha_approx, alpha2_approx, self.MH, self.HiggsDecayWidth, n1_, n2_, theta1, theta2, False)
	
	if (self.FFT and not self.MassOnly):
		signalCB_ggH.setBufferFraction(0.2);
		signalCB_VBF.setBufferFraction(0.2);
		resWH.setBufferFraction(0.2);
		resZH.setBufferFraction(0.2);
		signalCB_ttH.setBufferFraction(0.2);

	##---------------------------------------------------------------------------bkgs
        ## qqZZ contribution
        name = "qqzz_chepol1_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        qqzz_c0 = ROOT.RooRealVar(name,name,float(params[name]))
        name = "qqzz_chepol2_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        qqzz_c1 = ROOT.RooRealVar(name,name,float(params[name]))
        name = "qqzz_chepol3_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        qqzz_c2 = ROOT.RooRealVar(name,name,float(params[name]))
        name = "bkg_qqzzTmp_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkg_qqzz = ROOT.RooBernstein(name,name,CMS_zz4l_mass,RooArgList(qqzz_c0,qqzz_c1,qqzz_c2))

        ## ggZZ contribution
        name = "ggzz_chepol1_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        ggzz_c0 = ROOT.RooRealVar(name,name,float(params[name]))
        name = "ggzz_chepol2_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        ggzz_c1 = ROOT.RooRealVar(name,name,float(params[name]))
        name = "ggzz_chepol3_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        ggzz_c2 = ROOT.RooRealVar(name,name,float(params[name]))
        name = "bkg_ggzzTmp_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkg_ggzz = ROOT.RooBernstein(name,name,CMS_zz4l_mass,RooArgList(ggzz_c0,ggzz_c1,ggzz_c2))

        ## Reducible backgrounds ##							#they are not imported from outside
        bkg_zjets = ROOT.RooGenericPdf();
        print "zjets mass shape"
        if self.type_name == 'reco' or self.type_name == 'bs':
            if (self.channel == self.ID_4mu):
                p0_zjets_4mu = ROOT.RooRealVar("p0_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,"p0_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,130.4)
                p1_zjets_4mu = ROOT.RooRealVar("p1_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,"p1_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,15.6)
		name = "landau_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name
		landau_zjets_4mu = ROOT.RooFormulaVar(name,"TMath::Landau(@0,@1,@2)",RooArgList(CMS_zz4l_mass,p0_zjets_4mu,p1_zjets_4mu))
		name = "bkg_zjetsTMP_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name
		bkg_zjets = ROOT.RooGenericPdf(name,"@0", ROOT.RooArgList(landau_zjets_4mu) )

            if (self.channel == self.ID_4e):
                p0_zjets_4e = ROOT.RooRealVar("p0_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p0_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,141.9)
                p1_zjets_4e = ROOT.RooRealVar("p1_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p1_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,21.3)
                p2_zjets_4e = ROOT.RooRealVar("p2_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p2_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,7.06)
                p3_zjets_4e = ROOT.RooRealVar("p3_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p3_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,-0.00497)
		name = "ladau_zjetsTMP_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name
                landau_zjets_4e = ROOT.RooFormulaVar(name,"TMath::Landau(@0,@1,@2)",ROOT.RooArgList(CMS_zz4l_mass,p0_zjets_4e,p1_zjets_4e))
		name = "bkg_zjetsTMP_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name
                bkg_zjets = ROOT.RooGenericPdf(name,"@0*(1+TMath::Exp(@1+@2*@3))", ROOT.RooArgList(landau_zjets_4e, p2_zjets_4e, p3_zjets_4e,CMS_zz4l_mass) )

            if (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ):
                p0_zjets_2e2mu = ROOT.RooRealVar("p0_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p0_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,131.1)
                p1_zjets_2e2mu = ROOT.RooRealVar("p1_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p1_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,18.1)
                p2_zjets_2e2mu = ROOT.RooRealVar("p2_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p2_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,0.45)
                p3_zjets_2e2mu = ROOT.RooRealVar("p3_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p3_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,133.8)
                p4_zjets_2e2mu = ROOT.RooRealVar("p4_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p4_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,18.9)
                p5_zjets_2e2mu = ROOT.RooRealVar("p5_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p5_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,0.55)
		name = "landau_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name
                landau_zjets_2e2mu = ROOT.RooFormulaVar(name,"TMath::Landau(@0,@1,@2)*@3 + TMath::Landau(@0,@4,@5)*@6",RooArgList(CMS_zz4l_mass,p0_zjets_2e2mu, p1_zjets_2e2mu, p2_zjets_2e2mu, p3_zjets_2e2mu, p4_zjets_2e2mu, p5_zjets_2e2mu))
		name = "bkg_zjetsTMP_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name
                bkg_zjets = ROOT.RooGenericPdf(name,"@0", ROOT.RooArgList(landau_zjets_2e2mu))

        if self.type_name == 'refit' or self.type_name == 'bsrefit':
            if (self.channel == self.ID_4mu):
                p0_zjets_4mu = ROOT.RooRealVar("p0_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,"p0_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,134.1)
                p1_zjets_4mu = ROOT.RooRealVar("p1_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,"p1_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name,21.01)
                name = "landau_zjets_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name
		landau_zjets_4mu = ROOT.RooFormulaVar(name,"TMath::Landau(@0,@1,@2)",RooArgList(CMS_zz4l_mass,p0_zjets_4mu,p1_zjets_4mu))
		name = "bkg_zjetsTMP_"+self.year+"_4mu_"+self.cate_name+"_"+self.tag_name
                bkg_zjets = ROOT.RooGenericPdf(name,"@0", ROOT.RooArgList(landau_zjets_4mu) )

            if (self.channel == self.ID_4e):
                p0_zjets_4e = ROOT.RooRealVar("p0_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p0_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,141.9)
                p1_zjets_4e = ROOT.RooRealVar("p1_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p1_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,21.3)
                p2_zjets_4e = ROOT.RooRealVar("p2_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p2_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,7.06)
                p3_zjets_4e = ROOT.RooRealVar("p3_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,"p3_zjets_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name,-0.00497)
                name = "ladau_zjetsTMP_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name
		landau_zjets_4e = ROOT.RooFormulaVar(name,"TMath::Landau(@0,@1,@2)",ROOT.RooArgList(CMS_zz4l_mass,p0_zjets_4e,p1_zjets_4e))
                name = "bkg_zjetsTMP_"+self.year+"_4e_"+self.cate_name+"_"+self.tag_name
		bkg_zjets = ROOT.RooGenericPdf(name,"@0*(1+TMath::Exp(@1+@2*@3))", ROOT.RooArgList(landau_zjets_4e, p2_zjets_4e, p3_zjets_4e, CMS_zz4l_mass) )

            if (self.channel == self.ID_2e2mu or self.channel == self.ID_2mu2e ):        
                p0_zjets_2e2mu = ROOT.RooRealVar("p0_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p0_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,142.8)
                p1_zjets_2e2mu = ROOT.RooRealVar("p1_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,"p1_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name,23.57)
                name = "landau_zjets_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name
		landau_zjets_2e2mu = ROOT.RooFormulaVar(name,"TMath::Landau(@0,@1,@2)",RooArgList(CMS_zz4l_mass,p0_zjets_2e2mu,p1_zjets_2e2mu))
                name = "bkg_zjetsTMP_"+self.year+"_"+self.fs_name+"_"+self.cate_name+"_"+self.tag_name
		bkg_zjets = ROOT.RooGenericPdf(name,"@0", ROOT.RooArgList(landau_zjets_2e2mu))
        
	##------------------------------------------------------------------------------------->1D shapes completed
	
	##-------------------------------------- EBE model -----------------------------------------------##
	##### signal EBE model
        name = "sig_ggHErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sig_ggHErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrS), ROOT.RooFit.Conditional(ROOT.RooArgSet(signalCB_ggH), ROOT.RooArgSet(CMS_zz4l_mass)))
        name = "sig_qqHErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sig_VBFErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrS), ROOT.RooFit.Conditional(ROOT.RooArgSet(signalCB_VBF), ROOT.RooArgSet(CMS_zz4l_mass)))
        name = "sig_WHErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sig_WHErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrS), ROOT.RooFit.Conditional(ROOT.RooArgSet(signalCB_WH), ROOT.RooArgSet(CMS_zz4l_mass)))
        name = "sig_ZHErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sig_ZHErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrS), ROOT.RooFit.Conditional(ROOT.RooArgSet(signalCB_ZH), ROOT.RooArgSet(CMS_zz4l_mass)))
        name = "sig_ttHErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sig_ttHErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrS), ROOT.RooFit.Conditional(ROOT.RooArgSet(signalCB_ttH), ROOT.RooArgSet(CMS_zz4l_mass)))
        
	##### bkg EBE model
        name = "bkg_qqzzErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	bkg_qqzzErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrqqZZ), ROOT.RooFit.Conditional(ROOT.RooArgSet(bkg_qqzz), ROOT.RooArgSet(CMS_zz4l_mass)));
        name = "bkg_ggzzErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	bkg_ggzzErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrggZZ), ROOT.RooFit.Conditional(ROOT.RooArgSet(bkg_ggzz), ROOT.RooArgSet(CMS_zz4l_mass)));
        name = "bkg_zjetsErr_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	bkg_zjetsErr = ROOT.RooProdPdf(name,name, ROOT.RooArgSet(pdfErrZX), ROOT.RooFit.Conditional(ROOT.RooArgSet(bkg_zjets), ROOT.RooArgSet(CMS_zz4l_mass)));
        
	##------------------------------------------------------------------------------------->EBE model completed
	    
        
	
	## -------------------------------------- MELA 2D PDFS ----------------------------------------- ##
        
        TemplateName = "sigTempDataHist_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),sigTemplate)
        TemplateName = "sigTempDataHist_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTempDataHist_Up = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),sigTemplate_Up)
        TemplateName = "sigTempDataHist_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTempDataHist_Down = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),sigTemplate_Down)			#various signals share the same KD templates

        TemplateName = "sigTemplatePdf_ggH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ggH = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist)
        TemplateName = "sigTemplatePdf_ggH_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ggH_Up = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Up)
        TemplateName = "sigTemplatePdf_ggH_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ggH_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Down)
        
        TemplateName = "sigTemplatePdf_VBF_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_VBF = ROOT.RooHistPdf(TemplateName,TemplateName,RooArgSet(CMS_zz4l_mass,D),sigTempDataHist)
        TemplateName = "sigTemplatePdf_VBF_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_VBF_Up = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Up)
        TemplateName = "sigTemplatePdf_VBF_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_VBF_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Down)
        
        TemplateName = "sigTemplatePdf_WH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_WH = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist)
        TemplateName = "sigTemplatePdf_WH_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_WH_Up = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Up)
        TemplateName = "sigTemplatePdf_WH_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_WH_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Down)
        
        TemplateName = "sigTemplatePdf_ZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ZH = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist)
        TemplateName = "sigTemplatePdf_ZH_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ZH_Up = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Up)
        TemplateName = "sigTemplatePdf_ZH_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ZH_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Down)
        
        TemplateName = "sigTemplatePdf_ttH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ttH = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist)
        TemplateName = "sigTemplatePdf_ttH_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ttH_Up = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Up)
        TemplateName = "sigTemplatePdf_ttH_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplatePdf_ttH_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),sigTempDataHist_Down)

        funcList_ggH = ROOT.RooArgList()  
        funcList_VBF = ROOT.RooArgList()
        funcList_WH  = ROOT.RooArgList()
        funcList_ZH  = ROOT.RooArgList()
        funcList_ttH = ROOT.RooArgList()

        funcList_ggH.add(sigTemplatePdf_ggH)
        funcList_VBF.add(sigTemplatePdf_VBF)
        funcList_WH.add(sigTemplatePdf_WH)
        funcList_ZH.add(sigTemplatePdf_ZH)
        funcList_ttH.add(sigTemplatePdf_ttH)

        if(self.sigMorph): 
           funcList_ggH.add(sigTemplatePdf_ggH_Up)
           funcList_ggH.add(sigTemplatePdf_ggH_Down)  
           funcList_VBF.add(sigTemplatePdf_VBF_Up)
           funcList_VBF.add(sigTemplatePdf_VBF_Down)  
           funcList_WH.add(sigTemplatePdf_WH_Up)
           funcList_WH.add(sigTemplatePdf_WH_Down)  
           funcList_ZH.add(sigTemplatePdf_ZH_Up)
           funcList_ZH.add(sigTemplatePdf_ZH_Down)  
           funcList_ttH.add(sigTemplatePdf_ttH_Up)
           funcList_ttH.add(sigTemplatePdf_ttH_Down)  
                
        morphSigVarName = "CMS_zz4l_sigMELA_{0:.0f}".format(self.channel)				## it is a nuisance
        alphaMorphSig = ROOT.RooRealVar(morphSigVarName,morphSigVarName,0,-20,20)
        if(self.sigMorph): alphaMorphSig.setConstant(False)
        else: alphaMorphSig.setConstant(True)

        morphVarListSig = ROOT.RooArgList()
        if(self.sigMorph): 
          morphVarListSig.add(alphaMorphSig)  								## just one morphing for all signal processes        

        TemplateName = "sigTemplateMorphPdf_ggH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplateMorphPdf_ggH = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,funcList_ggH,morphVarListSig,1.0,1)
        
        TemplateName = "sigTemplateMorphPdf_VBF_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplateMorphPdf_VBF = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,funcList_VBF,morphVarListSig,1.0,1)
        
        TemplateName = "sigTemplateMorphPdf_WH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplateMorphPdf_WH = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,funcList_WH,morphVarListSig,1.0,1)
        
        TemplateName = "sigTemplateMorphPdf_ZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplateMorphPdf_ZH = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,funcList_ZH,morphVarListSig,1.0,1)
        
        TemplateName = "sigTemplateMorphPdf_ttH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigTemplateMorphPdf_ttH = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,funcList_ttH,morphVarListSig,1.0,1)

        ##### 2D -> mass4l + KD  or mass4l,delta_m + KD
        name = "sigCB2d_ggH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigCB2d_ggH = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(sig_ggHErr,signalCB_ggH, self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(sigTemplateMorphPdf_ggH),ROOT.RooArgSet(D) ) )
        name = "sigCB2d_qqH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigCB2d_VBF = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(sig_VBFErr,signalCB_VBF, self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(sigTemplateMorphPdf_VBF),ROOT.RooArgSet(D) ) )
        name = "sigCB2d_WH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigCB2d_WH = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(sig_WHErr,signalCB_WH, self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(sigTemplateMorphPdf_WH),ROOT.RooArgSet(D) ) )
        name = "sigCB2d_ZH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigCB2d_ZH = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(sig_ZHErr,signalCB_ZH, self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(sigTemplateMorphPdf_ZH),ROOT.RooArgSet(D) ) )
	name = "sigCB2d_ttH_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        sigCB2d_ttH = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(sig_ttHErr,signalCB_ttH, self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(sigTemplateMorphPdf_ttH),ROOT.RooArgSet(D) ) )        

      ## ----------------- 2D BACKGROUND SHAPES --------------- ##
        qqzzbkgTemplate_beforeFix = fi.Get('qqzz_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_kd')
        qqzzbkgTemplate_beforeFix.SetDirectory(0)
        qqzzbkgTemplate = self.histogramBinFix(qqzzbkgTemplate_beforeFix, qqzzbkgTemplate_beforeFix.GetName())
	TemplateName = "qqzz_bkgTempDataHist_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        qqzzbkgTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),qqzzbkgTemplate)
        TemplateName = "bkgTemplatePdf_qqzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	qqzzbkgTemplatePdf = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),qqzzbkgTempDataHist)

        ggzzbkgTemplate_beforeFix = fi.Get('ggzz_'+self.year+'_'+self.fs_name+'_'+self.cate_name+'_'+self.tag_name+'_kd')
        ggzzbkgTemplate_beforeFix.SetDirectory(0)
	ggzzbkgTemplate = self.histogramBinFix(ggzzbkgTemplate_beforeFix, ggzzbkgTemplate_beforeFix.GetName())
        TemplateName = "ggzz_bkgTempDataHist_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        ggzzbkgTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),ggzzbkgTemplate)
	TemplateName = "bkgTemplatePdf_ggzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        ggzzbkgTemplatePdf = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),ggzzbkgTempDataHist)

        ##### ZX KD
	templatezxBkgName = 'commoninputs/Dbackground_ZX_'+self.fs_name+'.root'
        zxbkgTempFile = ROOT.TFile(templatezxBkgName)
        zxbkgTemplate_beforeFix = zxbkgTempFile.Get("h_mzzD")
        zxbkgTemplate_Up_beforeFix = zxbkgTempFile.Get("h_mzzD_up")
        zxbkgTemplate_Down_beforeFix = zxbkgTempFile.Get("h_mzzD_dn")
        zxbkgTemplate = self.histogramBinFix(zxbkgTemplate_beforeFix, "h_mzzD")
        zxbkgTemplate_Up = self.histogramBinFix(zxbkgTemplate_Up_beforeFix, "h_mzzD_up")
        zxbkgTemplate_Down = self.histogramBinFix(zxbkgTemplate_Down_beforeFix, "h_mzzD_dn")

        TemplateName = "zxbkgTempDataHist_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        zxbkgTempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),zxbkgTemplate)
        TemplateName = "zxbkgTempDataHist_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        zxbkgTempDataHist_Up = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),zxbkgTemplate_Up)
        TemplateName = "zxbkgTempDataHist_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        zxbkgTempDataHist_Down = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(CMS_zz4l_mass,D),zxbkgTemplate_Down)
        
        TemplateName = "zxbkgTemplatePdf_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgTemplatePdf_zjets = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),zxbkgTempDataHist)
        TemplateName = "zxbkgTemplatePdf_Up_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgTemplatePdf_zjets_Up = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),zxbkgTempDataHist_Up)
        TemplateName = "zxbkgTemplatePdf_Down_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgTemplatePdf_zjets_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(CMS_zz4l_mass,D),zxbkgTempDataHist_Down)
        funcList_zjets = ROOT.RooArgList()  
        morphBkgVarName = "CMS_zz4l_bkgMELA"    
        alphaMorphBkg = ROOT.RooRealVar(morphBkgVarName,morphBkgVarName,0,-20,20)
        morphVarListBkg = ROOT.RooArgList()
        
        if(self.bkgMorph):
            funcList_zjets.add(bkgTemplatePdf_zjets)
            funcList_zjets.add(bkgTemplatePdf_zjets_Up)
            funcList_zjets.add(bkgTemplatePdf_zjets_Down)  
            alphaMorphBkg.setConstant(False)
            morphVarListBkg.add(alphaMorphBkg)  
        else:
            funcList_zjets.add(bkgTemplatePdf_zjets)
            alphaMorphBkg.setConstant(True)
        
	TemplateName = "bkgTemplateMorphPdf_qqzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgTemplateMorphPdf_qqzz = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,ROOT.RooArgList(qqzzbkgTemplatePdf),ROOT.RooArgList(),1.0,1)

        TemplateName = "bkgTemplateMorphPdf_ggzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgTemplateMorphPdf_ggzz = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,ROOT.RooArgList(ggzzbkgTemplatePdf),ROOT.RooArgList(),1.0,1)

        TemplateName = "bkgTemplateMorphPdf_zjets_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        #bkgTemplateMorphPdf_zjets = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,True,ROOT.RooArgList(bkgTemplatePdf_zjets),ROOT.RooArgList(),1.0,1)
	bkgTemplateMorphPdf_zjets = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,CMS_zz4l_mass,D,self.Interp,funcList_zjets,morphVarListBkg,1.0,1)
	
	#save histpdf2d
	#if self.fs_name == '4mu':
	#	f2d = ROOT.TFile.Open('/eos/user/c/chenguan/oldkdtrue.root','RECREATE')
	#	f2d.cd()
	#	bkgTemplateMorphPdf_qqzz.Write()
	#	sigTemplateMorphPdf_ggH.Write()
	#	f2d.Close()

        #### bkg 2D : mass4l + KD; mass4l, delta_m + KD
        name = "bkg2d_qqzz" + '_' + self.year + '_' + self.fs_name + '_' + self.cate_name + '_' + self.tag_name
        bkg2d_qqzz = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(bkg_qqzzErr,bkg_qqzz,self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(bkgTemplateMorphPdf_qqzz),ROOT.RooArgSet(D) ) )
        name = "bkg2d_ggzz" + '_' + self.year + '_' + self.fs_name + '_' + self.cate_name + '_' + self.tag_name
        bkg2d_ggzz = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(bkg_ggzzErr,bkg_ggzz,self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(bkgTemplateMorphPdf_ggzz),ROOT.RooArgSet(D) ) )
        name = "bkg2d_zjets" + '_' + self.year + '_' + self.fs_name + '_' + self.cate_name + '_' + self.tag_name
        bkg2d_zjets = ROOT.RooProdPdf(name,name,ROOT.RooArgSet(self.getVariable(bkg_zjetsErr,bkg_zjets,self.bIncludingError)),ROOT.RooFit.Conditional(ROOT.RooArgSet(bkgTemplateMorphPdf_zjets),ROOT.RooArgSet(D) ) )

	##------------------- LUMI -------------------- ##

        rrvLumi = ROOT.RooRealVar("cmshzz4l_lumi","cmshzz4l_lumi",self.lumi)  
        
        ## ---------------------------------------------------------------- SIGNAL RATES ---------------------------------------------------- ##
        #CMS_zz4l_mass.setRange("shape",self.low_M,self.high_M)
        #CMS_zz4l_mass.setRange("fullrangesignal",self.low_M,self.high_M)
        #CMS_zz4l_mass.setRange("fullrange",100,1400)

        sigRate_ggH_Shape = 1
        sigRate_VBF_Shape = 1
        sigRate_WH_Shape = 1
        sigRate_ZH_Shape = 1
        sigRate_ttH_Shape = 1
        
	print 'load signal yield parameterization'  
        ## what go into ws

        ###########
        print 'signal rates'
       
	name = "sigrate_ggh_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)			#norm's dictinct name is assigned in datacard
        rfvSigRate_ggH = ROOT.RooFormulaVar("ggH_hzz_norm","("+params[name]+")",ROOT.RooArgList(self.MH))

	name = "sigrate_vbf_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfvSigRate_VBF = ROOT.RooFormulaVar("qqH_hzz_norm","("+params[name]+")",ROOT.RooArgList(self.MH))

	name = "sigrate_wph_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfvSigRate_WpH = ROOT.RooFormulaVar("WpH_hzz_norm","("+params[name]+")",ROOT.RooArgList(self.MH))

	name = "sigrate_wmh_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	rfvSigRate_WmH = ROOT.RooFormulaVar("WmH_hzz_norm","("+params[name]+")",ROOT.RooArgList(self.MH))

	name = "sigrate_zh_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfvSigRate_ZH = ROOT.RooFormulaVar("ZH_hzz_norm","("+params[name]+")",ROOT.RooArgList(self.MH))

	name = "sigrate_tth_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        rfvSigRate_ttH = ROOT.RooFormulaVar("ttH_hzz_norm","("+params[name]+")",ROOT.RooArgList(self.MH))
	
	name = "sigrate_wh_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
	rfvSigRate_WH = ROOT.RooFormulaVar("WH_hzz_norm","("+params[name]+")",ROOT.RooArgList(self.MH))

        print 'decide the final one export into the ws'


        # JES uncertainty
   
        JES = ROOT.RooRealVar("JES","JES",0,-5,5) 
        gghJES = ROOT.RooRealVar("gghJES","gghJES",0.07) 
        vbfJES = ROOT.RooRealVar("vbfJES","vbfJES",0.04)
        whJES = ROOT.RooRealVar("whJES","whJES",0.05)
        zhJES = ROOT.RooRealVar("zhJES","zhJES",0.06)
        tthJES = ROOT.RooRealVar("tthJES","tthJES",0.02)

        qqzzJES = ROOT.RooRealVar("qqzzJES","qqzzES",0.09)
        ggzzJES = ROOT.RooRealVar("ggzzJES","ggzzJES",0.08)
 

#        if (self.bVBF and self.VBFcat == 0) :
#            rfvSigRate_ggH = ROOT.RooFormulaVar("ggH_hzz_norm","@0-@1*@2*@3",ROOT.RooArgList(rfvSigRate_ggH_0,rfvSigRate_ggH_1,JES,gghJES))
#            rfvSigRate_VBF = ROOT.RooFormulaVar("qqH_hzz_norm","@0-@1*@2*@3",ROOT.RooArgList(rfvSigRate_VBF_0,rfvSigRate_VBF_1,JES,vbfJES))
#            rfvSigRate_WH = ROOT.RooFormulaVar("WH_hzz_norm","@0-@1*@2*@3",ROOT.RooArgList(rfvSigRate_WH_0,rfvSigRate_WH_1,JES,whJES))
#            rfvSigRate_ZH = ROOT.RooFormulaVar("ZH_hzz_norm","@0-@1*@2*@3",ROOT.RooArgList(rfvSigRate_ZH_0,rfvSigRate_ZH_1,JES,zhJES))
#            rfvSigRate_ttH = ROOT.RooFormulaVar("ttH_hzz_norm","@0-@1*@2*@3",ROOT.RooArgList(rfvSigRate_ttH_0,rfvSigRate_ttH_1,JES,tthJES))

#            print 'Untagged'
#            print 'ggH ', rfvSigRate_ggH.getVal(),'VBF ', rfvSigRate_VBF.getVal()
#            print 'WH ', rfvSigRate_WH.getVal(),'ZH ', rfvSigRate_ZH.getVal()

#        if (self.bVBF and self.VBFcat == 1) :  
#            rfvSigRate_ggH = ROOT.RooFormulaVar("ggH_hzz_norm","(@0)*(1+@1*@2)",ROOT.RooArgList(rfvSigRate_ggH_1,JES,gghJES))
#            rfvSigRate_VBF = ROOT.RooFormulaVar("qqH_hzz_norm","(@0)*(1+@1*@2)",ROOT.RooArgList(rfvSigRate_VBF_1,JES,vbfJES))
#            rfvSigRate_WH = ROOT.RooFormulaVar("WH_hzz_norm","(@0)*(1+@1*@2)",ROOT.RooArgList(rfvSigRate_WH_1,JES,whJES))
#            rfvSigRate_ZH = ROOT.RooFormulaVar("ZH_hzz_norm","(@0)*(1+@1*@2)",ROOT.RooArgList(rfvSigRate_ZH_1,JES,zhJES))
#            rfvSigRate_ttH = ROOT.RooFormulaVar("ttH_hzz_norm","(@0)*(1+@1*@2)",ROOT.RooArgList(rfvSigRate_ttH_1,JES,tthJES))

#            print 'VBFtagged'
#            print 'ggH ', rfvSigRate_ggH.getVal(),'VBF ', rfvSigRate_VBF.getVal()
#            print 'WH ', rfvSigRate_WH.getVal(),'ZH ', rfvSigRate_ZH.getVal()            
        
	#print 'ttH just copy ggH'
        #rfvSigRate_ttH = rfvSigRate_ggH.Clone()
        #rfvSigRate_ttH.SetName("ttH_hzz_norm")
#        print 'Rates ggH ',rfvSigRate_ggH.getVal(),' VBF ',rfvSigRate_VBF.getVal()
#        print 'Rates WH ',rfvSigRate_WH.getVal(),' ZH ',rfvSigRate_ZH.getVal()

        ## ----------------------- BACKGROUND RATES ----------------------- ##

        key = "bkgrate_qqzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgRate_qqzz = float(params[key])
        key = "bkgrate_ggzz_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgRate_ggzz = float(params[key])
        key = "bkgrate_zjets_{0}_{1}_{2}_{3}".format(self.year,self.fs_name,self.cate_name,self.tag_name)
        bkgRate_zjets = float(params[key])

        bkgRate_qqzz_Shape = bkgRate_qqzz
        bkgRate_ggzz_Shape = bkgRate_ggzz
        bkgRate_zjets_Shape = bkgRate_zjets

#        name = "tagRatio_qqzz_{0:.0f}".format(self.channel)
#        tagRatio_qqzz = RooRealVar(name,name,bkgRate_qqzz_VBFtagged/bkgRate_qqzz_untagged)
#        name = "tagRatio_ggzz_{0:.0f}".format(self.channel)
#        tagRatio_ggzz = RooRealVar(name,name,bkgRate_ggzz_VBFtagged/bkgRate_ggzz_untagged)     

#        if (self.bVBF and self.VBFcat == 0) :
#            rfvSigRate_qqzz = ROOT.RooFormulaVar("qqzz_norm","1-@0*@1*@2",ROOT.RooArgList(JES,qqzzJES,tagRatio_qqzz))
#            rfvSigRate_ggzz = ROOT.RooFormulaVar("ggzz_norm","1-@0*@1*@2",ROOT.RooArgList(JES,ggzzJES,tagRatio_ggzz))
#
#        if (self.bVBF and self.VBFcat == 1) :
#            rfvSigRate_qqzz = ROOT.RooFormulaVar("qqzz_norm","1+@0*@1",ROOT.RooArgList(JES,qqzzJES))
#           rfvSigRate_ggzz = ROOT.RooFormulaVar("ggzz_norm","1+@0*@1",ROOT.RooArgList(JES,ggzzJES))

        
        ## --------------------------- DATASET --------------------------- ##					## skip this section
	
	tmp_name = self.fs_name
	if self.fs_name == '2mu2e':
		tmp_name = '2e2mu'

        dataFileDir = "CMSdata"
        if(self.type_name == 'refit') :
           dataFileDir+='_REFIT'

        dataTreeName = "data_obs" 
        dataFileName = "{0}/hzz{1}_{2}.root".format(dataFileDir,tmp_name,self.lumi)
        
	if (DEBUG): print dataFileName," ",dataTreeName 
        data_obs_file = ROOT.TFile(dataFileName)

        print data_obs_file.Get(dataTreeName)
        
        if not (data_obs_file.Get(dataTreeName)):
            print "File, \"",dataFileName,"\", or tree, \"",dataTreeName,"\", not found" 
            print "Exiting..."
            sys.exit()
        
        data_obs_tree = data_obs_file.Get(dataTreeName)
        data_obs = ROOT.RooDataSet()
        datasetName = "data_obs_{0}".format(tmp_name)
        
        if (self.is2D == 0):
            if(self.bIncludingError): data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ROOT.RooArgSet(CMS_zz4l_mass, MassErr))
            else: data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ROOT.RooArgSet(CMS_zz4l_mass))
		
        if (self.is2D == 1):
            if(self.bIncludingError):
                data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ROOT.RooArgSet(CMS_zz4l_mass,D,MassErr) )
            else: data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ROOT.RooArgSet(CMS_zz4l_mass,D) )

        ## --------------------------- WORKSPACE -------------------------- ##

        endsInP5 = False
        tmpMH = self.mH
        if ( math.fabs(math.floor(tmpMH)-self.mH) > 0.001): endsInP5 = True					#I dont know what is the mean of this endsInP5, but it never be used, always false
        if (DEBUG): print "ENDS IN P5  ",endsInP5

        name_Shape = ""
        name_ShapeWS = ""
        name_ShapeWS2 = ""

	#datacard name
        name_Shape = self.outputDir + '/hzz4l_' +self.year+"_"+self.fs_name+"_cat"+self.cate_name+"_"+self.tag_name+".txt"
	#workspace name
        name_ShapeWS = self.outputDir + '/hzz4l_' +self.year+"_"+self.fs_name+"_cat"+self.cate_name+"_"+self.tag_name+".input.root"
	#workspace name written in datacard
        name_ShapeWS2 = "hzz4l_"+self.year+"_"+self.fs_name+"_cat"+self.cate_name+"_"+self.tag_name+".input.root"
        if(DEBUG): print name_Shape,"  ",name_ShapeWS2
        w = ROOT.RooWorkspace("w","w")
        w.importClassCode(RooDoubleCB.Class(),True)
        w.importClassCode(RooFormulaVar.Class(),True)
   	
	getattr(w,'import')(data_obs,ROOT.RooFit.Rename("data_obs")) ### Should this be renamed?
	 
        if (self.is2D == 0):
	    if not self.bIncludingError:
                	signalCB_ggH.SetNameTitle("ggH_hzz","ggH_hzz")						#why do they need to be renamed? these name will be changed by combinetool,
                	signalCB_VBF.SetNameTitle("qqH_hzz","qqH_hzz")						#like, 'shapeSig_'+$name+'_{1}', 'shapeBkg_'+$name+'_{1}'
                	signalCB_WH.SetNameTitle("WH_hzz","WH_hzz")						#there is one line start with 'bin' in datacard, the string in this line will be suffix {1}
                	signalCB_ZH.SetNameTitle("ZH_hzz","ZH_hzz")						#the bin line string can be set within combineCards.py command
                	signalCB_ttH.SetNameTitle("ttH_hzz","ttH_hzz")
                
                	getattr(w,'import')(signalCB_ggH, ROOT.RooFit.RecycleConflictNodes())
                	getattr(w,'import')(signalCB_VBF, ROOT.RooFit.RecycleConflictNodes())
               		getattr(w,'import')(signalCB_WH, ROOT.RooFit.RecycleConflictNodes())
                	getattr(w,'import')(signalCB_ZH, ROOT.RooFit.RecycleConflictNodes())
                	getattr(w,'import')(signalCB_ttH, ROOT.RooFit.RecycleConflictNodes())
	    else:
                	sig_ggHErr.SetNameTitle("ggH_hzz","ggH_hzz")
                	sig_VBFErr.SetNameTitle("qqH_hzz","qqH_hzz")
                	sig_WHErr.SetNameTitle("WH_hzz","WH_hzz")
                	sig_ZHErr.SetNameTitle("ZH_hzz","ZH_hzz")
                	sig_ttHErr.SetNameTitle("ttH_hzz","ttH_hzz")
                
                	getattr(w,'import')(sig_ggHErr, ROOT.RooFit.RecycleConflictNodes())
                	getattr(w,'import')(sig_VBFErr, ROOT.RooFit.RecycleConflictNodes())
               		getattr(w,'import')(sig_WHErr, ROOT.RooFit.RecycleConflictNodes())
                	getattr(w,'import')(sig_ZHErr, ROOT.RooFit.RecycleConflictNodes())
                	getattr(w,'import')(sig_ttHErr, ROOT.RooFit.RecycleConflictNodes())
                
        if (self.is2D == 1):
        
                    sigCB2d_ggH.SetNameTitle("ggH_hzz","ggH_hzz")
                    sigCB2d_VBF.SetNameTitle("qqH_hzz","qqH_hzz")
                    sigCB2d_WH.SetNameTitle("WH_hzz","WH_hzz")
                    sigCB2d_ZH.SetNameTitle("ZH_hzz","ZH_hzz")
                    sigCB2d_ttH.SetNameTitle("ttH_hzz","ttH_hzz")
                
                    getattr(w,'import')(sigCB2d_ggH, ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(sigCB2d_VBF, ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(sigCB2d_WH, ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(sigCB2d_ZH, ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(sigCB2d_ttH, ROOT.RooFit.RecycleConflictNodes()) 

        if (self.is2D == 0):
		if not self.bIncludingError:
			bkg_qqzz.SetNameTitle("bkg_qqzz","bkg_qqzz")
			bkg_ggzz.SetNameTitle("bkg_ggzz","bkg_ggzz")
			bkg_zjets.SetNameTitle("bkg_zjets","bkg_zjets")
            		getattr(w,'import')(bkg_qqzz, ROOT.RooFit.RecycleConflictNodes())
            		getattr(w,'import')(bkg_ggzz, ROOT.RooFit.RecycleConflictNodes())
            		getattr(w,'import')(bkg_zjets, ROOT.RooFit.RecycleConflictNodes())
		else:
			bkg_qqzzErr.SetNameTitle("bkg_qqzz","bkg_qqzz")
			bkg_ggzzErr.SetNameTitle("bkg_ggzz","bkg_ggzz")
			bkg_zjetsErr.SetNameTitle("bkg_zjets","bkg_zjets")
            		getattr(w,'import')(bkg_qqzzErr, ROOT.RooFit.RecycleConflictNodes())
            		getattr(w,'import')(bkg_ggzzErr, ROOT.RooFit.RecycleConflictNodes())
            		getattr(w,'import')(bkg_zjetsErr, ROOT.RooFit.RecycleConflictNodes())
            
        if (self.is2D == 1):
		bkg2d_qqzz.SetNameTitle("bkg2d_qqzz","bkg2d_qqzz")
		bkg2d_ggzz.SetNameTitle("bkg2d_ggzz","bkg2d_ggzz")
		bkg2d_zjets.SetNameTitle("bkg2d_zjets","bkg2d_zjets")
                getattr(w,'import')(bkg2d_qqzz,ROOT.RooFit.RecycleConflictNodes())
                getattr(w,'import')(bkg2d_ggzz,ROOT.RooFit.RecycleConflictNodes())
                getattr(w,'import')(bkg2d_zjets,ROOT.RooFit.RecycleConflictNodes())

        
        getattr(w,'import')(rfvSigRate_ggH, ROOT.RooFit.RecycleConflictNodes())
        getattr(w,'import')(rfvSigRate_VBF, ROOT.RooFit.RecycleConflictNodes())
        getattr(w,'import')(rfvSigRate_WH, ROOT.RooFit.RecycleConflictNodes())
        getattr(w,'import')(rfvSigRate_ZH, ROOT.RooFit.RecycleConflictNodes())
        getattr(w,'import')(rfvSigRate_ttH, ROOT.RooFit.RecycleConflictNodes())
            
        CMS_zz4l_mass.setRange(self.low_M,self.high_M)
        w.writeToFile(name_ShapeWS)									##write 'w' to name_ShapeWS
	
	## --------------------------- DATACARDS -------------------------- ##

        ## If the channel is not declared in inputs, set rate = 0
        if not self.ggH_chan and not self.all_chan :  sigRate_ggH_Shape = 0
        if not self.qqH_chan:  sigRate_VBF_Shape = 0
        if not self.WH_chan:   sigRate_WH_Shape = 0
        if not self.ZH_chan:   sigRate_ZH_Shape = 0
        if not self.ttH_chan:  sigRate_ttH_Shape = 0

        if not self.qqZZ_chan:  bkgRate_qqzz_Shape = 0
        if not self.ggZZ_chan:  bkgRate_ggzz_Shape = 0
        if not self.zjets_chan: bkgRate_zjets_Shape = 0

        smallNumber = 1e-9
        rates = {}

        rates['ggH'] = sigRate_ggH_Shape#*scaleLumi*scalenlo
        rates['qqH'] = sigRate_VBF_Shape#*scaleLumi*scalenlo
        rates['WH']  = sigRate_WH_Shape#*scaleLumi*scalenlo
        rates['ZH']  = sigRate_ZH_Shape#*scaleLumi*scalenlo
        rates['ttH'] = sigRate_ttH_Shape#*scaleLumi*scalenlo


        rates['qqZZ']  = bkgRate_qqzz_Shape#bkgRate_qqzz_Shape
        rates['ggZZ']  = bkgRate_ggzz_Shape#bkgRate_ggzz_Shape
        rates['zjets'] = bkgRate_zjets_Shape#bkgRate_zjets_Shape
        rates['ttbar'] = smallNumber
        rates['zbb']   = smallNumber
        
        for key in rates:
            rates[key] = rates[key]*(3000.0/59.83)

        ## Write Datacards
        fo = open( name_Shape, "wb")
        self.WriteDatacard(fo,theInputs, name_ShapeWS2, rates, data_obs.numEntries(), self.is2D )
        systematics.WriteSystematics(fo, theInputs)
        systematics.WriteShapeSystematics(fo,theInputs)
        fo.close()

	## --------------------------------------------- PLOTS FOR SANITY CHECKS ------------------------------- ##	every time update the old one
	'''
	#if sanitycheckdir != '':
		#frame = CMS_zz4l_mass.frame()
		#signalCB_ZH.plotOn(frame,ROOT.RooFit.LineColor(1))
		#signalCB_WH.plotOn(frame,ROOT.RooFit.LineColor(2))
		#signalCB_ggH.plotOn(frame,ROOT.RooFit.LineColor(3))
		#signalCB_ttH.plotOn(frame,ROOT.RooFit.LineColor(4))
		#bkg_ggzz.plotOn(frame,ROOT.RooFit.LineColor(5))
		#bkg_qqzz.plotOn(frame,ROOT.RooFit.LineColor(6))
		#bkg_zjets.plotOn(frame,ROOT.RooFit.LineColor(7))
	c = ROOT.TCanvas('c','',1400,1000)
		#frame.Draw()
		#if self.MassOnly: name = 'dcb'
		#if not self.MassOnly and self.FFT: name = 'fft'
		#if not self.MassOnly and not self.FFT: name = 'approx'
		#c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/'+name+'_ggH_'+self.fs_name+'_'+self.cate_name+'.png')
	sigTemplate.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/sigkdmap_af.png')
	c.Clear()
	qqzzbkgTemplate.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/qqzzkdmap_af.png')
	c.Clear()
	ggzzbkgTemplate.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/ggzzkdmap_af.png')
	c.Clear()
	zxbkgTemplate.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/zjetskdmap_af.png')
	c.Clear()

	sigTemplate_beforeFix.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/sigkdmap_bf.png')
	c.Clear()
	qqzzbkgTemplate_beforeFix.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/qqzzkdmap_bf.png')
	c.Clear()
	ggzzbkgTemplate_beforeFix.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/ggzzkdmap_bf.png')
	c.Clear()
	zxbkgTemplate_beforeFix.Draw('colz')
	c.SaveAs('/eos/user/c/chenguan/www/DataCardPlots/zjetskdmap_bf.png')
	
	f_sig = ROOT.TFile.Open('/eos/user/c/chenguan/www/DataCardPlots/sigkdmap_af.root','RECREATE')
	sigTemplate.Write()
	f_sig.Close()
	f_qqzz = ROOT.TFile.Open('/eos/user/c/chenguan/www/DataCardPlots/qqzzkdmap_af.root','RECREATE')
	qqzzbkgTemplate.Write()
	f_qqzz.Close()
	f_zjets = ROOT.TFile.Open('/eos/user/c/chenguan/www/DataCardPlots/zjetskdmap_af.root','RECREATE')
	zxbkgTemplate.Write()
	f_zjets.Close()
	'''






    def WriteDatacard(self,file,theInputs,nameWS,theRates,obsEvents,is2D,isAltCard=False,AltLabel="",bVBF=False,VBFcat=""):

        numberSig = self.numberOfSigChan(theInputs)
        numberBg  = self.numberOfBgChan(theInputs)
        
        file.write("imax 1\n")
        file.write("jmax {0}\n".format(numberSig+numberBg-1))
        file.write("kmax *\n")															#wildcard *, combine can recognize it automaticlly
        
        file.write("------------\n")
        file.write("shapes * * {0} w:$PROCESS \n".format(nameWS))
        file.write("------------\n")
        
        file.write("bin cms_{0}_{1}_cat{2} \n".format(self.year,self.fs_name,self.cate_name))
        file.write("observation {0} \n".format(obsEvents))
        
        file.write("------------\n")
        file.write("## mass window [{0},{1}] \n".format(self.low_M,self.high_M))
        file.write("bin ")        

        channelList=['ggH','qqH','WH','ZH','ttH','qqZZ','ggZZ','zjets','ttbar','zbb']

        channelName1D=['ggH_hzz','qqH_hzz','WH_hzz','ZH_hzz','ttH_hzz','bkg_qqzz','bkg_ggzz','bkg_zjets','bkg_ttbar','bkg_zbb']
        channelName2D=['ggH_hzz','qqH_hzz','WH_hzz','ZH_hzz','ttH_hzz','bkg2d_qqzz','bkg2d_ggzz','bkg2d_zjets','bkg2d_ttbar','bkg2d_zbb']
		 
        for chan in channelList:
            if theInputs[chan]:
                    file.write("cms_{0}_{1}_cat{2} ".format(self.year,self.fs_name,self.cate_name))
        file.write("\n")
                                        
        file.write("process ")

        i=0
        if not (self.is2D == 1):
		for chan in channelList:
                	if theInputs[chan]:
				file.write("{0} ".format(channelName1D[i]))
                    	i+=1
        else:
		for chan in channelList:
                	if theInputs[chan]:
                    		file.write("{0} ".format(channelName2D[i]))
                    	i+=1
        file.write("\n")
            
        processLine = "process "

        for x in range(-numberSig+1,1):
            processLine += "{0} ".format(x)

        for y in range(1,numberBg+1):
            processLine += "{0} ".format(y)

        file.write(processLine)
        file.write("\n")
            
        file.write("rate ")
        for chan in channelList:
		if theInputs[chan]:
                	file.write("{0:.10f} ".format(theRates[chan]))
        file.write("\n")
        file.write("------------\n")


        
    def numberOfSigChan(self,inputs):

        counter=0

        if inputs['ggH']: counter+=1
        if inputs['qqH']: counter+=1
        if inputs['WH']:  counter+=1
        if inputs['ZH']:  counter+=1
        if inputs['ttH']: counter+=1
        if inputs['all']: counter+=1
        
        return counter

    def numberOfBgChan(self,inputs):

        counter=0

        if inputs['qqZZ']:  counter+=1
        if inputs['ggZZ']:  counter+=1
        if inputs['zjets']: counter+=1
        if inputs['ttbar']: counter+=1
        if inputs['zbb']:   counter+=1
        
        return counter

