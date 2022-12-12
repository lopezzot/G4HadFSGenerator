# G4HadFSGenerator
Geant4 test for hadron inelastic process final states.

## Description
This code is developed by Lorenzo Pezzotti on top of the Geant4 example `extended/hadronic/Hadr09/` by Alberto Ribon. It allows to sample secondaries from the hadron inelastic process for a given projectile and nucleus.

## Usage
```
./G4HadFSGenerator -pl physicslist -p projectile -e energy_GeV -m material
```
optional boolean parser options allow to save random seeds or redo a single event
```
./G4HadFSGenerator -pl physicslist -p projectile -e energy_GeV -m material -seed save_seed -redo redo_an_event
```
example, FTFP_BERT pl with 10 GeV pi- on copper without seed saving or event redoing
```
./G4HadFSGenerator -pl FTFP_BERT -p pi- -e 10 -m G4_Cu -seed 0 -redo 0
```

## Selected Presentations
- 29/11/2022, Geant4 simulation bi-weekly meeting: [**Investigation on G4HadronInelasticProcess final states**](https://indico.cern.ch/event/1226079/contributions/5158618/attachments/2556416/4405327/lopezzot_29_11_2022.pdf)
