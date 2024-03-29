//**************************************************
// \file G4HadFSGenerator.cc
// \brief: main() of G4HadFSGenerator test
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 7 November 2022
//**************************************************

#include "G4GenericIon.hh"
#include "G4IonTable.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VParticleChange.hh"
#include "G4Version.hh"
#include "G4ios.hh"
#include "HadronicGenerator.hh"
#include "globals.hh"
#include <algorithm>
#include <iomanip>
#include <iostream>
#if G4VERSION_NUMBER < 1100
#include "g4root.hh" // replaced by G4AnalysisManager.h  in G4 v11 and up
#else
#include "G4AnalysisManager.hh"
#endif
#include "G4NucleiProperties.hh"
#include <cmath>
#include <filesystem>

namespace pl {
std::vector<G4String> list{"FTFP_BERT", "FTFP_BERT_ATL", "QGSP_BERT",
                           "QGSP_BIC",  "FTFP_INCLXX",   "FTFP",
                           "QGSP",      "BERT",          "BIC",
                           "IonBIC",    "INCL"};
}

namespace CLIoutput {
void PrintError() {
  G4cerr << "Wrong usage. Options:\n"
         << "-pl physicslist (FTFP_BERT)\n"
         << "-p particle (proton)\n"
         << "-e energy_geV (100)\n"
         << "-m g4material (G4_Fe)\n"
         << "-seed 1/0 (optional)\n"
         << "-redo 1/0 (optional)\n"
         << G4endl;
}
} // namespace CLIoutput

