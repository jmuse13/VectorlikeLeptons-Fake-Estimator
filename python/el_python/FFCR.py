import ROOT
import array
from config import *
import math

def make_FF_histo_updown(up_down,in_anti,in_id,bins,samples,file_path):
    outtitle = in_anti.split("_")
    binarray = array.array('d',bins)
    outanti = ROOT.TH1D("anti","anti",len(bins)-1,binarray)
    outid = ROOT.TH1D("id","id",len(bins)-1,binarray)
    outhisto = ROOT.TH1D(up_down+outtitle[1]+"_"+outtitle[2]+"_"+outtitle[3],up_down+outtitle[1]+"_"+outtitle[2]+"_"+outtitle[3],len(bins)-1,binarray)
    outanti.Sumw2()
    outid.Sumw2()
    outhisto.Sumw2()
    df = ROOT.TFile(file_path+"/data.root","READ")
    data_anti_before = df.Get(in_anti+"dataX")
    data_id_before = df.Get(in_id+"dataX")
    data_anti = data_anti_before.Clone()
    data_id = data_id_before.Clone()
    outanti.Add(data_anti)
    outid.Add(data_id)
    files = []
    histosanti = []
    histosid = []
    for i in range(len(samples)):
        f = ROOT.TFile(file_path+"/"+samples[i]+".root","READ")
        files.append(f)
    for i in range(len(samples)):
        temp_bkg_before = files[i].Get(in_anti+samples[i])
        temp_bkg = temp_bkg_before.Clone()
        for j in range(len(bins)-1):
            tenpercent = temp_bkg.GetBinContent(j+1)*0.10
            if(up_down == "DOWN_"):
                temp_bkg.SetBinContent(j+1,temp_bkg.GetBinContent(j+1) - tenpercent)
            if(up_down == "UP_"):
                temp_bkg.SetBinContent(j+1,temp_bkg.GetBinContent(j+1) + tenpercent)
        temp_bkg.Scale(-1)
        histosanti.append(temp_bkg)
    for i in range(len(samples)):
        temp_bkg_before = files[i].Get(in_id+samples[i])
        temp_bkg = temp_bkg_before.Clone()
        for j in range(len(bins)-1):
            tenpercent = temp_bkg.GetBinContent(j+1)*0.10
            if(up_down == "DOWN_"):
                temp_bkg.SetBinContent(j+1,temp_bkg.GetBinContent(j+1) - tenpercent)
            if(up_down == "UP_"):
                temp_bkg.SetBinContent(j+1,temp_bkg.GetBinContent(j+1) + tenpercent)
        temp_bkg.Scale(-1)
        histosid.append(temp_bkg)
    for i in range(len(samples)):
        outanti.Add(histosanti[i])
    for i in range(len(samples)):
        outid.Add(histosid[i])
    outid.Divide(outanti)
    outhisto.Add(outid)
    outhisto.GetXaxis().Set(len(bins)-1,binarray)
    outf = ROOT.TFile(file_path+"/VLL_FF_el.root","UPDATE")
    outhisto.Write()
    outf.Close()
    df.Close()
    for file in files:
        file.Close()

def make_FF_histo_nominal(in_anti,in_id,bins,samples,file_path):
    outtitle = in_anti.split("_")
    binarray = array.array('d',bins)
    outanti = ROOT.TH1D("anti","anti",len(bins)-1,binarray)
    outid = ROOT.TH1D("id","id",len(bins)-1,binarray)
    outhisto = ROOT.TH1D(outtitle[0]+"_"+outtitle[1]+"_"+outtitle[2]+"_"+outtitle[3],outtitle[0]+"_"+outtitle[1]+"_"+outtitle[2]+"_"+outtitle[3],len(bins)-1,binarray)
    outanti.Sumw2()
    outid.Sumw2()
    outhisto.Sumw2()
    df = ROOT.TFile(file_path+"/data.root","READ")
    data_anti = df.Get(in_anti+"dataX")
    data_id = df.Get(in_id+"dataX")
    outanti.Add(data_anti)
    outid.Add(data_id)
    files = []
    histosanti = []
    histosid = []
    for i in range(len(samples)):
        f = ROOT.TFile(file_path+"/"+samples[i]+".root","READ")
        files.append(f)
    for i in range(len(samples)):
        temp_bkg = files[i].Get(in_anti+samples[i])
        temp_bkg.Scale(-1)
        histosanti.append(temp_bkg)
    for i in range(len(samples)):
        temp_bkg = files[i].Get(in_id+samples[i])
        temp_bkg.Scale(-1)
        histosid.append(temp_bkg)
    for i in range(len(samples)):
        outanti.Add(histosanti[i])
    for i in range(len(samples)):
        outid.Add(histosid[i])
    outid.Divide(outanti)
    outhisto.Add(outid)
    outhisto.GetXaxis().Set(len(bins)-1,binarray)
    outf = ROOT.TFile(file_path+"/VLL_FF_el.root","UPDATE")
    outhisto.Write()
    outf.Close()
    df.Close()
    for file in files:
        file.Close()
