//**************************************************
// \file G4HadFSGenerator.cc
// \brief: main() of G4HadFSGenerator test
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 7 November 2022
//**************************************************

#include <iomanip>
#include "globals.hh"
#include "G4ios.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4VParticleChange.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "HadronicGenerator.hh"
#include "G4GenericIon.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4AnalysisManager.hh"

int main( int , char** ) {
  
  G4cout << "=== Using HadronicGenerator for final states sampling test, ===" << G4endl
         << "    this test is based on example Hadr09                      " << G4endl;

  // See the HadronicGenerator class for the possibilities and meaning of the "physics cases".
  // ( In short, it is the name of the Geant4 hadronic model used for the simulation of
  //   the collision, with the possibility of having a transition between two models in
  //   a given energy interval, as in physics lists. )
  const G4String namePhysics = "FTFP_BERT";        //***LOOKHERE***  PHYSICS CASE
  //const G4String namePhysics = "FTFP_BERT_ATL";
  //const G4String namePhysics = "QGSP_BERT";
  //const G4String namePhysics = "QGSP_BIC";
  //const G4String namePhysics = "FTFP_INCLXX";
  //const G4String namePhysics = "FTFP";
  //const G4String namePhysics = "QGSP";
  //const G4String namePhysics = "BERT";
  //const G4String namePhysics = "BIC";
  //const G4String namePhysics = "IonBIC";
  //const G4String namePhysics = "INCL";
  
  //Set primary particle
  //
  G4ParticleDefinition* projectile = G4Proton::Proton();
  G4ThreeVector aDirection = G4ThreeVector( 0.0, 0.0, 1.0 ); 
  G4double projectileEnergy = 100*CLHEP::GeV; 
  G4DynamicParticle dParticle( projectile, aDirection, projectileEnergy );

  //Set material
  //
  G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial( "G4_N" );

  //Create root output file
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("had.root");
  analysisManager->CreateH1("Momentum_conservation","Momentum_conservation",2000,-0.2,0.2);
  analysisManager->CreateH1("Neutron_kenergy","Neutron_kenergy",1000,0.0,200.);
  
 
  //Printout the configuration
  //
  G4cout << G4endl
         << "=================  Configuration ==================" << G4endl
         << "Model: " << namePhysics << G4endl
         << "Projectile: " << projectile->GetParticleName() << G4endl
         << "Ekin: " << projectileEnergy/CLHEP::GeV << " GeV" << G4endl
         << "Etot: " << dParticle.GetTotalEnergy()/CLHEP::GeV << " GeV" << G4endl
         << "Momentum: " << dParticle.GetTotalMomentum()/CLHEP::GeV << " GeV" << G4endl
         << "Material: " << material->GetName() << G4endl
         << "===================================================" << G4endl
         << G4endl;

  //The HadronicGenerator from Hadr09 example
  //
  HadronicGenerator* theHadronicGenerator = new HadronicGenerator( namePhysics );

  //Variables of interest
  //
  G4VParticleChange* aChange = nullptr;
  std::size_t events = 10000;
  G4int nsecondaries;
  G4double mz_conservation;
  G4double neutron_kenergy;
    
  for (std::size_t i=0; i<events; i++){
      
    aChange = theHadronicGenerator->GenerateInteraction( projectile, projectileEnergy,
                                                         aDirection, material );
  
    nsecondaries = aChange ? aChange->GetNumberOfSecondaries() : 0;

    //Initial momentum along z
    //
    mz_conservation = dParticle.GetTotalMomentum()/CLHEP::GeV;

    //Check is primary is killed, otherwise abort
    //
    G4TrackStatus leadStatus = aChange->GetTrackStatus();
    if (leadStatus != 2){
        G4cout<<"PRIMARY NOT KILLED!"<<G4endl;
        std::abort();
    }
    for (G4int j=0; j<nsecondaries; j++){

      //Get dynamic particle
      //
      auto particle = aChange->GetSecondary(j)->GetDynamicParticle();

      //Compute momentum conservation along z
      //
      mz_conservation = mz_conservation - particle->Get4Momentum()[2]/CLHEP::GeV;

      //Add kinetic energy of neutrons
      //
      if ( particle->GetDefinition() == G4Neutron::Neutron() ){
        
        neutron_kenergy += particle->GetKineticEnergy()/CLHEP::GeV;

      } 
    
    }

    analysisManager->FillH1(0, mz_conservation);
    analysisManager->FillH1(1, neutron_kenergy);

    neutron_kenergy = 0.;
  }

  //Clonse and write output file
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  G4cout<<"The end."<<G4endl;  

  /*
  if ( true ) {
      G4cout << G4endl << "\t --> #secondaries=" << nsec 
             << " ; impactParameter[fm]=" << theHadronicGenerator->GetImpactParameter() / fermi
	     << " ; #projectileSpectatorNucleons=" << theHadronicGenerator->GetNumberOfProjectileSpectatorNucleons()
	     << " ; #targetSpectatorNucleons=" << theHadronicGenerator->GetNumberOfTargetSpectatorNucleons()
	     << " ; #NNcollisions=" << theHadronicGenerator->GetNumberOfNNcollisions() << G4endl;
      G4cout << "\t \t List of produced secondaries: " << G4endl;
      // Loop over produced secondaries and eventually print out some information.
      for ( G4int j = 0; j < nsec; ++j ) {
        const G4DynamicParticle* sec = aChange->GetSecondary(j)->GetDynamicParticle();
        G4cout << "\t \t \t j=" << j << "\t" << sec->GetDefinition()->GetParticleName()
               << "\t p=" << sec->Get4Momentum() << " MeV" << G4endl;
        delete aChange->GetSecondary(j);
      }
  }
  */
}

//**************************************************