int main(int argc, char **argv) {

  G4cout << "=== Using HadronicGenerator for final states sampling test, ==="
         << G4endl
         << "    this test is based on example Hadr09.                      "
         << G4endl;
  G4cout << "=== Using Geant4: " << G4VERSION_NUMBER << G4endl;
  // See the HadronicGenerator class for the possibilities and meaning of the
  // "physics cases". ( In short, it is the name of the Geant4 hadronic model
  // used for the simulation of
  //   the collision, with the possibility of having a transition between two
  //   models in a given energy interval, as in physics lists. )

  // Initial variables (physics list, particle, energy, material)
  //
  G4String namePhysics;
  G4String nameProjectile;
  G4double energyProjectile;
  G4String nameMaterial;
  G4bool saveRandomStatus = false;
  G4bool redoEvent = false;

  // CLI variables
  //
  if (argc == 1) {
    CLIoutput::PrintError();
    return 1;
  }
  for (G4int i = 1; i < argc; i = i + 2) {
    if (G4String(argv[i]) == "-pl")
      namePhysics = argv[i + 1];
    else if (G4String(argv[i]) == "-p")
      nameProjectile = argv[i + 1];
    else if (G4String(argv[i]) == "-e")
      energyProjectile = G4UIcommand::ConvertToDouble(argv[i + 1]);
    else if (G4String(argv[i]) == "-m")
      nameMaterial = argv[i + 1];
    else if (G4String(argv[i]) == "-seed")
      saveRandomStatus = G4UIcommand::ConvertToInt(argv[i + 1]);
    else if (G4String(argv[i]) == "-redo")
      redoEvent = G4UIcommand::ConvertToInt(argv[i + 1]);
    else {
      CLIoutput::PrintError();
      return 1;
    }
  }

  // Check namePhysics is in physicslists
  //
  if (std::find(pl::list.begin(), pl::list.end(), namePhysics) ==
      pl::list.end()) {
    G4cerr << namePhysics << " is not in: " << G4endl;
    for (auto &i : pl::list) {
      G4cout << i << G4endl;
    }
    return 1;
  }

  // The HadronicGenerator from Hadr09 example
  //
  HadronicGenerator *theHadronicGenerator = new HadronicGenerator(namePhysics);

  // Set primary particle
  //
  G4ParticleTable *partTable = G4ParticleTable::GetParticleTable();
  partTable->SetReadiness();
  G4ParticleDefinition *projectile = partTable->FindParticle(nameProjectile);
  G4ThreeVector aDirection = G4ThreeVector(0.0, 0.0, 1.0); // along z
  G4double projectileEnergy = energyProjectile * CLHEP::GeV;
  G4DynamicParticle dParticle(projectile, aDirection, projectileEnergy);

  // Set material, get nuclear mass and binding energy
  //
  G4Material *material =
      G4NistManager::Instance()->FindOrBuildMaterial(nameMaterial);
  G4NucleiProperties NucleiProperties;
  const G4Element *element = material->GetElement(0);
  G4double nuclearMass =
      NucleiProperties.GetNuclearMass(element->GetN(), element->GetZ());
  G4double bindingEnergy =
      NucleiProperties.GetBindingEnergy(element->GetN(), element->GetZ());

  // Create root output file
  //
  auto analysisManager = G4AnalysisManager::Instance();
  G4String nameOutput = namePhysics + nameProjectile +
                        std::to_string(energyProjectile).substr(0, 4) +
                        nameMaterial + ".root";
  analysisManager->OpenFile(nameOutput);
  analysisManager->CreateH1("Momentum_conservation", "Momentum_conservation",
                            2000, -0.02, 0.02);
  analysisManager->CreateH1("Neutron_kenergy", "Neutron_kenergy", 1000, 0.0,
                            1.1 * energyProjectile);
  analysisManager->CreateH1("Pi0_energy", "Pi0_energy", 1000, 0.0,
                            1.1 * energyProjectile);
  analysisManager->CreateH1("E_loss", "E_loss", 500, -1.0,
                            2.0 * bindingEnergy / CLHEP::GeV);
  analysisManager->CreateH1("Pi-_Pz", "Pi-_Pz", 100, -1.2 * energyProjectile,
                            1.2 * energyProjectile);
  analysisManager->CreateH1("Pi-_Pz_wPt", "Pi-_Pz_wPt", 100,
                            -1.2 * energyProjectile, 1.2 * energyProjectile);

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  CLHEP::HepRandom::setTheSeed(123);

  // Printout the configuration
  //
  G4cout << G4endl
         << "=================  Configuration ==================" << G4endl
         << "Model: " << namePhysics << G4endl
         << "Projectile: " << projectile->GetParticleName() << G4endl
         << "Ekin: " << projectileEnergy / CLHEP::GeV << " GeV" << G4endl
         << "Etot: " << dParticle.GetTotalEnergy() / CLHEP::GeV << " GeV"
         << G4endl << "Momentum: " << dParticle.GetTotalMomentum() / CLHEP::GeV
         << " GeV" << G4endl << "Material: " << material->GetName() << G4endl
         << "Nuclear Mass: " << nuclearMass / CLHEP::GeV << " GeV" << G4endl
         << "Binding Energy: " << bindingEnergy / CLHEP::GeV << " GeV" << G4endl
         << "===================================================" << G4endl
         << G4endl;

  // Variables of interest
  //
  G4VParticleChange *aChange = nullptr;
  std::size_t startEvent = 0;
  std::size_t events = 100000;
  G4int nsecondaries;
  G4double mz_conservation;
  G4double neutron_kenergy = 0.;
  G4double pizero_energy = 0.;
  G4double e_loss;

  if (redoEvent) {
    G4cout << "which event: " << G4endl;
    std::cin >> startEvent;
    events = startEvent + 1;
  }

  for (std::size_t i = startEvent; i < events; i++) {

    if (saveRandomStatus && (redoEvent == false)) {
      std::string fileName = "event_" + std::to_string(i) + "rndm.stat";
      CLHEP::HepRandom::getTheEngine()->saveStatus(fileName.c_str());
    }
    if (redoEvent) {
      G4cout << "Redoing event: " << i << G4endl;
      std::string fileName = "event_" + std::to_string(i) + "rndm.stat";
      CLHEP::HepRandom::getTheEngine()->restoreStatus(fileName.c_str());
    }

    aChange = theHadronicGenerator->GenerateInteraction(
        projectile, projectileEnergy, aDirection, material);

    nsecondaries = aChange ? aChange->GetNumberOfSecondaries() : 0;

    // Initial momentum along z
    //
    mz_conservation = dParticle.GetTotalMomentum() / CLHEP::GeV;

    // Initial particle energy (total energy for mesons, kinetic energy for
    // baryons)
    //
    if (dParticle.GetDefinition()->GetBaryonNumber() >= 1) {
      e_loss = dParticle.GetKineticEnergy() / CLHEP::GeV;
    } else {
      e_loss = dParticle.GetTotalEnergy() / CLHEP::GeV;
    }

    // Check is primary is killed, otherwise abort
    //
    G4TrackStatus leadStatus = aChange->GetTrackStatus();
    if (leadStatus != 2) {
      G4cout << "PRIMARY NOT KILLED!" << G4endl;
      std::abort();
    }

    for (G4int j = 0; j < nsecondaries; j++) {

      // Get dynamic particle
      //
      auto particle = aChange->GetSecondary(j)->GetDynamicParticle();

      // Printout with redo command true
      //
      if (redoEvent) {
        G4cout << " particle: " << particle->GetDefinition()->GetParticleName()
               << " momentum (MeV): " << particle->GetTotalMomentum()
               << " energy (MeV): " << particle->GetTotalEnergy()
               << " k energy (MeV): " << particle->GetKineticEnergy() << G4endl;
      }

      // Compute momentum conservation along z,
      //
      mz_conservation =
          mz_conservation - particle->Get4Momentum()[2] / CLHEP::GeV;

      // Compute energy lost to release nucleons
      // how: kinetic energy projectile - kinetic energy of nucleons (p and n)
      // - total energy of mesons - kinetic energy of nuclear fragments (baryon
      // number > 1)
      //
      if (particle->GetDefinition()->GetBaryonNumber() >= 1) {
        e_loss = e_loss - particle->GetKineticEnergy() / CLHEP::GeV;
      } else {
        e_loss = e_loss - particle->GetTotalEnergy() / CLHEP::GeV;
      }

      // Add kinetic energy of neutrons, pi0
      //
      if (particle->GetDefinition() == G4Neutron::Neutron()) {

        neutron_kenergy += particle->GetKineticEnergy() / CLHEP::GeV;
      }
      if (particle->GetDefinition() == G4PionZero::PionZero()) {

        pizero_energy += particle->GetTotalEnergy() / CLHEP::GeV;
      }

      // Fill h1 pi- pz and pt
      //
      if (particle->GetDefinition() == G4PionMinus::PionMinus()) {

        analysisManager->FillH1(4, particle->Get4Momentum()[2] / CLHEP::GeV);
        G4double pt =
            std::sqrt(std::pow(particle->GetMomentum()[0] / CLHEP::GeV, 2) +
                      std::pow(particle->GetMomentum()[1] / CLHEP::GeV, 2));
        analysisManager->FillH1(5, particle->Get4Momentum()[2] / CLHEP::GeV,
                                pt);
      }
    }

    analysisManager->FillH1(0, mz_conservation);
    analysisManager->FillH1(1, neutron_kenergy);
    analysisManager->FillH1(2, pizero_energy);
    analysisManager->FillH1(3, e_loss);
    if (saveRandomStatus) {
      G4cout << "event " << i << " e_loss " << e_loss << G4endl;
    }

    neutron_kenergy = 0.;
    pizero_energy = 0.;
    aChange = nullptr;
  }

  // Close and write output file
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  G4cout << "The end." << G4endl;
}

//**************************************************
