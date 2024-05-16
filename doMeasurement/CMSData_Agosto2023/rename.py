import sys
import os
import glob

files = glob.glob(sys.argv[1]+'/*root')

dict1 = {}
for f in files:
	if 'Baseline' in f:
		targetname = f.replace('Baseline','reco')
		dict1[f] = targetname
	if 'Refitted_VX_BS' in f:
		targetname = f.replace("Refitted_VX_BS","bsrefit")
		dict1[f] = targetname
	if sys.argv[1]+'_VX_BS' in f:
		targetname = f.replace("VX_BS","bs")
		dict1[f] = targetname
	
dict2 = {'_A_':'_0_', '_B_':'_1_', '_C_':'_2_', '_D_':'_3_', '_E_':'_4_', '_F_':'_5_', '_G_':'_6_', '_H_':'_7_', '_I_':'_8_'}
for i in dict1:
	for j in dict2:
		if j in i:
			dict1[i] = dict1[i].replace(j,dict2[j])
for i in dict1:
	print i, '-------->', dict1[i]
	os.system('mv '+i+' '+dict1[i])
