from ROOT import *
from array import array
import numpy

def MakeTree(fs, lumi):

    fin = TFile('/raid/raid5/dsperka/Run2/HZZ4l/SubmitArea_13TeV/Ana_ZZ4L_80X/Data_Run2016-03Feb2017_hzz4l_bestCandMela.root')
    tin = fin.Get('passedEvents')

    fout = TFile('hzz' + fs + '_' + lumi + '.root',"recreate")
    tout = TTree( 'data_obs', 'data_obs')

    CMS_zz4l_mass = numpy.zeros(1, dtype=float)#array( 'f', [ 0 ] )
    melaLD = numpy.zeros(1, dtype=float)#array( 'f', [ 0 ] )
    CMS_zz4l_massErr = numpy.zeros(1, dtype=float)#array( 'f', [ 0 ] )

    tout.Branch( 'CMS_zz4l_mass', CMS_zz4l_mass, 'CMS_zz4l_mass/D' )
    tout.Branch( 'melaLD', melaLD, 'melaLD/D' )
    tout.Branch( 'CMS_zz4l_massErr', CMS_zz4l_massErr, 'CMS_zz4l_massErr/D' )

    for i in range(tin.GetEntries()):

           tin.GetEntry(i)

#        if tin.passedFullSelection and tin.mass4l > 105 and tin.mass4l < 140:
#        if tin.mass4l > 105 and tin.mass4l < 140:

           if not tin.passedFullSelection: continue

           if fs == "4mu" and tin.finalState == 1 and tin.mass4l > 105 and tin.mass4l < 140:#tin.mass4mu > 105 and tin.mass4mu < 140: 
              CMS_zz4l_mass[0] = tin.mass4l
#              melaLD[0] = tin.melaLD
              melaLD[0] = tin.D_bkg_kin
#              CMS_zz4l_massErr[0] = tin.CMS_zz4l_massErr/tin.mass4mu
              CMS_zz4l_massErr[0] = tin.mass4lErr/tin.mass4mu
              tout.Fill()
           if fs == "4e" and tin.finalState == 2 and tin.mass4l > 105 and tin.mass4l < 140:#tin.mass4e > 105 and tin.mass4e < 140:           
              CMS_zz4l_mass[0] = tin.mass4l
#              melaLD[0] = tin.melaLD
              melaLD[0] = tin.D_bkg_kin
#              CMS_zz4l_massErr[0] = tin.CMS_zz4l_massErr/tin.mass4e
              CMS_zz4l_massErr[0] = tin.mass4lErr/tin.mass4e
              tout.Fill()
           if fs == "2e2mu" and (tin.finalState == 3 or tin.finalState == 4) and tin.mass4l > 105 and tin.mass4l < 140:#tin.mass2e2mu > 105 and tin.mass2e2mu < 140:           
              CMS_zz4l_mass[0] = tin.mass4l
#              melaLD[0] = tin.melaLD
              melaLD[0] = tin.D_bkg_kin
#              CMS_zz4l_massErr[0] = tin.CMS_zz4l_massErr/tin.mass2e2mu
              CMS_zz4l_massErr[0] = tin.mass4lErr/tin.mass2e2mu
             
              tout.Fill()              
#           melaLD[0] = tin.D_bkg_kin
#           CMS_zz4l_massErr[0] = tin.mass4l/tin.mass4lErr
#           print CMS_zz4l_mass[-1]
#           tout.Fill()
#           print CMS_zz4l_mass
    fout.Write()
    fout.Close()

MakeTree('4mu', '35.8671')
MakeTree('4e', '35.8671')
MakeTree('2e2mu', '35.8671')
