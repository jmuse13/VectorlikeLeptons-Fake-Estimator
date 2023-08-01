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
bool singlemu_trig_passed = false;
for(auto &trig_passed : singlemu_trigs_passed)
    singlemu_trig_passed = singlemu_trig_passed || *trig_passed;
if(!singlemu_trig_passed) continue;

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
Double_t weight = genweight *  *pileup_weight * *trig_global_sf;
try {
    weight *= lumi_weights.at(*mc_channel_number).at(systematic);
} catch(std::out_of_range e) {
    try {
        weight *= lumi_weights.at(*mc_channel_number).at("NOSYS");
    } catch(std::out_of_range e) {
        std::cerr << "WARNING: Could not retrieve lumi weight for DSID " << *mc_channel_number << std::endl;
    }
}

std::vector<int> goodTrigIndex;
std::vector<float> trig_pt;
int year = 0;
if(is_data){
    if(*runNumber < 290000){
        year = 2015;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(1);
        trig_pt.push_back(30);
        trig_pt.push_back(53);
        trig_pt.push_back(1000);
    }
    if((*runNumber > 290000) && (*runNumber < 324000)){
        year = 2016;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(1);
        trig_pt.push_back(30);
        trig_pt.push_back(53);
        trig_pt.push_back(1000);
    }
    if((*runNumber > 324000) && (*runNumber < 348000)){
        year = 2017;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(1);
        trig_pt.push_back(30);
        trig_pt.push_back(53);
        trig_pt.push_back(1000);
    }
    if(*runNumber > 348000){
        year = 2018;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(1);
        trig_pt.push_back(30);
        trig_pt.push_back(53);
        trig_pt.push_back(1000);
    }
}

if(!is_data){
    if(campaign == "a"){
        year = 201516;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(1);
        trig_pt.push_back(30);
        trig_pt.push_back(53);
        trig_pt.push_back(1000);
    }
    if(campaign == "d"){
        year = 2017;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(1);
        trig_pt.push_back(30);
        trig_pt.push_back(53);
        trig_pt.push_back(1000);
    }
    if(campaign == "e"){
        year = 2018;
        goodTrigIndex.push_back(0);
        goodTrigIndex.push_back(1);
        trig_pt.push_back(30);
        trig_pt.push_back(53);
        trig_pt.push_back(1000);
    }
}

int N_tau = 0;
bool all_taus_quality = true;
bool all_taus_truthmatched = true;
for(int i = 0; i < tau_pt.GetSize(); i++){
    if(tau_pt[i] < 20e3) continue;
    if(fabs(tau_eta[i]) > 2.47) continue;
    if(fabs(tau_charge[i]) != 1) continue;
    if(tau_RNNJetScoreSigTrans[i] < 0.01) continue;

    bool quality = tau_isMedium[i];
    all_taus_quality &= quality;
    if(!quality) continue;

    N_tau++;
}

// B jets
std::vector<struct jet> jets;
Int_t N_bjets = 0;
int N_jets = 0;
for(int i = 0; i < jet_pt.GetSize(); i++) {
    if(jet_pt[i] < 20e3) continue;
    if(fabs(jet_eta[i]) > 4.5) continue;
    N_jets++;
    struct jet temp_jet;
    temp_jet.vector.SetPtEtaPhiM(jet_pt[i], jet_eta[i], jet_phi[i], jet_m[i]);
    temp_jet.pt = jet_pt[i]/1000;
    jets.push_back(temp_jet);
}
for(int i = 0; i < jet_pt.GetSize(); i++) {
    if(jet_pt[i] < 20e3) continue;
    if(fabs(jet_eta[i]) > 2.5) continue;
    if(jet_btag[i])
    N_bjets++;
}

if(N_bjets != 0) continue;
if(N_jets < 1) continue;

int highest_index = 0;
float highest_pt = 0;
for(int i = 0; i < jets.size(); i++){
    if(jets.at(i).pt > highest_pt){
        highest_index = i;
        highest_pt = jets.at(i).pt;
    }
}

int N_el = 0;
for(int i = 0; i < el_pt.GetSize(); i++){
    if(el_pt[i] < 10e3) continue;
    if(fabs(el_eta[i]) > 2.47) continue;
    if(fabs(el_eta[i]) > 1.37 && fabs(el_eta[i]) < 1.52) continue;
    if(!is_data){
        if((el_truthClassification[i] != 2) && (el_truthClassification[i] != 3)) continue;
    }
    N_el++;
}

if(N_el != 0) continue;

