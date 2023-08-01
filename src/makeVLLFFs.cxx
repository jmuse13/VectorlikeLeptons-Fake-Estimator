#include "vll_helpers.h"
#include "fake_vll_helpers.h"

#include "TChain.h"
#include "TDirectory.h"
#include "TEfficiency.h"
#include "TH2.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TThread.h"
#include "ROOT/TTreeProcessorMT.hxx"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TEfficiency.h"

#include <getopt.h>

using namespace std;

void read_metadata(std::unordered_map<Int_t,std::unordered_map<std::string,Double_t>> *lumiweight_map, std::string campaign, std::string derivation, std::string argv0) {
    (*lumiweight_map)[0]["NOSYS"] = 1.0;

    std::string bindir = argv0.substr(0,argv0.find_last_of("/"));
    std::string sumweightsFile_in = bindir + "/../vectorlike-leptons-data/" + derivation + "_sumweights_MC16" + campaign + ".txt";
    std::string pyamiFile_in = bindir + "/../vectorlike-leptons-data/" + derivation + "_metadata_MC16" + campaign + ".txt";
    float lum = 1.0;
    if(campaign.compare("a") == 0)
        lum = 36207.66;
    else if(campaign.compare("d") == 0)
        lum = 44307.4;
    else if(campaign.compare("e") == 0)
        lum = 58450.1;
    else 
        std::cerr << "MC campaign declaration incorrect.  Events will not be normalized to correct luminosity." << std::endl;

    std::ifstream pyami_file(pyamiFile_in.c_str());
    std::ifstream sumweights_file(sumweightsFile_in.c_str());
    if(!pyami_file.is_open()) {
        std::cerr << "Failed to open " << pyamiFile_in << std::endl;
        std::exit(1);
    }
    if(!sumweights_file.is_open()) {
        std::cerr << "Failed to open " << sumweightsFile_in << std::endl;
        std::exit(1);
    }
    std::unordered_map<Int_t,Double_t> xsec_map;
    std::string line;
    while(std::getline(pyami_file,line)) {
        if(line.length() == 0 || line.at(0) == '#') continue;
        std::stringstream ss(line);
        int DSID = -1;
        double cross_section, k_factor, filt_efficiency, events;
        ss >> DSID >> cross_section >> k_factor >> filt_efficiency >> events;
        if(DSID <= 0) continue;
        xsec_map.emplace(DSID,cross_section*k_factor*lum*filt_efficiency);
    }
    while(std::getline(sumweights_file,line)) {
        if(line.length() == 0 || line.at(0) == '#') continue;
        std::stringstream ss(line);
        int DSID = -1;
        std::string systematic;
        double sum_of_weights;
        ss >> DSID >> systematic >> sum_of_weights;
        if(DSID <= 0) continue;
        if(xsec_map.find(DSID) == xsec_map.end()) {
            std::cerr << "WARNING: DSID " << DSID << " was found in "+sumweightsFile_in+" but not "+pyamiFile_in+".  MC events for this DSID will not be normalized correctly." << std::endl;
        }
        (*lumiweight_map)[DSID][systematic] = xsec_map[DSID]/sum_of_weights;
    }
}

void create_tau_FF_histos(std::map<std::string,TH1D*> *ffmap, std::string sample,std::vector<std::string> intauSystematics, std::unordered_map<std::string,bool> inSystematics, int do_systematics){
    std::mutex mutex;
    std::vector<std::string> prongs = {"1p_", "3p_"};
    std::vector<std::string> pass = {"pass_", "fail_"};
    std::vector<std::string> flavor = {"tau_"};
    if(do_systematics == 1){
        for(int m = 0; m < intauSystematics.size(); m++){
            for(int i = 0; i < prongs.size(); i++){
                for(int j = 0; j < pass.size(); j++){
                    for(int k = 0; k < flavor.size(); k++){
                        std::string histo = intauSystematics.at(m) + "_FF_" + prongs.at(i) + pass.at(j) + flavor.at(k) + sample;
                        mutex.lock();
                        ffmap->insert(std::pair<std::string,TH1D*>(histo, new TH1D(histo.c_str(), histo.c_str(), 1000,0,1000)));
                        mutex.unlock();
                        ffmap->at(histo)->Sumw2();
                    }
                }
            }
        }
    }
}

