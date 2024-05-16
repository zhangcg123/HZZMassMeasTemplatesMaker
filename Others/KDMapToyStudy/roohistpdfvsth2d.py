import sys
import ROOT
import os
import glob
sys.path.append('/afs/cern.ch/work/c/chenguan/private/pycommontool/')
from FileSystemClass import *

dirs = DirTree()
dirs.mkrootdir('datacard_vs_again_'+sys.argv[1]+'_'+sys.argv[2]+'_NOTsychbinswithdatacardClass_x70y100')
path = dirs.root
cardkdpdf = dirs.mksubdir('cardkdpdf')
card2dpdf = dirs.mksubdir('card2dpdf')
outkdhist = dirs.mksubdir('outkdhist')
againkdpdf = dirs.mksubdir('againkdpdf')

suffix = ''
if 'KDTrue' in sys.argv[1]: suffix = '_opt'

c = ROOT.TCanvas('c','',1500,1000)
c.cd()
t = ROOT.TLatex()
t.SetNDC()
t.SetTextFont( 62 )
t.SetTextColor( 36 )
t.SetTextSize( 0.08 )
t.SetTextAlign( 12 )

x = ROOT.RooRealVar('CMS_zz4l_mass','',105,140)
y = ROOT.RooRealVar('melaLD','',0,1)
x.setBins(70)
y.setBins(100)

f = ROOT.TFile('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/datacardsOutputs_'+sys.argv[1]+'/2018_reco_'+sys.argv[2]+'_1_notag_masspdf/hzz4l_2018_allfinalstates_allcategs_massmodelforMH_floatothers1_MH_124_126.root')
w = f.Get('w')
#roohistpdf from datacard, they are mass_kd 2d maps
obj_roohistpdf_ggh = w.obj('sigTemplatePdf_ggH_2018_4mu_0_notag')
obj_roohistpdf_qqzz = w.obj('bkgTemplatePdf_qqzz_2018_4mu_0_notag')
#obj_roohistpdf_zj = w.obj('zxbkgTemplatePdf_2018_4mu_0_notag')
#rooprodpdf from datacard, they are 2d model
obj_rooprodpdf_ggh = w.obj('shapeSig_ggH_hzz_ch2'+suffix)
obj_rooprodpdf_qqzz = w.obj('shapeBkg_bkg2d_qqzz_ch2'+suffix)
#obj_rooprodpdf_zj = w.obj('shapeBkg_bkg2d_zjets_ch2'+suffix)

names = ['ggh_4mu_kdmap','qqzz_4mu_kdmap', 'ggh_4mu_2dmodel', 'qqzz_4mu_2dmodel']

objs = [obj_roohistpdf_ggh, obj_roohistpdf_qqzz, obj_rooprodpdf_ggh, obj_rooprodpdf_qqzz]

frames_tox = []
frames_toy = []
for i in range(len(objs)):
	frames_tox.append(x.frame())
	frames_toy.append(y.frame())

for i in range(len(objs)):
	c.Clear()
	objs[i].plotOn(frames_tox[i])
	frames_tox[i].Draw()
	if i < 2 :
		c.SaveAs(cardkdpdf + '/' + names[i]+'_tox.png')
	else:
		c.SaveAs(card2dpdf + '/' + names[i]+'_tox.png')
	c.Clear()
	objs[i].plotOn(frames_toy[i])
	frames_toy[i].Draw()
	if i < 2:
		c.SaveAs(cardkdpdf+ '/' + names[i]+'_toy.png')
	else:
		c.SaveAs(card2dpdf + '/' + names[i]+'_toy.png')

for i in range(len(objs)):
	c.Clear()
	hh2d = objs[i].createHistogram(names[i],x,ROOT.RooFit.YVar(y))
	hh2d.Print()
	hh2d.Draw('surf')
	t.DrawLatex(0.2,0.8,str(hh2d.Integral('width')))
	if i < 2:
		c.SaveAs(cardkdpdf + '/' + names[i] + '_2d.png')
	else:
		c.SaveAs(card2dpdf + '/' + names[i] + '_2d.png')

#above from datacards and workspace
#below from histogram themselves
a = 'kd70x100_doublesmooth_origin'
if 'slicenorm' in sys.argv[1]:
	a = a.replace('origin','slicenorm')
fh = ROOT.TFile('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/datacardsInputs_'+a+'/2018_reco_1_notag/2018_1categs_reco_notag_templates.root','READ')
h_ggh = fh.Get('ggh_2018_4mu_0_notag_kd')
h_qqzz = fh.Get('qqzz_2018_4mu_0_notag_kd')
h_ggh.SetDirectory(0)
h_qqzz.SetDirectory(0)
fh.Close()
zh = ROOT.TFile('/afs/cern.ch/work/c/chenguan/CMSRunII-HiggsMassMears/HZZMassMeasTemplatesMaker/doMeasurement/commoninputs/Dbackground_ZX_4mu.root','READ')
h_zj = zh.Get('h_mzzD')
h_zj.SetDirectory(0)
zh.Close()
histograms = [h_ggh, h_qqzz]
names_ = ['ggh_4mu_kdmap_outside','qqzz_4mu_kdmap_outside']
for i in range(len(histograms)):
	c.Clear()
	histograms[i].Draw('surf')
	histograms[i].SetStats(1)
	t.DrawLatex(0.2,0.8,str(histograms[i].Integral('width')))
	c.SaveAs(outkdhist + '/' + names_[i] + '_2d.png')
	
	c.Clear()
	histograms[i].ProjectionX('',1,histograms[i].GetYaxis().GetNbins()).Draw()
	t.DrawLatex(0.2,0.8,str(histograms[i].ProjectionX().Integral('width')))
	c.SaveAs(outkdhist + '/' + names_[i] + '_tox.png')

	c.Clear()
	histograms[i].ProjectionY('',1,histograms[i].GetXaxis().GetNbins()).Draw()
	t.DrawLatex(0.2,0.8,str(histograms[i].ProjectionY().Integral('width')))
	c.SaveAs(outkdhist + '/' + names_[i] + '_toy.png')

#create roohistpdf again
roopdfs = []
histpdfs = []
names_again = ['ggh_4mu_kdmap_again','qqzz_4mu_kdmap_again']
for i in range(len(histograms)):
	datahist = ROOT.RooDataHist(names_[i]+'_datahist','',ROOT.RooArgList(x,y),histograms[i])
	histpdfs.append(datahist)
for i in range(len(histograms)):
	histpdf = ROOT.RooHistPdf(names_again[i],'',ROOT.RooArgSet(x,y),histpdfs[i])
	roopdfs.append(histpdf)

frames_tox_again = []
frames_toy_again = []
for i in range(len(roopdfs)):
	frames_tox_again.append(x.frame())
	frames_toy_again.append(y.frame())
for i in range(len(roopdfs)):
	c.Clear()
	roopdfs[i].plotOn(frames_tox_again[i])
	frames_tox_again[i].Draw()
	c.SaveAs(againkdpdf + '/' + names_again[i] + '_tox.png')
	c.Clear()
	roopdfs[i].plotOn(frames_toy_again[i])
	frames_toy_again[i].Draw()
	c.SaveAs(againkdpdf + '/' + names_again[i] + '_toy.png')

for i in range(len(roopdfs)):
	c.Clear()
	hh2d_ = roopdfs[i].createHistogram(names_again[i],x,ROOT.RooFit.YVar(y))
	hh2d_.Draw('surf')
	t.DrawLatex(0.2,0.8,str(hh2d.Integral('width')))
	c.SaveAs(againkdpdf + '/' + names_again[i] + '_2d.png')