// Muons
std::vector<TLorentzVector> muvec;
std::vector<struct mu> mus;
std::vector<float> prescales;
for(int i = 0; i < mu_pt.GetSize(); i++) {
    if(mu_pt[i] < 10e3) continue;
    if(fabs(mu_eta[i]) > 2.5) continue;
    if(mu_isBadMuon[i] == 1) continue;
    struct mu temp_mu;
    temp_mu.pt = mu_pt[i]/1000;
    temp_mu.eta = mu_eta[i];
    if(mu_isIsolated[i] == 1){
        temp_mu.isFake = 0;
    }
    if(mu_isIsolated[i] != 1){
        temp_mu.isFake = 1;
    }
    if(!is_data){
        if(mu_truthClassification[i] == 4){
            temp_mu.isGood = 1;
        }
        if(mu_truthClassification[i] != 4){
            temp_mu.isGood = 0;
        }
    }
    if(is_data){
        temp_mu.isGood = 1;
    }
    float pre_scale = 1;
    float passedtrig = 0;
    if(!is_data){
        for(int j = 0; j < goodTrigIndex.size(); j++){
            if(mu_fired_trigs.at(goodTrigIndex.at(j))[i] == 1){
                passedtrig++;
            }
        }
    }
    std::vector<float> prescales;
    if(is_data){
        for(int j = 0; j < goodTrigIndex.size(); j++){
            if(mu_fired_trigs.at(goodTrigIndex.at(j))[i] == 1){
                passedtrig++;
                prescales.push_back(*mu_prescale_trigs.at(goodTrigIndex.at(j))){
            }
        }
        if(prescales.size() == 0){
            pre_scale = 0;
        }
        if(prescales.size() == 1){
            pre_scale = prescales.at(0);
        }
        if(prescales.size() == 2){
            float p1 = prescales.at(0);
            float p2 = prescales.at(1);
            float term1 = 1/p1;
            float term2 = 1/p2;
            float term3 = 1 - term1;
            float term4 = 1 - term2;
            float term5 = term3*term4;
            float term6 = 1 - term5;
            float outterm = 1/term6;
            pre_scale = outterm;
        }
    }
    temp_mu.firedTrig = passedtrig;
    temp_mu.prescaleTrig = pre_scale;
    temp_mu.index = i;
    mus.push_back(temp_mu);
    TLorentzVector tempmu;
    tempmu.SetPtEtaPhiM(mu_pt[i], mu_eta[i], mu_phi[i],105.6583715);
    muvec.push_back(tempmu);
    weight *= mu_effSF_reco[i] * mu_effSF_TTVA[i] * mu_effSF_isol[i];
}

if(mus.size() != 1) continue;
if(mus.at(0).firedTrig == 0) continue;

weight *= mus.at(0).prescaleTrig;

if(is_data){
    for(int j = 0; j < goodTrigIndex.size(); j++){
        if(*mu_prescale_trigs.at(goodTrigIndex.at(j)) > 0){
            float pt = mus.at(0).pt;
        }
    }
}
if(!is_data){
    for(int j = 0; j < goodTrigIndex.size(); j++){
        float pt = mus.at(0).pt;
    }
}
for(int j = 0; j < muSystematics.size(); j++){
    if(j != 1){
        if(jets.at(highest_index).pt < 35) continue;
    }
    if(j == 1){
        if(jets.at(highest_index).pt < 40) continue;
    }
    if((j != 2) && (j != 3)){
        if(fabs(muvec.at(0).DeltaPhi(jets.at(highest_index).vector)) < 2.7) continue;
    }
    if(j == 2){
        if(fabs(muvec.at(0).DeltaPhi(jets.at(highest_index).vector)) < 2.8) continue;
    }
    if(j == 3){
        if(fabs(muvec.at(0).DeltaPhi(jets.at(highest_index).vector)) < 2.6) continue;
    }
    if((j != 4) && (j != 5) && (j != 6)){
        if(MET[met_idx]/1000 >= 40) continue;
    }
    if(j == 5){
        if(MET[met_idx]/1000 >= 50) continue;
    }
    if(j == 6){
        if(MET[met_idx]/1000 >= 30) continue;
    }
    if(j == 7){
        if(!is_data){
            double tenpercent = fabs(weight * 0.1);
            weight = weight + tenpercent;
        }
    }
    if(j == 8){
        if(!is_data){
            double tenpercent = fabs(weight * 0.1);
            weight = weight - tenpercent;
        }
    }

    std::string ffmapstring = muSystematics.at(j)+"_FF_";
    if(mus.at(0).isGood == 1){
        if(mus.at(0).isFake == 0){
            ffmapstring += "pass_";
        }
        if(mus.at(0).isFake == 1){
            ffmapstring += "fail_";
        }

        std::string etastringbins[6] = {"0","0.7","1.37","1.52","2.01","2.5"};
        double etabins[6] = {0,0.7,1.37,1.52,2.01,2.5};
        int numetabins = 5;

        std::string ptstringbins[15] = {"30","31","32","33","34","35","36","38","40","42","46","55","70","90","1000"};
        double ptbins[15] = {30, 31, 32, 33, 34, 35, 36, 38, 40,42,46,55,70,90, 1000};
        int numptbins = 14;

        if(mus.at(0).pt < 30) continue;
        for(int i = 0; i < numetabins; i++){
            if((fabs(mus.at(0).eta) >= etabins[i]) && (fabs(mus.at(0).eta) < etabins[i+1])){
                ffmapstring += etastringbins[i];
                ffmapstring += "-";
                ffmapstring += etastringbins[i+1];
                ffmapstring += "_";
                ffmapstring += insample;
                muffmap.at(ffmapstring)->Fill(mus.at(0).pt,weight);
            }
        }
    }
}
