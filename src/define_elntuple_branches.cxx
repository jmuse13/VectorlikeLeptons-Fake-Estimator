int year = 0;

singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e26_lhvloose_nod0_L1EM20VH");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e28_lhvloose_nod0_L1EM20VH");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e28_lhvloose_nod0_L1EM22VH");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e60_lhvloose_nod0");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e70_lhvloose_nod0");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e80_lhvloose_nod0");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e100_lhvloose_nod0");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e120_lhvloose_nod0");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e140_lhvloose_nod0");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e160_lhvloose_nod0");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e200_etcut");
singlee_trigs_passed.emplace_back(reader,"trigPassed_HLT_e300_etcut");

e_fired_trigs.emplace_back(reader,"el_matched_HLT_e26_lhvloose_nod0_L1EM20VH");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e28_lhvloose_nod0_L1EM20VH");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e28_lhvloose_nod0_L1EM22VH");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e60_lhvloose_nod0");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e70_lhvloose_nod0");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e80_lhvloose_nod0");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e100_lhvloose_nod0");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e120_lhvloose_nod0");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e140_lhvloose_nod0");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e160_lhvloose_nod0");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e200_etcut");
e_fired_trigs.emplace_back(reader,"el_matched_HLT_e300_etcut");

if(is_data){
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e26_lhvloose_nod0_L1EM20VH");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e28_lhvloose_nod0_L1EM20VH");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e28_lhvloose_nod0_L1EM22VH");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e60_lhvloose_nod0");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e70_lhvloose_nod0");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e80_lhvloose_nod0");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e100_lhvloose_nod0");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e120_lhvloose_nod0");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e140_lhvloose_nod0");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e160_lhvloose_nod0");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e200_etcut");
    e_prescale_trigs.emplace_back(reader,"trigPrescale_HLT_e300_etcut");
}
