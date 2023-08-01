int year = 0;

singlemu_trigs_passed.emplace_back(reader,"trigPassed_HLT_mu24");
singlemu_trigs_passed.emplace_back(reader,"trigPassed_HLT_mu50");

mu_fired_trigs.emplace_back(reader,"mu_matched_HLT_mu24");
mu_fired_trigs.emplace_back(reader,"mu_matched_HLT_mu50");

if(is_data){
    mu_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_mu24");
    mu_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_mu50");
}

