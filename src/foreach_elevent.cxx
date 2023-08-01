local_total++;
if(!is_data){
    if(*mc_channel_number==410472) continue;
    if(*mc_channel_number==410648) continue;
    if(*mc_channel_number==410649) continue;
    if(*mc_channel_number==346343) continue;
    if(*mc_channel_number==346344) continue;
    if(*mc_channel_number==346345) continue;
    if(*mc_channel_number==410560) continue;
}

bool singlee_trig_passed = false;
for(auto &trig_passed : singlee_trigs_passed)
    singlee_trig_passed = singlee_trig_passed || *trig_passed;
if(!singlee_trig_passed) continue;

std::string diboson = "diboson";
std::string multiboson = "multiboson";
std::string Zjets = "Z+jets";
std::string Wjets = "W+jets";
float genweight = 1;
if((insample == diboson) || (insample == multiboson) || (insample == Zjets) || (insample == Wjets)){
    if(fabs(*generator_weight) > 100){
        genweight *= 1; 
    } 
    else{
        genweight *= *generator_weight;
    }
}   
else{
    genweight *= *generator_weight;
}   
Double_t weight = genweight *  *pileup_weight;
try {
    weight *= lumi_weights.at(*mc_channel_number).at(systematic);
} catch(std::out_of_range e) {
    weight *= lumi_weights.at(*mc_channel_number).at("NOSYS");
}

std::vector<int> goodTrigIndex;
std::vector<float> trig_pt;
int year = 0;
if(is_data){
    if(*runNumber < 290000){
        year = 2015;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(3);
        goodTrigIndex.push_back(10);
        trig_pt.push_back(30);
        trig_pt.push_back(65);
        trig_pt.push_back(315);
        trig_pt.push_back(999);
    }
    if((*runNumber > 290000) && (*runNumber < 324000)){
        year = 2016;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(3);
        goodTrigIndex.push_back(7);
        goodTrigIndex.push_back(11);
        trig_pt.push_back(30);
        trig_pt.push_back(65);
        trig_pt.push_back(126);
        trig_pt.push_back(315);
        trig_pt.push_back(999);
    }
    if((*runNumber > 324000) && (*runNumber < 348000)){
        year = 2017;
        goodTrigIndex.push_back(1);
        goodTrigIndex.push_back(3);
        goodTrigIndex.push_back(8);
        goodTrigIndex.push_back(9);
        goodTrigIndex.push_back(11);
        trig_pt.push_back(30);
        trig_pt.push_back(65);
        trig_pt.push_back(147);
        trig_pt.push_back(168);
        trig_pt.push_back(315);
        trig_pt.push_back(999);
    }
    if(*runNumber > 348000){
        year = 2018;
        goodTrigIndex.push_back(2);
        goodTrigIndex.push_back(3);
        goodTrigIndex.push_back(4);
        goodTrigIndex.push_back(5);
        goodTrigIndex.push_back(6);
        goodTrigIndex.push_back(7);
        goodTrigIndex.push_back(8);
        goodTrigIndex.push_back(9);
        goodTrigIndex.push_back(11);
        trig_pt.push_back(30);
        trig_pt.push_back(65);
        trig_pt.push_back(75);
        trig_pt.push_back(85);
        trig_pt.push_back(105);
        trig_pt.push_back(126);
        trig_pt.push_back(147);
        trig_pt.push_back(168);
        trig_pt.push_back(315);
        trig_pt.push_back(999);
    }
}
// B jets
Int_t N_bjets = 0;
int N_jets = 0;
for(int i = 0; i < jet_pt.GetSize(); i++) {
    if(jet_pt[i] < 20e3) continue;
    if(fabs(jet_eta[i]) > 2.5) continue;
    	N_jets++;
    if(jet_btag[i]) 
        N_bjets++;
}

if(N_bjets != 0) continue;

// Muons
int N_muon = 0;
for(int i = 0; i < mu_pt.GetSize(); i++){
    if(mu_pt[i] < 10e3) continue;
    if(fabs(mu_eta[i]) > 2.5) continue;
    N_muon++;
}

if(N_muon != 0) continue;

