import ROOT
import array
from config import *
import math 

def make2dList(rows, cols):
    return [ ([0] * cols) for row in range(rows) ]

def make_FF_histo_updown(up_down,in_anti,in_id,bins,samples,file_path):
    outtitle = in_anti.split("_")
    binarray = array.array('d',bins)
    outanti = ROOT.TH1D("anti","anti",len(bins)-1,binarray)
    outid = ROOT.TH1D("id","id",len(bins)-1,binarray)
    outhisto = ROOT.TH1D(up_down+outtitle[1]+"_"+outtitle[2],up_down+outtitle[1]+"_"+outtitle[2],len(bins)-1,binarray)
    outanti.Sumw2()
    outid.Sumw2()
    outhisto.Sumw2()
    df = ROOT.TFile(file_path+"/data.root","READ")
    data_anti_before = df.Get(in_anti+"dataX")
    data_id_before = df.Get(in_id+"dataX")
    data_anti = data_anti_before.Clone()
    data_id = data_id_before.Clone()
    data_anti_new = data_anti.Rebin(len(bins)-1,"data_anti_new",binarray)
    data_id_new = data_id.Rebin(len(bins)-1,"data_id_new",binarray)
    outanti.Add(data_anti_new)
    outid.Add(data_id_new)
    files = []
    histosanti = []
    histosid = []
    for i in range(len(samples)):
        f = ROOT.TFile(file_path+"/"+samples[i]+".root","READ")
        files.append(f)
    for i in range(len(samples)):
        temp_bkg_before = files[i].Get(in_anti+samples[i])
        temp_bkg = temp_bkg_before.Clone()
	temp_bkg_new = temp_bkg.Rebin(len(bins)-1,"temp_bkg_new",binarray)
        for j in range(len(bins)-1):
            tenpercent = temp_bkg_new.GetBinContent(j+1)*0.10
            if(up_down == "DOWN_"):
                temp_bkg_new.SetBinContent(j+1,temp_bkg_new.GetBinContent(j+1) - tenpercent)
            if(up_down == "UP_"):
                temp_bkg_new.SetBinContent(j+1,temp_bkg_new.GetBinContent(j+1) + tenpercent)
        temp_bkg_new.Scale(-1)
        histosanti.append(temp_bkg_new)
    for i in range(len(samples)):
        temp_bkg_before = files[i].Get(in_id+samples[i])
	temp_bkg = temp_bkg_before.Clone()
	temp_bkg_new = temp_bkg.Rebin(len(bins)-1,"temp_bkg_new",binarray)
        for j in range(len(bins)-1):
            tenpercent = temp_bkg_new.GetBinContent(j+1)*0.10
            if(up_down == "DOWN_"):
                temp_bkg_new.SetBinContent(j+1,temp_bkg_new.GetBinContent(j+1) - tenpercent)
            if(up_down == "UP_"):
                temp_bkg_new.SetBinContent(j+1,temp_bkg_new.GetBinContent(j+1) + tenpercent)
        temp_bkg_new.Scale(-1)
        histosid.append(temp_bkg_new)
    for i in range(len(samples)):
        outanti.Add(histosanti[i])
    for i in range(len(samples)):
        outid.Add(histosid[i])
    outid.Divide(outanti)
    outhisto.Add(outid)
    outf = ROOT.TFile(file_path+"/VLL_FF_heavytau.root","UPDATE")
    outhisto.Write()
    outf.Close()
    df.Close()
    for file in files:
        file.Close()


def make_FF_histo_nominal(in_anti,in_id,bins,samples,file_path,dostat):
    outtitle = in_anti.split("_")
    binarray = array.array('d',bins)
    outanti = ROOT.TH1D("anti","anti",len(bins)-1,binarray)
    outid = ROOT.TH1D("id","id",len(bins)-1,binarray)
    outhisto = ROOT.TH1D(outtitle[0]+"_"+outtitle[1]+"_"+outtitle[2],outtitle[0]+"_"+outtitle[1]+"_"+outtitle[2],len(bins)-1,binarray)
    outhistoup = ROOT.TH1D("STATUP_"+outtitle[1]+"_"+outtitle[2],"STATUP_"+outtitle[1]+"_"+outtitle[2],len(bins)-1,binarray)
    outhistodown = ROOT.TH1D("STATDOWN_"+outtitle[1]+"_"+outtitle[2],"STATDOWN_"+outtitle[1]+"_"+outtitle[2],len(bins)-1,binarray)
    outanti.Sumw2()
    outid.Sumw2()
    outhisto.Sumw2()
    outhistoup.Sumw2()
    outhistodown.Sumw2()
    df = ROOT.TFile(file_path+"/data.root","READ")
    data_anti = df.Get(in_anti+"dataX")
    data_id = df.Get(in_id+"dataX")
    print('data id',data_id.Integral())
    print('data anti',data_anti.Integral())
    data_anti_new = data_anti.Rebin(len(bins)-1,"data_anti_new",binarray)
    data_id_new = data_id.Rebin(len(bins)-1,"data_id_new",binarray)
    outanti.Add(data_anti_new)
    outid.Add(data_id_new)
    files = []
    histosanti = []
    histosid = []
    for i in range(len(samples)):
        f = ROOT.TFile(file_path+"/"+samples[i]+".root","READ")
        files.append(f)
    for i in range(len(samples)):
        temp_bkg = files[i].Get(in_anti+samples[i])
        print(samples[i]+' anti',temp_bkg.Integral())
        temp_bkg_new = temp_bkg.Rebin(len(bins)-1,"temp_bkg_new",binarray)
        temp_bkg_new.Scale(-1)
        histosanti.append(temp_bkg_new)
    for i in range(len(samples)):
        temp_bkg = files[i].Get(in_id+samples[i])
        print(samples[i]+' id',temp_bkg.Integral())
        temp_bkg_new = temp_bkg.Rebin(len(bins)-1,"temp_bkg_new",binarray)
        temp_bkg_new.Scale(-1)
        histosid.append(temp_bkg_new)
    for i in range(len(samples)):
        outanti.Add(histosanti[i])
    for i in range(len(samples)):
        outid.Add(histosid[i])
    outid.Divide(outanti)
    outhisto.Add(outid)
    for i in range(len(bins)-1):
        outhistoup.SetBinContent(i+1,outhisto.GetBinContent(i+1)+outhisto.GetBinError(i+1))
        outhistodown.SetBinContent(i+1,outhisto.GetBinContent(i+1)-outhisto.GetBinError(i+1))
    outf = ROOT.TFile(file_path+"/VLL_FF_heavytau.root","UPDATE")
    outhisto.Write()
    if(dostat == 1):
        outhistoup.Write()
        outhistodown.Write()
    outf.Close()
    df.Close()
    for file in files:
        file.Close()
