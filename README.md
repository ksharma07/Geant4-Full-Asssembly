# Project Map and Execution Order

## Flow at a glance
```
main                       (neutron_collimator.cc)
  ↓ setup run manager/UI
ActionInitialization       (src/ActionInitialization.cc)
  ↓ registers actions
RunAction::BeginOfRunAction
  ↓ open output<runID>.root (ntuples)
for each event
  EventAction::BeginOfEventAction  ↓ reset
  PrimaryGeneratorAction           ↓ make primary
  tracking/SteppingAction          ↓ fill per-step data into EventAction
  EventAction::EndOfEventAction    ↓ fTree->Fill() → eventoutput.root
RunAction::EndOfRunAction          ↓ write/close ntuples; tree → runoutput.root
```

## High-level flow
1. `main`: [neutron_collimator.cc](neutron_collimator.cc)
   - Seeds RNG, creates G4 run manager, registers initialization classes, starts UI or batch macro.
2. `ActionInitialization`: [src/ActionInitialization.cc](src/ActionInitialization.cc)
   - Wires user actions: `PrimaryGeneratorAction`, `RunAction`, `EventAction`, `SteppingAction`.
3. Run sequence (per `/run/beamOn N`):
   - `RunAction::BeginOfRunAction` opens `output<runID>.root` for G4AnalysisManager ntuples.
   - For each event `0..N-1`:
     - `EventAction::BeginOfEventAction` resets per-event accumulators.
     - `PrimaryGeneratorAction::GeneratePrimaries` creates one neutron primary vertex.
     - Tracking/stepping: `SteppingAction::UserSteppingAction` updates `EventAction` fields based on volume/energy loss.
     - `EventAction::EndOfEventAction` prints summary and `fTree->Fill()` into `eventoutput.root`.
   - `RunAction::EndOfRunAction` writes/closes G4AnalysisManager ntuples and fills the small `tree` in `runoutput.root`.

## Key components
- Geometry: [src/DetectorConstruction.cc](src/DetectorConstruction.cc) (builds volumes and placements).
- Physics list: [src/physics.hh](src/physics.hh) (custom `MyPhysicsList`).
- Primary generator: [src/PrimaryGeneratorAction.cc](src/PrimaryGeneratorAction.cc) with `n_particle=1` (one neutron per event) and random source position/direction.
- Per-run actions: [src/RunAction.cc](src/RunAction.cc) / [include/RunAction.hh](include/RunAction.hh)
  - Sets up G4AnalysisManager ntuples (`output<runID>.root`, IDs 0–4) and a manual ROOT `TTree("tree")` in `runoutput.root`.
- Per-event actions: [src/EventAction.cc](src/EventAction.cc) / [include/EventAction.hh](include/EventAction.hh)
  - Opens `eventoutput.root`, defines `TTree("Events")` with branches for hit counts, energies, times, positions, kill counters, etc.; resets per event and fills at end.
- Per-step actions: [src/SteppingAction.cc](src/SteppingAction.cc) / [include/SteppingAction.hh](include/SteppingAction.hh)
  - Runs each step; detects volume/particle, accumulates energy/time/position into `EventAction` vectors/counters, sets first-hit metadata, updates kill counters.

## Outputs
- `output<runID>.root`: G4AnalysisManager ntuples from `RunAction` (IDs 0–4).
- `eventoutput.root`: Manual per-event `Events` tree from `EventAction`.
- `runoutput.root`: Manual `tree` with `totalEvents` from `RunAction` destructor.
- Optional analysis macro: [rootanalysis/evtreehisto.C](rootanalysis/evtreehisto.C) reads `eventoutput*.root` and produces histograms into `hist.root`.

## Typical run
```
# from build directory after cmake config
make -j4
./neutron_collimator           # interactive; uses gui macros
./neutron_collimator run.mac    # batch macro containing /run/beamOn N
```