// Electrons
std::vector<struct el> els;
for(int i = 0; i < el_pt.GetSize(); i++) {
    if(el_pt[i] < 10e3) continue;
    if(fabs(el_eta[i]) > 2.47) continue;
    if(fabs(el_eta[i]) > 1.37 && fabs(el_eta[i]) < 1.52) continue;
    struct el temp_el;
    temp_el.pt = el_pt[i]/1000;
    temp_el.eta = el_eta[i];
    if((el_likelihood[i] == 1) && (el_isIsolated[i] == 1)){
        temp_el.isFake = 0;
    }
    if((el_likelihood[i] != 1) || (el_isIsolated[i] != 1)){
        temp_el.isFake = 1;
    }
    if(!is_data){
        if((el_truthClassification[i] == 2) || (el_truthClassification[i] == 3)){
            temp_el.isGood = 1;
        }
        else{
            temp_el.isGood = 0;
        }
    }
    if(is_data){
        temp_el.isGood = 1;
    }
    float pre_scale = 1;
    float passedtrig = 0;
    if(!is_data){
        for(int j = 0; j < e_fired_trigs.size(); j++){
            if(e_fired_trigs.at(j)[i] == 1){
                passedtrig++;
            }
        }
    }
    if(is_data){
        for(int j = 0; j < goodTrigIndex.size()-1; j++){
            if(e_fired_trigs.at(goodTrigIndex.at(j))[i] == 1){
                if((temp_el.pt >= trig_pt.at(j)) && (temp_el.pt < trig_pt.at(j+1))){
                    if(*e_prescale_trigs.at(goodTrigIndex.at(j)) > 0){
                        passedtrig++;
                        pre_scale = *e_prescale_trigs.at(goodTrigIndex.at(j));
                    }
                }
            }
        }
        if((temp_el.pt >= trig_pt.at(goodTrigIndex.size()-1)) && (temp_el.pt < trig_pt.at(goodTrigIndex.size()))){
            if(*e_prescale_trigs.at(goodTrigIndex.size()-1) > 0){
                passedtrig++;
                pre_scale = *e_prescale_trigs.at(goodTrigIndex.size()-1);
            }
        }	    
    }
    temp_el.firedTrig = passedtrig;
    temp_el.prescaleTrig = pre_scale;
    els.push_back(temp_el);  
    weight *= el_effSF_reco[i] * el_effSF_PID[i] * el_effSF_isol[i];
}

if(els.size() != 1) continue;
if(els.at(0).firedTrig == 0) continue;

weight *= els.at(0).prescaleTrig;

for(int j = 0; j < elSystematics.size(); j++){
    if(j != 2){
        if(MET[met_idx]/1000 > 40) continue;
    }
    if(j != 1){
        if(N_jets < 2) continue;
    }
    if(j == 3){
        if(!is_data){
            double tenpercent = fabs(weight * 0.1);
            weight = weight + tenpercent;
        }
    }
    if(j == 4){
        if(!is_data){
            double tenpercent = fabs(weight * 0.1);
            weight = weight - tenpercent;
        }
    }

    if(els.at(0).isGood == 1){
        std::string ffmapstring = elSystematics.at(j)+"_FF_";

        if(els.at(0).isFake == 0){
            ffmapstring += "pass_";
        }
        if(els.at(0).isFake == 1){
            ffmapstring += "fail_";
        }

        std::string etastringbins[6] = {"0","0.7","1.37","1.52","2.01","2.47"};
        double etabins[6] = {0,0.7,1.37,1.52,2.01,2.47};
        int numetabins = 5;

        std::string ptstringbins[33] = {"30","32","34","36","38","40","45","50","60","70","75","80","85","90","95","100","125","150","175","200","225","250","275","300","325","350","375","400","425","450","475","500","1000"};
        double ptbins[33] = {30,32,34,36,38,40,45,50,60,70,75,80,85,90,95,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,1000};
        int numptbins = 32;

        for(int i = 0; i < numetabins; i++){
            if(i==2) continue;
            if((fabs(els.at(0).eta) >= etabins[i]) && (fabs(els.at(0).eta) < etabins[i+1])){
                ffmapstring += etastringbins[i];
                ffmapstring += "-";
                ffmapstring += etastringbins[i+1];
                ffmapstring += "_";
                ffmapstring += insample;
                elffmap.at(ffmapstring)->Fill(els.at(0).pt,weight);
            }
        }
    } 
}
