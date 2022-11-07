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
         << "    this tests is based on example Hadr09                      " << G4endl;

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
  analysisManager->CreateH1("Momentum","Momentum",2000,-0.2,0.2);
 
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

  G4VParticleChange* aChange = nullptr;
  G4int nsec;
  G4double mresidual;
    

  for (int i=0; i<600; i++){
    aChange = theHadronicGenerator->GenerateInteraction( projectile, projectileEnergy,
                                                       aDirection, material );
  
    nsec = aChange ? aChange->GetNumberOfSecondaries() : 0;
    G4double mresidual = dParticle.GetTotalMomentum()/CLHEP::GeV;
    G4TrackStatus leadStatus = aChange->GetTrackStatus();
    if (leadStatus != 2){
        G4cout<<"NOT KILLED!"<<G4endl;
        std::abort();
    }
    for (std::size_t j=0; j<nsec; j++){
        mresidual = mresidual - aChange->GetSecondary(j)->GetDynamicParticle()->Get4Momentum()[2]/CLHEP::GeV; 
    }
    G4cout<<mresidual<<" GeV"<<G4endl;
    analysisManager->FillH1(0, mresidual);
  }

  analysisManager->Write();
  analysisManager->CloseFile();






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
    G4cout<<"correct end"<<G4endl;  
}

//**************************************************
