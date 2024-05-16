import os

data_dirs = ['/timestamp_14_58_12_122885_2016pre_data_allbins_muon_ebe_withvxbs/', 'timestamp_15_03_19_345807_2016post_data_allbins_muon_ebe_withvxbs/', 'timestamp_15_03_57_875088_2017_data_allbins_muon_ebe_withvxbs/', 'timestamp_15_08_34_924918_2018_data_allbins_muon_ebe_withvxbs/']
mc_dirs = ['/24_Mar_2023/timestamp_18_10_39_603679_2016post_mc_allbins_muon_ebe_withvxbs/','/24_Mar_2023/timestamp_18_10_31_907527_2016pre_mc_allbins_muon_ebe_withvxbs/','/24_Mar_2023/timestamp_18_10_21_979717_2017_mc_allbins_muon_ebe_withvxbs/','/24_Mar_2023/timestamp_18_10_14_865893_2018_mc_allbins_muon_ebe_withvxbs/']

# for data_dirs, each element add a prefix '/27_Mar_2023/'

for i in range(len(data_dirs)):
    data_dirs[i] = '/27_Mar_2023/' + data_dirs[i]

# add a prefix '/eos/user/c/chenguan/www/TESTPLOTS/' on two lists
for i in range(len(data_dirs)):
    data_dirs[i] = '/eos/user/c/chenguan/www/TESTPLOTS/' + data_dirs[i]
    mc_dirs[i] = '/eos/user/c/chenguan/www/TESTPLOTS/' + mc_dirs[i]

# run2muclosure.py element as argu
for i in range(len(data_dirs)):
    os.system('python run2muclosure.py ' + mc_dirs[i])
