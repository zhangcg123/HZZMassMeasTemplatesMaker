#rm OutPutTrees/*root
#rm Dump/*log
#for i in /eos/cms/store/group/phys_higgs/cmshzz4l/xBF/Run2/UL/MC/2018/*;do echo $i; python Run.py $i;done
#for i in /eos/user/f/ferrico/www/FullRun2/20165/*; do echo $i; python Run.py $i;done 
for i in /eos/user/f/ferrico/www/New_Sample_Marzo2023/GluGluHToZZTo4L_M125*root;do echo $i;python Run.py $i;done