void create_el_FF_histos(std::map<std::string,TH1D*> *ffmap, std::vector<std::string> inSystematics,std::string sample){
    std::mutex mutex;
    double ptbins[33] = {30,32,34,36,38,40,45,50,60,70,75,80,85,90,95,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,1000};
    int numptbins = 32;
    std::string etastrbins[6] = {"0","0.7","1.37","1.52","2.01","2.47"};
    int numetabins = 5;
    std::vector<std::string> pass = {"pass_", "fail_"};
    for(int i = 0; i < inSystematics.size(); i++){
        for(int j = 0; j < pass.size(); j++){
            for(int k = 0; k < numetabins; k++){
                std::string histo = inSystematics.at(i) + "_FF_" + pass.at(j) + etastrbins[k] + "-" + etastrbins[k+1] + "_"+sample;
                mutex.lock();
                ffmap->insert(std::pair<std::string,TH1D*>(histo, new TH1D(histo.c_str(), histo.c_str(), numptbins,ptbins)));
                mutex.unlock();
                ffmap->at(histo)->Sumw2();
            }
        }
    }
}

void create_mu_FF_histos(std::map<std::string,TH1D*> *ffmap,std::vector<std::string> inSystematics,std::string sample){
    std::mutex mutex;
    double ptbins[15] = {30,31,32,33,34,35,36,38,40,42,46,55,70,90,1000};
    int numptbins = 14;
    std::string etabins[6] = {"0","0.7","1.37","1.52","2.01","2.5"};
    int numetabins = 5;
    std::vector<std::string> pass = {"pass_", "fail_"};
    for(int i = 0; i < inSystematics.size(); i++){
        for(int j = 0; j < pass.size(); j++){
            for(int k = 0; k < numetabins; k++){
                std::string histo = inSystematics.at(i) + "_FF_" + pass.at(j) + etabins[k] + "-" + etabins[k+1] + "_"+sample;
                mutex.lock();
                ffmap->insert(std::pair<std::string,TH1D*>(histo, new TH1D(histo.c_str(), histo.c_str(),970,30,1000)));
                mutex.unlock();
                ffmap->at(histo)->Sumw2();
            }
        }
    }
}

void print_help(const char *argv0) {
    std::cout << "usage: " << argv0 << " -i <INPUT1> -i <INPUT2> -i ... -o <OUTPUT>\n\n" <<
        "Specify as many inputs as you want, inputs can be ntuples or directories containing ntuples that are recursively scanned.\n" <<
        "Output is a ROOT file.  If not specified a default will be created as ./out.root\n" <<
        "--keep-fake-taus to keep MC events with one or more non-truth-matched taus.  By default these events are vetoed.\n" <<
        "--veto-real-taus to veto MC events with no non-truth-matched taus.  By default these events are kept.\n" <<
        "--systematics or -s to run over systematic variations.  By default the NOSYS variation is only used.\n" <<
        "--threads X or -t X to force the program to use no more than X threads.\n" <<
        "-N X to limit the run to X events for debugging purposes.  The output events will likely not be normalized properly.\n" <<
        "--is-mc or -m if running over MC (enables MC weights and scale factors, on by default).\n" <<
        "--is-data or -d if running over data (disables MC weights and scale factors)." << std::endl;
}

