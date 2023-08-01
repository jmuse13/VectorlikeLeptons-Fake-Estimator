import ROOT

pt_plot_bins = [0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000]
pt_bins = [30,31,32,33,34,35,36,38,40,42,46,55,70,90,1000]
eta_bins = [0,0.7,1.37,1.52,2.01,2.5]
plot_bins = [30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,110,120,130,140,150,160,170,180,190,200,220,250]
samples = ["multiboson","top","diboson","Z+jets","W+jets"]
col1 = ROOT.TColor.GetFreeColorIndex()
color1 = ROOT.TColor(col1,0.24,0.35,0.50)
col2 = ROOT.TColor.GetFreeColorIndex()
color2 = ROOT.TColor(col2,0.60,0.76,0.85)
col3 = ROOT.TColor.GetFreeColorIndex()
color3 = ROOT.TColor(col3,0.88,0.98,0.99)
col4 = ROOT.TColor.GetFreeColorIndex()
color4 = ROOT.TColor(col4,0.93,0.42,0.30)
col5 = ROOT.TColor.GetFreeColorIndex()
color5 = ROOT.TColor(col5,0.16,0.20,0.25)
colors = [col5,col4,col3,col2,col1]
systematics = ["NOSYS_","JETPT_","MET_","METUP_","METDOWN_","DELPHIUP_","DELPHIDOWN_","UP_","DOWN_"]
