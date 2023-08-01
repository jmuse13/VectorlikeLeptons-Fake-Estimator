#!/usr/bin/python
import sys
import ROOT
from config import *
from FFCR import *

ROOT.gROOT.SetBatch()

outf = ROOT.TFile(sys.argv[1]+"/VLL_FF_el.root","RECREATE")
outf.Close()

##############################################################################################################
######################## nominal FFCR ########################################################################
##############################################################################################################

for systematic in systematics:
    for i in range(len(eta_bins)-1):
        if eta_bins[i] == 1.37:
            continue
        in_anti = systematic + 'FF_' + 'fail_' + str(eta_bins[i])+'-' + str(eta_bins[i+1]) + '_'
        in_id = systematic + 'FF_' + 'pass_' + str(eta_bins[i])+'-' + str(eta_bins[i+1]) + '_'
        if(systematic == 'UP_'):
            in_anti_new = 'NOSYS_FF_' + 'fail_' + str(eta_bins[i])+'-' + str(eta_bins[i+1]) + '_'
            in_id_new = 'NOSYS_FF_' + 'pass_' + str(eta_bins[i])+'-' + str(eta_bins[i+1]) + '_'
            make_FF_histo_updown(systematic,in_anti_new,in_id_new,pt_bins,samples,sys.argv[1])
        if(systematic == 'DOWN_'):
            in_anti_new = 'NOSYS_FF_' + 'fail_' + str(eta_bins[i])+'-' + str(eta_bins[i+1]) + '_'
            in_id_new = 'NOSYS_FF_' + 'pass_' + str(eta_bins[i])+'-' + str(eta_bins[i+1]) + '_'
            make_FF_histo_updown(systematic,in_anti_new,in_id_new,pt_bins,samples,sys.argv[1])
        if(systematic != 'UP_' and systematic != 'DOWN_'): 
            make_FF_histo_nominal(in_anti,in_id,pt_bins,samples,sys.argv[1])
