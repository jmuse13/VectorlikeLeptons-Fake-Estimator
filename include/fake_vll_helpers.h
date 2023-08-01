#ifndef __fake_vll_helpers_h_
#define __fake_vll_helpers_h_

#include <map>
#include <string>
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <iterator>
#include <mutex>
#include "TEfficiency.h"

typedef enum {TAU, LIGHT, HEAVY, GLUON, PILEUP, DATA} TAU_TRUTH_LABEL;

struct lightlep;
    int charge;
    float pt;
    float eta;
    float phi;
    float mass;
};
struct tau {
    int prong;
    float pt;
    float eta;
    float rnnscore;
    int isFake;
    float rnn;
    TLorentzVector vector;
    int charge;
    TAU_TRUTH_LABEL truth_label;
};

struct el {
    float pt;
    float eta;
    float phi;
    int isFake;
    int isGood;
    int firedTrig;
    float prescaleTrig;
    TLorentzVector vector;
    int charge;
};

struct mu {
    float pt;
    float eta;
    float phi;
    int isFake;
    int isGood;
    int firedTrig;
    int index;
    float prescaleTrig;
    TLorentzVector vector;
    int charge;
};
struct lep {
    float pt;
    float eta;
    float phi;
    int isFake;
    int isGood;
    int firedTrig;
    float prescaleTrig;
    TLorentzVector vector;
    int charge;
};

struct jet {
    float pt;
    int index;
    TLorentzVector vector;
};

void write_tau_histos(std::map<std::string,TH1D*> ffmap, std::string output){
    std::mutex mutex;
    TFile *outFile = new TFile(output.c_str(),"RECREATE");
    std::map<std::string, TH1D*>::iterator it = ffmap.begin();
    for(std::pair<std::string, TH1D*> element : ffmap){
        mutex.lock();
        element.second->Write();
        mutex.unlock();
    }
    outFile->Close();
}    


void write_emu_histos(std::map<std::string,TH1D*> rnnmap, std::string output){
    std::mutex mutex;
    TFile *outFile = new TFile(output.c_str(),"RECREATE");
    std::map<std::string, TH1D*>::iterator itrnnscore = rnnmap.begin();
    for(std::pair<std::string, TH1D*> elementrnnscore : rnnmap){
        mutex.lock();
        elementrnnscore.second->Write();
        mutex.unlock();
    }
    outFile->Close();
}

#endif


