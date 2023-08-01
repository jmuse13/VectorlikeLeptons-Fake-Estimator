#!/usr/bin/python

import sys
import ROOT
from config import *
from FFCR import *

ROOT.gROOT.SetBatch()

outf = ROOT.TFile(sys.argv[1]+"/VLL_FF_heavytau.root","RECREATE")
outf.Close()

##############################################################################################################
for systematic in systematics:
    for prong in prongs:
        in_anti = systematic + "FF_" + prong + "_fail_tau_"
        in_id = systematic + "FF_" + prong + "_pass_tau_"

        if(prong == "1p"):
            if(systematic == 'UP_'):
                in_anti_new = "NOSYS_FF_" + prong + "_fail_tau_"
                in_id_new = "NOSYS_FF_" + prong + "_pass_tau_"
                make_FF_histo_updown(systematic,in_anti_new,in_id_new,ff_bins,samples,sys.argv[1])
                if(systematic == 'DOWN_'):
                    in_anti_new = "NOSYS_FF_" + prong + "_fail_tau_"
                    in_id_new = "NOSYS_FF_" + prong + "_pass_tau_"
                    make_FF_histo_updown(systematic,in_anti_new,in_id_new,ff_bins,samples,sys.argv[1])
                if(systematic != 'UP_' and systematic != 'DOWN_' and systematic != 'NOSYS_'):
                    make_FF_histo_nominal(in_anti,in_id,ff_bins,samples,sys.argv[1],0)
                if(systematic != 'UP_' and systematic != 'DOWN_' and systematic == 'NOSYS_'):
                    make_FF_histo_nominal(in_anti,in_id,ff_bins,samples,sys.argv[1],1)
        if(prong == "3p"):
            if(systematic == 'UP_'):
                in_anti_new = "NOSYS_FF_" + prong + "_fail_tau_"
                in_id_new = "NOSYS_FF_" + prong + "_pass_tau_"
                make_FF_histo_updown(systematic,in_anti_new,in_id_new,ff_bins,samples,sys.argv[1])
            if(systematic == 'DOWN_'):
                in_anti_new = "NOSYS_FF_" + prong + "_fail_tau_"
                in_id_new = "NOSYS_FF_" + prong + "_pass_tau_"
                make_FF_histo_updown(systematic,in_anti_new,in_id_new,ff_bins,samples,sys.argv[1])
            if(systematic != 'UP_' and systematic != 'DOWN_' and systematic != 'NOSYS_'):
                make_FF_histo_nominal(in_anti,in_id,ff_bins,samples,sys.argv[1],0)
            if(systematic != 'UP_' and systematic != 'DOWN_' and systematic == 'NOSYS_'):
                make_FF_histo_nominal(in_anti,in_id,ff_bins,samples,sys.argv[1],1)
