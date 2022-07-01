import ROOT
import random
import numpy
import sys
import os

tot = int(sys.argv[1])
#mass is the center
width = float(sys.argv[2])
sigma = float(sys.argv[3])
low = float(sys.argv[4])
high = float(sys.argv[5])
shift_ = float(sys.argv[6])

shift = abs(shift_)*(high-low)
range1low = low+abs(shift_)*(high-low)			#make a room for shift
range1high = high-abs(shift_)*(high-low)
range2low = range1low + shift_*(high-low)
range2high = range1high + shift_*(high-low)

print low, high, range1low, range1high, range2low, range2high
assert range1low < range1high and range2low < range2high and range1low >= low and range2low >= low and range1high <= high and range2high <= high and sigma < 10.0 and abs(shift_) < 0.25, 'rrrrr'

#generate toys
x = ROOT.RooRealVar('x','',low,high)
genm = ROOT.RooRealVar('genm','',0.5*(low+high))		#
genw = ROOT.RooRealVar('genw','',width)
bw = ROOT.RooBreitWigner('bw','',x,genm,genw)
dataset = bw.generate(ROOT.RooArgSet(x),tot)

numpy.random.seed(12345)
smeared = ROOT.RooDataSet('smeared','',ROOT.RooArgSet(x))
for i in range(tot):
	point  = dataset.get(i).getRealValue('x') +  numpy.random.normal(0,sigma)
	tmp = ROOT.RooRealVar('x','',low,high)
	tmp.setVal(point)
	smeared.add(ROOT.RooArgSet(tmp))

#fft
m = ROOT.RooRealVar('m','',0,-5,5)
s = ROOT.RooRealVar('s','',sigma,0,10)
gauss = ROOT.RooGaussian('gauss','',x,m,s)
fft = ROOT.RooFFTConvPdf('fft','',x,bw,gauss)
fft.setBufferFraction(0.2)

#voigtian
genmv = ROOT.RooRealVar('genm','',0.5*(low+high),0.5*(low+high)-1.0,0.5*(low+high)+1.0)
genwv = ROOT.RooRealVar('genw','',width,width-1,width+1)
bwv = ROOT.RooBreitWigner('bw','',x,genmv,genwv)
voigt = ROOT.RooVoigtian('voigt','',x,genmv,genwv,s)

#85-95 dataset
dataset8595 = ROOT.RooDataSet('dataset8595','',ROOT.RooArgSet(x),ROOT.RooFit.Import(smeared),ROOT.RooFit.Cut('x<'+str(range1high)+'&&x>'+str(range1low)))			#sys window
#96-96 dataset
dataset8696 = ROOT.RooDataSet('dataset8696','',ROOT.RooArgSet(x),ROOT.RooFit.Import(smeared),ROOT.RooFit.Cut('x<'+str(range2high)+'&&x>'+str(range2low)))			#un sys window

#make plot
c = ROOT.TCanvas('c','',1000,1000)
c.Divide(2,2)
lx = ROOT.TLatex()
lx = ROOT.TLatex()
lx.SetNDC()
lx.SetTextSize(0.05)
lx.SetTextFont(42)
lx.SetTextAlign(23)

#first fft
x.setRange(range1low,range1high)
x.setBins(int((range1high-range1low)*100))
m.setVal(0)
s.setVal(1)
m.setRange(-5,5)
s.setRange(0,10)
frame8595fft = x.frame(ROOT.RooFit.Bins(30))
frame8595fft.SetTitle('FFT '+str(int(range1low))+'-'+str(int(range1high)))
dataset8595.plotOn(frame8595fft)
fft.fitTo(dataset8595)
fft.plotOn(frame8595fft)
c.cd(1)
frame8595fft.Draw()
lx.DrawLatex(0.3,0.9,'peak='+str(0.5*(low+high)+m.getVal())[0:7])
#lx.DrawLatex(0.3,0.7,'width='+str(genw.getVal())[0:7])
#lx.DrawLatex(0.3,0.8,'sigma='+str(s.getVal())[0:7])
ffta = m.getVal()

