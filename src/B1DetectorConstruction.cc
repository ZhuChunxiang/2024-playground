//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeX = 30*cm, env_sizeY = 20*cm, env_sizeZ = 200*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeX = 1.2*env_sizeX;
  G4double world_sizeY = 1.2*env_sizeY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeX, 0.5*env_sizeY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  //     
  // Tracker
  //  
  G4Material* tracker_mat = nist->FindOrBuildMaterial("G4_S");
  G4double distance =  30.*cm;
        
  // Tracker size
  G4double tracker_x =  10.*cm, tracker_y =  20.*cm, tracker_z =  0.1*mm;
  G4Box* solidTracker1 =
    new G4Box("Tracker1",
              0.5*tracker_x, 0.5*tracker_y, 0.5*tracker_z);
                      
  G4LogicalVolume* logicShape1 =                         
    new G4LogicalVolume(solidTracker1,         //its solid
                        tracker_mat,          //its material
                        "Tracker1");           //its name

  for (int i = 0; i < 7; i++)
    {
        new G4PVPlacement(0,                       //no rotation
                          G4ThreeVector(0, 0, (3-i)*distance),                    //at position
                          logicShape1,             //its logical volume
                          "Tracker1",                //its name
                          logicEnv,                //its mother  volume
                          false,                   //no boolean operation
                          i,                       //copy number
                          checkOverlaps);          //overlaps checking
    }
//  //
//  // Shape 2
//  //
//  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
//  G4ThreeVector pos2 = G4ThreeVector(0, -1*cm, 7*cm);
//
//  // Trapezoid shape
//  G4double shape2_dxa = 12*cm, shape2_dxb = 12*cm;
//  G4double shape2_dya = 10*cm, shape2_dyb = 16*cm;
//  G4double shape2_dz  = 6*cm;
//  G4Trd* solidShape2 =
//    new G4Trd("Shape2",                      //its name
//              0.5*shape2_dxa, 0.5*shape2_dxb,
//              0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz); //its size
//
//  G4LogicalVolume* logicShape2 =
//    new G4LogicalVolume(solidShape2,         //its solid
//                        shape2_mat,          //its material
//                        "Shape2");           //its name
//
//  new G4PVPlacement(0,                       //no rotation
//                    pos2,                    //at position
//                    logicShape2,             //its logical volume
//                    "Shape2",                //its name
//                    logicEnv,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking
                
  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicShape1;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