int main(int argc, char *argv[]) {
    // Argument parsing
    int do_systematics = 0;
    int do_systematics_FAKE = 1;
    int is_data = 0;
    int nevents = 0;
    int make_fake_tau_ff = 0;
    int make_fake_heavytau_ff = 0;
    int make_fake_el_ff = 0;
    int make_fake_mu_ff = 0;
    std::string campaign = "null";
    std::string insample = "null";
    struct option long_options[] = {
        {"mccampaign",required_argument,0,'p'},
        {"input",required_argument,0,'i'},
        {"output",required_argument,0,'o'},
        {"make-heavy-fake-tau-ff",no_argument,&make_fake_heavytau_ff,1},
        {"make-fake-tau-ff",no_argument,&make_fake_tau_ff,1},
        {"make-fake-el-ff",no_argument,&make_fake_el_ff,1},
        {"make-fake-mu-ff",no_argument,&make_fake_mu_ff,1},
        {"samplename",required_argument,0,'g'},
        {"systematics",no_argument,&do_systematics,1},
        {"systematics_fake",no_argument,&do_systematics_FAKE,1},
        {"is-mc",no_argument,&is_data,0},
        {"is-data",no_argument,&is_data,1},
        {"threads",required_argument,0,'t'},
        {"help",no_argument,0,'h'},
        {0,0,0,0}
    };
    int option_index = 0;
    int c = 0;
    std::vector<std::string> inputs;
    std::string output;
    int nthreads = 0;
    while((c = getopt_long(argc,argv,"p:i:o:fbeug:samdt:N:h",long_options,&option_index)) != -1) {
        switch(c) {
            case 0:
                break;
            case 'p':
                campaign = optarg;
                break;
            case 'i':
                inputs.push_back(optarg);
                break;
            case 'o':
                output = optarg;
                break;
            case 'f':
                make_fake_tau_ff = 1;
                break;    
            case 'b':
                make_fake_heavytau_ff = 1;
                break;
            case 'e':
                make_fake_el_ff = 1;
                break;
            case 'u':
                make_fake_mu_ff = 1;
                break;
            case 'g':
                insample = optarg;
                break;
            case 's':
                do_systematics = 1;
                break;
            case 'a':
                do_systematics_FAKE = 1;
                break;
            case 'm':
                is_data = 0;
                break;
            case 'd':
                is_data = 1;
                break;
            case 't':
                nthreads = atoi(optarg);
                break;
            case 'N':
                nevents = atoi(optarg);
                break;
            case 'h':
                print_help(argv[0]);
                exit(0);
            default:
                print_help(argv[0]);
                exit(1);
        }
    }

    if(inputs.empty()) {
        std::cerr << "ERROR: No inputs specified." << std::endl;
        std::exit(1);
    }
    if(is_data) {
        if(campaign.compare("1516") == 0) campaign = "a";
        else if(campaign.compare("17") == 0) campaign = "d";
        else if(campaign.compare("18") == 0) campaign = "e";
    }
    if( (campaign.compare("a") != 0) && (campaign.compare("d") != 0) && (campaign.compare("e") != 0)) {
        std::cerr << "ERROR: Invalid campaign \"" << campaign << "\"." << std::endl;
        std::exit(1);
    }
    if(output.empty()) output = "out.root";
    if(nevents < 0) nevents = 0;
    if(nthreads <= 0) nthreads = std::thread::hardware_concurrency();

    if(insample == "WZ"){
	insample = "diboson";
    }
    if(insample == "ZZ"){
        insample = "diboson";
    }
    if(insample == "WW"){
        insample = "diboson";
    }
    std::map<std::string,TH1D*> tauffmap;
    std::map<std::string,TH1D*> elffmap;
    std::map<std::string,TH1D*> muffmap;
    std::vector<std::string> elSystematics;
    std::vector<std::string> muSystematics;
    std::vector<std::string> tauSystematics;
    std::vector<std::string> htauSystematics;
    std::string derivation = "";

    if(make_fake_tau_ff == 1){
        tauSystematics.push_back("NOSYS");
        if(do_systematics_FAKE == 1){
            tauSystematics.push_back("INVM");
            tauSystematics.push_back("NUMBJETS");
            tauSystematics.push_back("MET");
            tauSystematics.push_back("UP");
            tauSystematics.push_back("DOWN");
            tauSystematics.push_back("RNN");
        }
        create_tau_FF_histos(&tauffmap, insample, tauSystematics,vll_helpers::systematics, do_systematics_FAKE);
        derivation += "EXOT12";
    }

    if(make_fake_heavytau_ff == 1){
        htauSystematics.push_back("NOSYS");
        if(do_systematics_FAKE == 1){
            htauSystematics.push_back("INVM");
            htauSystematics.push_back("NUMBJETS");
            htauSystematics.push_back("UP");
            htauSystematics.push_back("DOWN");
            htauSystematics.push_back("RNN");
        }
        create_tau_FF_histos(&tauffmap, insample, htauSystematics,vll_helpers::systematics, do_systematics_FAKE);
        derivation += "EXOT12";
    }

    if(make_fake_el_ff == 1){
        elSystematics.push_back("NOSYS");   
        if(do_systematics_FAKE == 1){
            elSystematics.push_back("NUMJETS");
            elSystematics.push_back("MET");
            elSystematics.push_back("UP");
            elSystematics.push_back("DOWN");
        }
        create_el_FF_histos(&elffmap, elSystematics,insample);
        derivation += "EXOT19";        
    }

    if(make_fake_mu_ff == 1){
	    muSystematics.push_back("NOSYS");
        if(do_systematics_FAKE == 1){
            muSystematics.push_back("JETPT");
            muSystematics.push_back("DELPHIUP");
            muSystematics.push_back("DELPHIDOWN");
            muSystematics.push_back("MET");
            muSystematics.push_back("METUP");
            muSystematics.push_back("METDOWN");
            muSystematics.push_back("UP");
            muSystematics.push_back("DOWN");
        }
        create_mu_FF_histos(&muffmap, muSystematics,insample);
        derivation += "EXOT22";
    }
    std::unordered_map<Int_t,std::unordered_map<std::string,Double_t>> lumi_weights;
    read_metadata(&lumi_weights, campaign,derivation,argv[0]);

    std::unique_ptr<std::vector<std::string>> in_files = vll_helpers::open_root_files(inputs);
    std::unique_ptr<TChain> in_chain(new TChain("physics"));
    for(const auto &in_file : *in_files)
        in_chain->Add(in_file.c_str());

    std::cout << "*** Input ROOT files ***" << std::endl;
    for(const auto &in_file : *in_files)
        std::cout << "\t" << in_file << std::endl;
    std::unique_ptr<TFile> out_file(new TFile(output.c_str(),"RECREATE"));
    std::cout << std::endl << "*** Output ROOT file ***" << std::endl;
    std::cout << "\t" << output << std::endl;

    int nsystematics = 1;
    if(do_systematics) {
        vll_helpers::get_systematics(in_chain.get());
        nsystematics = vll_helpers::systematics.size()+1;  // Include NOSYS for counting reasons but don't count it when printing out.
        std::cout << "Running over " << nsystematics-1 << " systematics." << std::endl;
    }

    gROOT->ProcessLine("gErrorIgnoreLevel = kWarning;");
    if(nevents == 0 || nevents > in_chain->GetEntries()) nevents = in_chain->GetEntries();
    vll_helpers::ProgressBar progress_bar(nevents*nsystematics/10000);

    // Event loop
    Int_t met_idx = vll_helpers::get_met_array_index(in_chain.get());
    ROOT::EnableImplicitMT(nthreads);
    ROOT::TTreeProcessorMT processor(*in_chain);
    std::mutex mutex;
    unsigned long int total = 0;
    if(nevents == 0 || nevents > in_chain->GetEntries()) nevents = in_chain->GetEntries();
    std::cout << "Running over " << nevents << " events." << std::endl;
    progress_bar.start();
    auto event_loop = [&](TTreeReader &reader) {
        unsigned long int this_reader_start = reader.GetCurrentEntry();
        if(nevents != 0 && total > nevents*nsystematics) return;
        // We'll keep track of event weights while processing weight systematics in this vector: think of it as a 2D array where the first index is event number (for this TTreeReader) and the second index is the weight systematic's name.
        std::vector<std::unordered_map<std::string,Double_t>> weight_systematics;
        auto do_event_loop = [&](const std::string &systematic) {
            reader.Restart();
            reader.SetEntry(this_reader_start);
            #include "define_ntuple_branches.cxx"
            if(make_fake_tau_ff == 1){
                #include "define_tauntuple_branches.cxx"
            }
            if(make_fake_heavytau_ff == 1){
                #include "define_tauntuple_branches.cxx"
            }
            if(make_fake_el_ff == 1){
                #include "define_elntuple_branches.cxx"
            }
            if(make_fake_mu_ff == 1){
                #include "define_muntuple_branches.cxx"
            }

            bool doing_nominal = systematic.compare("NOSYS") == 0;
            // The boolean associated to this systematic from the vll_helpers::systematics map was set by the constructors of MyReaderValue/MyReaderArray so it has to be retrieved after the constructors have all been called.
            bool is_weight_syst = !doing_nominal && vll_helpers::systematics.at(systematic);
            unsigned long int local_total = 0;
            while(reader.Next()) {
                if(make_fake_tau_ff == 1){
                    #include "foreach_tauevent.cxx"
                }
                if(make_fake_heavytau_ff == 1){
                    #include "foreach_heavytauevent.cxx"
                }
                if(make_fake_el_ff == 1){
                    #include "foreach_elevent.cxx"
                }
                if(make_fake_mu_ff == 1){
                    #include "foreach_muevent.cxx"
                }
            }
            int n_progressbar_updates = local_total/10000;
            mutex.lock();
            if((local_total%10000) + (total%10000) >= 10000) n_progressbar_updates++;
            total += local_total;
            mutex.unlock();
            for(int i = 0; i < n_progressbar_updates; i++)
                progress_bar.update_progress();
        };  // Done with do_event_loop
        // Process systematics first so all weight systematics are done before nominal run.
        for(auto it : vll_helpers::systematics)
            do_event_loop(it.first);
        // By this point, the non-weight systemaics should all have their trees created and filled, the nominal trees should have branches created for each systematic weight, and the weight_systematics map should be filled.
        do_event_loop("NOSYS");
    };
    processor.Process(event_loop);
    progress_bar.stop();
    std::cout << "Processed " << total/nsystematics << " events." << std::endl;

    if(make_fake_tau_ff == 1){
        write_tau_histos(tauffmap, output);
    }
    if(make_fake_heavytau_ff == 1){
        write_tau_histos(tauffmap, output);
    }
    if(make_fake_el_ff == 1){
        write_emu_histos(elffmap, output);
    }
    if(make_fake_mu_ff == 1){
        write_emu_histos(muffmap, output);
    }

    sleep(5);
    return 0;
}