#first voig
genmv.setVal(0.5*(low+high))
genwv.setVal(2.4)
genmv.setRange(0.5*(low+high)-1.0,0.5*(low+high)+1.0)
genwv.setRange(width-1,width+1)
s.setVal(1)
s.setRange(0,10)
frame8595voig = x.frame(ROOT.RooFit.Bins(30))
frame8595voig.SetTitle('voigt '+str(int(range1low))+'-'+str(int(range1high)))
dataset8595.plotOn(frame8595voig)
voigt.fitTo(dataset8595)
voigt.plotOn(frame8595voig)
c.cd(2)
frame8595voig.Draw()
lx.DrawLatex(0.3,0.9,'peak='+str(genmv.getVal())[0:7])
#lx.DrawLatex(0.3,0.7,'width='+str(genwv.getVal())[0:7])
#lx.DrawLatex(0.3,0.8,'sigma='+str(s.getVal())[0:7])
voiga = genmv.getVal()

#second fft
x.setRange(range2low,range2high)
x.setBins(int((range2high-range2low)*100))
m.setVal(0)
s.setVal(1)
m.setRange(-5,5)
s.setRange(0,10)
frame8696fft = x.frame(ROOT.RooFit.Bins(30))
frame8696fft.SetTitle('FFT '+str(int(range2low))+'-'+str(int(range2high)))
dataset8696.plotOn(frame8696fft)
fft.fitTo(dataset8696)
fft.plotOn(frame8696fft)
c.cd(3)
frame8696fft.Draw()
lx.DrawLatex(0.3,0.9,'peak='+str(0.5*(low+high)+m.getVal())[0:7])
#lx.DrawLatex(0.3,0.7,'width='+str(genw.getVal())[0:7])
#lx.DrawLatex(0.3,0.8,'sigma='+str(s.getVal())[0:7])
fftb = m.getVal()

#second voig
genmv.setVal(0.5*(low+high))
genwv.setVal(2.4)
genmv.setRange(0.5*(low+high)-1.0,0.5*(low+high)+1.0)
genwv.setRange(width-1,width+1)
s.setVal(1)
s.setRange(0,10)
frame8696voig = x.frame(ROOT.RooFit.Bins(30))
frame8696voig.SetTitle('voigt '+str(int(range2low))+'-'+str(int(range2high)))
dataset8696.plotOn(frame8696voig)
voigt.fitTo(dataset8696)
voigb = genmv.getVal()
voigt.plotOn(frame8696voig)
c.cd(4)
frame8696voig.Draw()
lx.DrawLatex(0.3,0.9,'peak='+str(genmv.getVal())[0:7])
#lx.DrawLatex(0.3,0.7,'width='+str(genwv.getVal())[0:7])
#lx.DrawLatex(0.3,0.8,'sigma='+str(s.getVal())[0:7])
lx.DrawLatex(0.7,0.9,'entries='+str(tot))
lx.DrawLatex(0.7,0.8,'gen peak='+str(0.5*(high+low))[0:3])
lx.DrawLatex(0.7,0.7,'gen width='+str(width)[0:4])
lx.DrawLatex(0.7,0.6,'sigma='+str(sigma)[0:4])
lx.DrawLatex(0.7,0.5,'#deltavoigt_peak='+str(voigb-voiga))
lx.DrawLatex(0.7,0.4,'#deltaFFT_peak='+str(fftb-ffta))
c.SaveAs('/eos/user/c/chenguan/www/TESTPLOTS/'+str(tot)+'_'+str(0.5*(low+high))[0:5]+'_'+str(width)[0:4]+'_'+str(width)[0:4]+'_'+str(low)[0:5]+'_'+str(high)[0:5]+'_'+str(shift_)[0:4]+'.png')
print low, high, range1low, range1high, range2low, range2high
