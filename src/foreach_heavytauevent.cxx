local_total++;

if(*mc_channel_number==410470) continue;
if(*mc_channel_number==410471) continue;
if(*mc_channel_number==410658) continue;
if(*mc_channel_number==410659) continue;
if(*mc_channel_number==410646) continue;
if(*mc_channel_number==410647) continue;
if(*mc_channel_number==410644) continue;
if(*mc_channel_number==410645) continue;

// Trigger
bool mm_trig_passed = false;
for(auto &trig_passed : mm_trigs_passed)
    mm_trig_passed = mm_trig_passed || *trig_passed;
if(!mm_trig_passed) continue;

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

if(N_jets == 0) continue;
weight *= *jet_btag_eff;

// Electrons
std::vector<TLorentzVector> goodLep;
std::vector<int> goodEl;
int charge = 0;
for(int i = 0; i < el_pt.GetSize(); i++){
    if(!el_likelihood[i]) continue;
    if(!el_isIsolated[i]) continue;
    if(el_pt[i] < 20e3) continue;
    if(fabs(el_eta[i]) > 2.47) continue;
    if(fabs(el_pt[i]) > 1.37 && fabs(el_pt[i]) < 1.52) continue;
    weight *= el_effSF_reco[i] * el_effSF_PID[i] * el_effSF_isol[i];
    goodEl.push_back(i);
    TLorentzVector ele1;
    ele1.SetPtEtaPhiM(el_pt[i], el_eta[i], el_phi[i], 0.511);
    charge += el_charge[i];   
    goodLep.push_back(ele1);
}

// Muons
std::vector<int> goodMuon;
for(int i = 0; i < mu_pt.GetSize(); i++){
    if(!mu_isIsolated[i]) continue;
    if(mu_pt[i] < 20e3) continue;
    if(fabs(mu_eta[i]) > 2.5) continue;
    weight *= mu_effSF_reco[i] * mu_effSF_TTVA[i] * mu_effSF_isol[i];
    goodMuon.push_back(i);
    TLorentzVector muon1;
    muon1.SetPtEtaPhiM(mu_pt[i], mu_eta[i], mu_phi[i], 105.66);
    charge += mu_charge[i];
    goodLep.push_back(muon1);
}

if(goodLep.size() != 2) continue;
if((goodMuon.size() == 2 && goodEl.size()) != 0) continue;
if((goodEl.size() == 2 && goodMuon.size()) != 0) continue;
if(charge != 0) continue;

int on_z_peak = 0;
int on_z_peak_var = 0;

for(int i = 0; i < goodLep.size(); i++){
    for(int j = 1; j < goodLep.size(); j++){
        float mll = (goodLep.at(i) + goodLep.at(j)).M();
        if(abs(mll - 91000) < 10000){
            on_z_peak++;
        }
        if(abs(mll - 91000) < 7000){
            on_z_peak_var++;
        }
    }
}
// Tau
std::vector<struct tau> taus;
for(int i = 0; i < tau_pt.GetSize(); i++){
    if(tau_EleBDT[i] == 0) continue;
    if(tau_pt[i] < 20e3) continue;
    if(tau_passEleOLR[i] == 0) continue;
    if(fabs(tau_eta[i]) > 2.47) continue;
    if(fabs(tau_eta[i]) > 1.37 && fabs(tau_eta[i]) < 1.52) continue;
    if(fabs(tau_charge[i]) != 1) continue;
    struct tau temp_tau;
    temp_tau.rnn = tau_RNNJetScoreSigTrans[i];
    temp_tau.prong = tau_nTracks[i];
    temp_tau.pt = tau_pt[i]/1000;
    temp_tau.eta = tau_eta[i];
    temp_tau.rnnscore = tau_RNNJetScoreSigTrans[i];
    if(tau_isMedium[i] == 1){
        temp_tau.isFake = 0;
        weight *= tau_effSF_Medium[i];
    }   
    if(tau_isMedium[i] == 0){
        temp_tau.isFake = 1;
        weight *= tau_effSF_Base[i];
    }
    if(!is_data){
       if((tau_truthClassification[i] == 0) || (tau_truthClassification[i] == 4) || (tau_truthClassification[i] == 5) || (tau_truthClassification[i] == 7)){
            temp_tau.truth_label = TAU;
        }
        if(tau_truthClassification[i] == 1){
            temp_tau.truth_label = LIGHT;
        }
        if(tau_truthClassification[i] == 2){
            temp_tau.truth_label = HEAVY;
        }
        if(tau_truthClassification[i] == 3){
            temp_tau.truth_label = GLUON;
        }
        if(tau_truthClassification[i] == 6){
            temp_tau.truth_label = PILEUP;
        }
    }
    if(is_data){
        temp_tau.truth_label = DATA;
    }
    taus.push_back(temp_tau); 
}
if(taus.size() != 1) continue;
for(int i = 0; i < htauSystematics.size(); i++){
    if(i == 1){
        if(on_z_peak_var != 0) continue;
    }
    if(i != 1){
        if(on_z_peak != 0) continue;
    }
    if(i != 2){
        if(N_bjets == 0) continue;
    }
    if(i != 5){
        if(taus.at(0).rnnscore  < 0.01) continue;
    }
    if(i == 5){
        if(taus.at(0).rnnscore  < 0.005) continue;
    }
    std::string ffmapstring = htauSystematics.at(i)+"_FF_";
    int prongness = taus.at(0).prong;

    switch(prongness){
        case 0:
            break;
        case 1:
            ffmapstring += "1p_";
            break;
        case 2:
            break;
        case 3:
            ffmapstring += "3p_";
            break;
        default:
            break;
    }

    if(taus.at(0).isFake == 0){
        ffmapstring += "pass_";
    }

    if(taus.at(0).isFake == 1){
        ffmapstring += "fail_";
    }

    int truth_label_enum = taus.at(0).truth_label;
    std::vector<float> tauptbinsout = {20,30,40,60,90,150,999};
    std::vector<std::string> tauptbinsoutstring = {"20_","30_","40_","60_","90_","150_"};
    switch(truth_label_enum){
        case 0:
            ffmapstring += "tau_" + insample;
            if(i == 3){
                double tenpercent = fabs(weight * 0.10);
                weight = weight + tenpercent;
            }
            if(i == 4){
                double tenpercent = fabs(weight * 0.10);
                weight = weight - tenpercent;
            }
            mutex.lock();
            tauffmap.at(ffmapstring)->Fill(taus.at(0).pt,weight);
            mutex.unlock();
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            ffmapstring += "tau_" + insample;
            mutex.lock();
            tauffmap.at(ffmapstring)->Fill(taus.at(0).pt);
            mutex.unlock();
            break;
        default:
            break;
    }
}

