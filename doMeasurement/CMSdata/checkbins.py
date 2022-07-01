import sys
import ROOT
fin = ROOT.TFile(sys.argv[1],'read')
tree = fin.Get('data_obs')
CMS_zz4l_mass = ROOT.RooRealVar('CMS_zz4l_mass','CMS_zz4l_mass',105,140)
CMS_zz4l_mass.setBins(1000)
CMS_zz4l_mass.setBins(100,"cache")
dataset = ROOT.RooDataSet()
data_obs = ROOT.RooDataSet('data_obs','data_obs',tree,ROOT.RooArgSet(CMS_zz4l_mass))
data_obs.Print()
CMS_zz4l_mass.Print()

#w = ROOT.RooWorkspace('w','w')
#getattr(w,'import')(CMS_zz4l_mass,ROOT.RooFit.RecycleConflictNodes())
#w.writeToFile('test.root')

