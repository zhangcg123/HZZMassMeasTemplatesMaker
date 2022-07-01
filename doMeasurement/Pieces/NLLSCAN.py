import sys
import os
t2w = 'text2workspace.py '+sys.argv[1]+' -P HiggsAnalysis.CombinedLimit.FloatingHiggsWidth:floatingHiggsWidth --PO higgsWidthRange='+sys.argv[2]+','+sys.argv[3]+' --PO higgsMassRange=115,135 -o width_range_'+sys.argv[2]+'_'+sys.argv[3]+'.root'
comb='combineTool.py -M FastScan -w width_range_'+sys.argv[2]+'_'+sys.argv[3]+'.root:w'
dir_name = '/eos/user/c/chenguan/www/NLLSCAN/'+sys.argv[1].split('.')[0]+'_'+sys.argv[2]+'_'+sys.argv[3]
os.system(t2w)
os.system(comb)
os.system('mkdir -p '+dir_name)
os.system('cp /eos/user/c/chenguan/www/index.php '+dir_name+'/')
os.system('pdftoppm -png nll.pdf nll.png')
os.system('mv nll.png* '+dir_name)
