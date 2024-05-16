import sys
import ROOT
filename = sys.argv[1]
f = ROOT.TFile(filename,'READ')
w = f.Get('w')
w.Print()
