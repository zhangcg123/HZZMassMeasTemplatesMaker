import ROOT
import os
import sys
import glob
path1 = sys.argv[1]
path2 = sys.argv[2]
aname = path1.split('/')[-2][8:]
bname = path2.split('/')[-2][8:]

for i in ['pt','eta','phi']:	
	for j in ['angle_distributions','fix_pt_distributions','free_pt_distributions','lepton_distributions','mass_distributions','zeta_distributions','zphi_distributions','zpt_distributions','zpz_distributions','pterr_distributions','pterrold_distributions']:
		
		path1_ = path1+j+'/'+i+'_pos_vs_neg'
		path2_ = path2+j+'/'+i+'_pos_vs_neg'
		root_lista = glob.glob(path1_+'/*root')
		root_listb = glob.glob(path2_+'/*root')
		histo_lista = {}
		histo_listb = {}
		
		for a in root_lista:
			print a
			f = ROOT.TFile.Open(a,'r')
			keylist = f.GetListOfKeys()
			obj = keylist[0].ReadObj()
			obj.SetDirectory(0)
			obj.SetName(keylist[0].GetName())
			histo_lista[a.split('/')[-1].split('.')[0]] = obj
			
		for b in root_listb:
			print b
			f = ROOT.TFile.Open(b,'r')
			keylist = f.GetListOfKeys()
			obj = keylist[0].ReadObj()
			obj.SetDirectory(0)
			obj.SetName(keylist[0].GetName())
			histo_listb[b.split('/')[-1].split('.')[0]] = obj

		for a in histo_lista:
			for b in histo_listb:
				if a == b:
					histo_lista[a].SetName(a[:-7]+'_'+aname)
					histo_listb[b].SetName(b[:-7]+'_'+bname)
					canvas = ROOT.TCanvas('c','',1400,1000)
					canvas.cd()
					histo_lista[a].SetLineWidth(6)
					histo_listb[b].SetLineWidth(6)
					histo_lista[a].SetStats(0)
					histo_listb[b].SetStats(0)
					histo_lista[a].SetLineColor(2)
					histo_listb[b].SetLineColor(4)
					lx = ROOT.TLatex()
					lx.SetNDC()
					lx.SetTextSize(0.05)
					lx.SetTextFont(42)
					lx.SetTextAlign(23)
					aentries = str(histo_lista[a].GetEntries())
					bentries = str(histo_listb[b].GetEntries())
					#print aentries, bentries
					if not j == 'fix_pt_distributions':
						histo_lista[a].Scale(1/histo_lista[a].Integral())
						histo_listb[b].Scale(1/histo_listb[b].Integral())
					histo_lista[a].SetMaximum(1.1*max(histo_lista[a].GetMaximum(),histo_listb[b].GetMaximum()))
					histo_lista[a].Draw('hist')
					histo_listb[b].Draw('histsame')
					lx.SetTextColor(2)
					lx.DrawLatex(0.7,0.9,aentries)
					lx.SetTextColor(4)
					lx.DrawLatex(0.7,0.8,bentries)
					canvas.BuildLegend()
					canvas.SaveAs(path1+j+'/'+i+'_datavsmc/'+histo_lista[a].GetName()+'.png')
