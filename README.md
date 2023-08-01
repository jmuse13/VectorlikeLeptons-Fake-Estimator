# VLL Fake Lepton Estimator

## Dependencies
* CMake 3.11 or higher
* ROOT 6.14 or higher

## Quick Start
Instructions for building and running:
```
setupATLAS && lsetup cmake && lsetup root
mkdir build && cd build
cmake ..
make -j`nproc`
```

## Running the code:
In `build/bin`, there are scripts to produce histograms used for the fake factor calculation for each lepton species:
* `produce_el_ffs.sh`, for electrons
* `produce_mu_ffs.sh`, for muons
* `produce_tau_ffs.sh`, for LF taus
* `produce_heavytau_ffs.sh`, for HF taus

## Calculating fake factors
In the `python` directory, choose the folder corresponding to the appropriate fake species and run:
```
python main.py output_folder_from_produce_*_ffs
```

The output will be a ROOT file named `VLL_""_FF.root`, where "" corresponds to the fake factors just created. These files should be copied to the `vectorlike-leptons-data` directory in the `VLL Flat Ntuple Maker` tool. By default, the VLL Flat Ntuple Maker tool already includes the nominal fake factors used in the analysis.
