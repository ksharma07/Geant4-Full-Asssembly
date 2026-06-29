#include "DetectorConstruction.hh"
#include "DetectorMap.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include <fstream> 

DetectorConstruction::DetectorConstruction()
{
  auto runManager = G4RunManager::GetRunManager();

  auto MessengerL1Thickness = new G4GenericMessenger(this, "/detector/", "L1Thickness");
  MessengerL1Thickness->DeclareProperty("L1Thickness", L1Thickness, "Collimator thickness");

  auto MessengerL4Thickness = new G4GenericMessenger(this, "/detector/", "L4Thickness");
  MessengerL4Thickness->DeclareProperty("L4Thickness", L4Thickness, "Collimator thickness");

  runManager->GeometryHasBeenModified();
  G4cout << "GEOMETRY CHANGED" << G4endl;
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{

  Materials *materials = new Materials();                                          // User-defined materials
  G4NistManager *nist = G4NistManager::Instance();                                 // NIST-defined materials
  CCalMaterialFactory *matfact = CCalMaterialFactory::getInstance("material.cms"); // Materials from materials.cms database
  ///Switch on-off these construction
  bool room = true;
  bool sipm = true;
  bool pcb = true;
  bool heat = false;
  bool frontback = true;//layer1 frame
  bool mount = false;
  bool shield = false;
  bool detshield = false;
  bool gamshield = false;

  // Experimental hall material and size
  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
  //G4Material *worldMat = materials->FindMaterial("Vacuum");

  G4double xWorld = 6.0 * m;
  G4double yWorld = 6.0 * m;
  G4double zWorld = 6.0 * m;

  auto *ExperimentalHall = new G4Box("ExperimentalHall", xWorld, yWorld, zWorld);
  ExpHall_log = new G4LogicalVolume(ExperimentalHall, worldMat, "ExpHall_log");
  auto *ExpHall_phys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), ExpHall_log, "ExpHall_phys", 0, false, 0, true);

  if(room){
    G4Material *roomMat = nist->FindOrBuildMaterial("G4_CONCRETE");
    G4double gap = 1 * cm; // leave tiny clearance to avoid touching world boundary
    G4double outerX = xWorld - gap;
    G4double outerY = yWorld - gap;
    G4double outerZ = zWorld - gap;
    G4double wall = 1 * m;
    G4double innerX = outerX - wall;
    G4double innerY = outerY - wall;
    G4double innerZ = outerZ - wall;
    auto *roomOuter = new G4Box("roomOuter", outerX, outerY, outerZ);
    auto *roomInner = new G4Box("roomInner", innerX, innerY, innerZ);
    auto *roomShell = new G4SubtractionSolid("roomShell", roomOuter, roomInner);
    auto *logicRoom = new G4LogicalVolume(roomShell, roomMat, "logicRoom");
    new G4PVPlacement(0, G4ThreeVector(), logicRoom, "physRoom", ExpHall_log, false, 0, true);
    auto *roomVisAtt = new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 0.2));
    roomVisAtt->SetForceSolid(false);
    logicRoom->SetVisAttributes(roomVisAtt);
  }

  ////////////////////////////////////////////////////////////////
  // Define L1 bar detector
  G4double dettp = 0.5;
  G4double barSizeCm = 0.67;//cm
  G4double barLengthCm = 0.75;//cm
  G4double detectorZPos = 5 * barSizeCm + 1;//layer1ThicknessHalfCm * 2 + layer2ThicknessHalfCm * 2 + layer3ThicknessHalfCm * 2 + layer4ThicknessHalfCm * 2 +
  G4Material *detMat = materials->FindMaterial("eljen232q");
  
  //auto *detectorBar = new G4Box("detectorBar", 2* (barLengthCm / 2) * cm, 3 *(0.5 / 2) * cm, 10* (barSizeCm / 2) * cm);
  //logicDetector = new G4LogicalVolume(detectorBar, detMat, "logicDetector");
  //auto *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., detectorZPos * cm), logicDetector, "physDetector", ExpHall_log, false, true);
  //auto *detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, dettp));
  //detVisAtt->SetForceSolid(true);
  //logicDetector->SetVisAttributes(detVisAtt);
  G4cout << "Detector Z-Location is: " << detectorZPos << G4endl;
  
  //////////////////////////////////////////////////////////////////
  G4double L1rows=10;
  
  G4double L1barwidth = 5.;//half length in mm
  G4double L1barlen = 7.5;//half length in mm
  G4double L1bardepth = 6.7;
  G4double detZpos = 5 * L1bardepth + 10.0;
  G4double angleB1B2=(32.0)*deg;
  G4double L1totalLen=L1rows*L1bardepth;//not used

  G4double L2rad=128.7;
  G4double barwidth = 7.5;
  //G4double zL1=(L2rad+L2thick/2*barwidth)/tan(angleB1B2)-L1rows/2*L1barwidth;//not used  
  
  G4Box *layer1bar = new G4Box("layer1bar",(L1barlen/2.)*mm,L1barwidth/2.*mm,L1bardepth/2.*mm);
  logicLayer1 = new G4LogicalVolume(layer1bar,detMat,"logicLayer1");
  std::ofstream outputFile("layer1_positions.txt", std::ios::out);
  for(G4int i =0; i<L1rows; i++)
  { 
    for(G4int j=0; j<2; j++)
	  {
	    for(G4int k=0; k<3; k++)
	    {
        G4ThreeVector blockpos(L1barlen*(j-0.5)*mm,L1barwidth*(k -1)*mm,(L1bardepth*(i+0.5)+10)*mm);
	      G4VPhysicalVolume *physLayer1 = new G4PVPlacement(0,blockpos,logicLayer1,"physLayer1",ExpHall_log,false,i*6+j*3+k,true);
        L1IDPosMap[i*6+j*3+k] = blockpos;
        outputFile << i*6+j*3+k << " " << blockpos.x() << " " << blockpos.y() << " " << blockpos.z() << "\n";
        G4cout<<"///////////////^^^^^^^^^^^^^^^^^^^^^^^^^^^^///////////" << G4endl;
        G4cout<<"ID: "<< i*6+j*3+k << "Posiiton: " << blockpos << G4endl;
        G4cout<<"///////////////^^^^^^^^^^^^^^^^^^^^^^^^^^^^///////////" << G4endl;
	    }
	  }
  }
  outputFile.close();
  auto *layer1VisAtt = new G4VisAttributes(G4Color(0.0, 0.5, 1.0, dettp));

  layer1VisAtt->SetVisibility(true);
  layer1VisAtt->SetForceSolid(true);
  layer1VisAtt->SetForceAuxEdgeVisible(true);//SetForceWireframe(true);
  logicLayer1->SetVisAttributes(layer1VisAtt);
  ////////////////////Define Layer2 surface///////////
  /*double fluxSheetThickness = 27;
  //double fluxSheetThickness = 100;//test optimum distance for maximum coincidences
  double zoffset = 0;//50 test optimum distance for maximum coincidences
  double L2radius = 12.87;//13.5
  G4double fluxSheetZPos = detectorZPos + 10* barSizeCm / 2 + fluxSheetThickness / 2; // Move flux sheet behind detector
  G4Material *fluxMat = materials->FindMaterial("eljen230");

  auto *fluxSheet = new G4Tubs("fluxSheet", L2radius * cm, (L2radius + 5*barSizeCm) * cm, (fluxSheetThickness / 2) * cm, 0., 360.);
  logicFluxSheet = new G4LogicalVolume(fluxSheet, fluxMat, "logicFluxSheet");
  auto *physFluxSheet = new G4PVPlacement(0, G4ThreeVector(0., 0., (fluxSheetZPos -zoffset) * cm), logicFluxSheet, "physFluxSheet", ExpHall_log, false, true);
  auto *fluxSheetVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.1));

  fluxSheetVisAtt->SetVisibility(true);
  fluxSheetVisAtt->SetForceSolid(true);
  logicFluxSheet->SetVisAttributes(fluxSheetVisAtt);*/
  /////////////////////////////////////////////////////
  G4Material *detMat2 = materials->FindMaterial("eljen230");
  //double L2rad=128.7;//150.;
  double nsegs=8.; //8.;
  double nperseg=60.;
  double L2thick=5.;//layers in radial direction
  double L2width=13.;//layers in tangential direction
  //double barwidth = 7.5;//defined before L1 
  double barlen = 270.;//270.; //z full length mm
  double l2shift = -(barlen*mm/2.+ 10*L1bardepth*mm/2.) + 150*mm/tan(angleB1B2);//2.35
  G4double layer2ZPos = 10.0*mm + 10*L1bardepth + barlen / 2 + l2shift; //mm Move layer2 behind detector
  
  G4Box *layer2bar = new G4Box("layer2bar",(barwidth/2.)*mm,barwidth/2.*mm,barlen/2.*mm);
  logicLayer2 = new G4LogicalVolume(layer2bar,detMat2,"logicLayer2");
  G4VPhysicalVolume *physLayer2;
  std::ofstream outFile("layer2_positions.txt", std::ios::out);
  for(G4int i =0; i<nsegs; i++)
  { 
    G4RotationMatrix* rotationL2 = new G4RotationMatrix();
    rotationL2->rotateZ(2.*M_PI/nsegs*i);
    for(G4int j=0; j<L2thick; j++)
    {
	    for(G4int k=0; k<L2width; k++)
	    {
	      
        if  (k==12 && j==0) {
	        break;
	      }
	    
        G4ThreeVector boxPosition(0, 0, 0);
        //boxPosition.transform(*rotationL2);
        boxPosition += G4ThreeVector((L2width/2*barwidth-k*barwidth)*1.00001*mm,L2rad+j*barwidth*1.00001*mm,0);
        double radius = boxPosition.perp(); // distance from the world's center in the XY plane
        double initialAngle = boxPosition.phi();
        
        double rotationAngle = 2*M_PI/nsegs*i;
        double newAngle = initialAngle + rotationAngle;
        G4ThreeVector newPosition(radius * cos(newAngle), radius * sin(newAngle), layer2ZPos*mm);
        physLayer2 = new G4PVPlacement(rotationL2,newPosition,logicLayer2,"physLayer2",ExpHall_log,false,i*nperseg+j*L2width+k,true);
        L2IDPosMap[i*nperseg+j*L2width+k] = newPosition;
        outFile << i*nperseg+j*L2width+k << " " << newPosition.x() << " " << newPosition.y() << " " << newPosition.z() << "\n";
	    }
    }
  }
  outFile.close();
  auto *layer2VisAtt = new G4VisAttributes(G4Color(0.0, 0.5, 1.0, dettp));

  layer2VisAtt->SetVisibility(true);
  layer2VisAtt->SetForceSolid(true);
  layer2VisAtt->SetForceAuxEdgeVisible(true);//SetForceWireframe(true);
  logicLayer2->SetVisAttributes(layer2VisAtt);
  ////////////////// SiPM construct

  G4Material *siMat = materials->FindMaterial("silicon");
  double SipmThickness = 0.123;//cm
  double sipmrows = 10;
  double sipmwidth = 4.18;//half length in mm
  double sipmlen = 1.23;//half length in mm
  double sipmdepth = 4.31;
  if(sipm)
  {
    /*auto *detectorBar2 = new G4Box("detectorBar2", (SipmThickness/2) * cm, (3 * (0.5 / 2) - 0.07) * cm, 10 * (barSizeCm/2) * cm);
    logicDetector2 = new G4LogicalVolume(detectorBar2, siMat, "logicDetector2");
    auto *physDetector2 = new G4PVPlacement(0, G4ThreeVector((barLengthCm + SipmThickness/2)*cm, 0.0, (detectorZPos)*cm), logicDetector2, "physDetector2", ExpHall_log, false, true);
    auto *detVisAtt2 = new G4VisAttributes(G4Color(0.0, 1.0, 0.0, dettp)); // Different color for second detector
    detVisAtt2->SetForceSolid(true);
    logicDetector2->SetVisAttributes(detVisAtt2);

    auto *detectorBar3 = new G4Box("detectorBar3", (SipmThickness/2) * cm, (3 * (0.5 / 2) - 0.07) * cm, 10 * (barSizeCm/2) * cm);
    logicDetector3 = new G4LogicalVolume(detectorBar3, siMat, "logicDetector3");
    auto *physDetector3 = new G4PVPlacement(0, G4ThreeVector(-(barLengthCm + SipmThickness/2)*cm, 0.0, (detectorZPos)*cm), logicDetector3, "physDetector3", ExpHall_log, false, true);
    auto *detVisAtt3 = new G4VisAttributes(G4Color(0.0, 1.0, 0.0, dettp)); // Different color for second detector
    detVisAtt3->SetForceSolid(true);
    logicDetector3->SetVisAttributes(detVisAtt3);*/
    /////////
    G4Box *sipm = new G4Box("sipm",(sipmlen/2.)*mm,sipmwidth/2.*mm,sipmdepth/2.*mm);
    logicSipm = new G4LogicalVolume(sipm,siMat,"logicSipm");
    for(G4int i =0; i<sipmrows; i++)
    { 
      for(G4int j=0; j<2; j++)
      {
        for(G4int k=0; k<3; k++)
        {
          G4ThreeVector sipmpos((2*L1barlen+sipmlen)*(j-0.5)*mm,L1barwidth*(k -1)*mm,(L1bardepth*(i+0.5)+10)*mm);
          G4VPhysicalVolume *physSipm = new G4PVPlacement(0,sipmpos,logicSipm,"physSipm",ExpHall_log,false,i*6+j*3+k,true);
        }
      }
    }
    auto *sipmVisAtt = new G4VisAttributes(G4Color(0.0, 1.0, 0.0, dettp));

    sipmVisAtt->SetVisibility(true);
    sipmVisAtt->SetForceSolid(true);
    logicSipm->SetVisAttributes(sipmVisAtt);
  }
  //////////////////Frame polyEth
  // Define the front material for the frame
  
  //G4Material *frameMat = materials->FindMaterial("ABS_GF");
  G4Material *frameMat = materials->FindMaterial("Aluminium");
  G4double tp = 0.5;
  G4double outerLengthX = 17.97 * mm;
  G4double outerLengthY = 17.02 * mm;
  G4double outerLengthZ = 1.0 * mm;

  G4double holeRadius = barLengthCm*10. *mm;  // Radius of the circular hole

  G4double frameZpos = (detectorZPos - 5.*barSizeCm - 0.05) *cm;//shift whole frame along z direction---detectorZPos
  G4double frameZlength = 6.8 *cm;
  if(frontback){
    auto *outerBox = new G4Box("outerBox", outerLengthX / 2, outerLengthY / 2, outerLengthZ / 2);
    auto *holeDisk = new G4Tubs("holeDisk", 0.0, holeRadius, outerLengthZ / 2 + 0.01*mm, 0.0, 360.0);
    //Subtract shape
    auto *framecross = new G4SubtractionSolid("framecross", outerBox, holeDisk, 0, G4ThreeVector(0.0, 0.0, 0.0));
    //
    logicFrame = new G4LogicalVolume(framecross, frameMat, "logicFrame");
    auto *physFramef = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, frameZpos),                logicFrame, "physFramef", ExpHall_log, false, true);
    auto *physFrameb = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, frameZpos + frameZlength), logicFrame, "physFrameb", ExpHall_log, false, true);
    auto *frameVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, tp));  // Red color with transparency
    frameVisAtt->SetForceSolid(true);  // Ensure the frame appears solid
    logicFrame->SetVisAttributes(frameVisAtt);
  
    ///////////////////////////
    //Define sides of the frame
    /*
    ///side left
    auto *slframeShape = new G4SubtractionSolid("slframeshape", outerBox, innerBox, 0, G4ThreeVector(0.0, 0.0, 0.0));

    logicslFrame = new G4LogicalVolume(slframeShape, frameMat, "logicslFrame");

    G4RotationMatrix *frameRotation = new G4RotationMatrix();
    frameRotation->rotateY(90.0 * deg); // Rotate the entire frame
    auto *physlFrame = new G4PVPlacement(frameRotation, G4ThreeVector(8.49*mm, 0.0, frameZpos + frameZlength/2), logicslFrame, "physlFrame", ExpHall_log, false, true);

    auto *frameslVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));  // Red with transparency
    frameslVisAtt->SetForceSolid(true);
    logicslFrame->SetVisAttributes(frameslVisAtt);

    ///side right
    auto *srframeShape = new G4SubtractionSolid("srframeshape", outerBox, innerBox, 0, G4ThreeVector(0.0, 0.0, 0.0));

    logicsrFrame = new G4LogicalVolume(srframeShape, frameMat, "logicsrFrame");

    auto *physrFrame = new G4PVPlacement(frameRotation, G4ThreeVector(-8.49*mm, 0.0, frameZpos + frameZlength/2), logicsrFrame, "physrFrame", ExpHall_log, false, true);

    auto *framesrVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));  // Red with transparency
    framesrVisAtt->SetForceSolid(true);
    logicsrFrame->SetVisAttributes(framesrVisAtt);
    ///////////////////////////
    */
    //Define top and bottom slabs of the frame
    // Define the material
    G4double outersideX = 70.0 * mm;
    G4double outersideY = 17.02 * mm;
    G4double sidethick = 1.0* mm; // Frame thickness

    G4double innersideX = outersideX - 2. * sidethick;
    G4double innersideY = outersideY - 2. * sidethick;

    //auto *outerBox = new G4Box("outerBox", outersideX / 2, outersideY / 2, sidethick / 2);
    //auto *innerBox = new G4Box("innerBox", innersideX / 2, innersideY / 2, 0.01*mm + sidethick / 2);
    // Define cuboid dimensions
    G4double boxX = 2.83 * mm;
    G4double boxY = 1.0 * mm;
    G4double boxZ = 67.0 *mm;//67.10 decreased to remove volume overlaps frameZlength - outerLengthZ;
    // Create a single cuboid shape
    auto *slabSolid = new G4Box("slabSolid", boxX / 2, boxY / 2, boxZ / 2);

    // Create a single logical volume
    logicSlab = new G4LogicalVolume(slabSolid, frameMat, "logicSlab");

    // Define positions for 6 placements
    std::vector<G4ThreeVector> positions = {
      G4ThreeVector(outerLengthX/2 - boxX/2, outerLengthY/2 - boxY/2, detectorZPos*cm),
      G4ThreeVector(-outerLengthX/2 + boxX/2, outerLengthY/2 - boxY/2, detectorZPos*cm),
      G4ThreeVector(0.0, outerLengthY/2 - boxY/2, detectorZPos*cm),
      G4ThreeVector(0.0, -outerLengthY/2 + boxY/2, detectorZPos*cm),
      G4ThreeVector(outerLengthX/2 - boxX/2, -outerLengthY/2 + boxY/2, detectorZPos*cm),
      G4ThreeVector(-outerLengthX/2 + boxX/2, -outerLengthY/2 + boxY/2, detectorZPos*cm)
    };

    // Place six instances of the cuboid
    for (size_t i = 0; i < positions.size(); i++) {
      new G4PVPlacement(0, positions[i], logicSlab, "physSlab" + std::to_string(i), ExpHall_log, false, i);
    }

    auto *slabVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, tp));
    slabVisAtt->SetForceSolid(true);
    logicSlab->SetVisAttributes(slabVisAtt);
  }
  //////////////////PCB FR4 construct
  G4Material *pcbMat = materials->FindMaterial("FR4");
  double pcbThickness = 0.185;//cm
  double pcblen = 1.85;//mm
  if(pcb)
  {
    auto *pcb = new G4Box("pcb", (pcblen/2.) * mm, (3 * (L1barwidth / 2.)) * mm, (5 * (L1bardepth))* mm);
    logicpcb = new G4LogicalVolume(pcb, pcbMat, "logicpcb");
    auto *physPcb1 = new G4PVPlacement(0, G4ThreeVector( (outerLengthX/2. + pcblen/2.)*mm, 0.0, (detectorZPos)*cm), logicpcb, "physPcb1", ExpHall_log, false, true);
    auto *physPcb2 = new G4PVPlacement(0, G4ThreeVector(-(outerLengthX/2. + pcblen/2.)*mm, 0.0, (detectorZPos)*cm), logicpcb, "physPcb2", ExpHall_log, false, true);
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateX(90*deg);
    auto *physPcb3 = new G4PVPlacement(rot, G4ThreeVector( (outerLengthX/2. + pcblen/2.)*mm, 0.0, (detectorZPos -5*barSizeCm)*cm - (3 * (L1barwidth / 2.))*mm), logicpcb, "physPlt3", ExpHall_log, false, true);
    auto *physPcb4 = new G4PVPlacement(rot, G4ThreeVector(-(outerLengthX/2. + pcblen/2.)*mm, 0.0, (detectorZPos -5*barSizeCm)*cm - (3 * (L1barwidth / 2.))*mm), logicpcb, "physPlt4", ExpHall_log, false, true);
    auto *pcbVisAtt = new G4VisAttributes(G4Color(0.0, 0.0, 1.0, dettp)); // Different color for second detector
    pcbVisAtt->SetForceSolid(true);
    logicpcb->SetVisAttributes(pcbVisAtt);
  }
  //////////////////
  //////////////////Heat exchange Copper plate construct
  G4Material *heatMat = nist->FindOrBuildMaterial("G4_Cu");
  double heatlen = 4.0;//mm
  if(heat)
  {
    auto *plt = new G4Box("plt", (heatlen/2.)*mm, 3.*L1barwidth/2.*mm,10.*L1bardepth/2.*mm);
    logicheat = new G4LogicalVolume(plt, heatMat, "logicheat");
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateX(0*deg);
    auto *physPlt1 = new G4PVPlacement(rot, G4ThreeVector( (outerLengthX/2. + pcblen + heatlen/2.)*mm, 0.0, (detectorZPos)*cm), logicheat, "physPlt1", ExpHall_log, false, true);
    auto *physPlt2 = new G4PVPlacement(rot, G4ThreeVector(-(outerLengthX/2. + pcblen + heatlen/2.)*mm, 0.0, (detectorZPos)*cm), logicheat, "physPlt2", ExpHall_log, false, true);
    auto *heatVisAtt = new G4VisAttributes(G4Color(0.72, 0.45, 0.20, dettp)); // Different color for second detector
    heatVisAtt->SetForceSolid(true);
    logicheat->SetVisAttributes(heatVisAtt);
  }
  //////////////////
  //////////////////
  //Mounting angles
  
  G4Material *mtMat = materials->FindMaterial("Aluminium");

  G4double mtx = 5;//thickness mm
  G4double mty = 240.0;//mm
  G4double mtz = 30.0;//mm 
    if(mount){
    auto *mtLpl = new G4Box("mtLpl", mtx/2 * mm, mty/2 * mm, mtz/2 * mm);
    logicmtL = new G4LogicalVolume(mtLpl, mtMat, "logicmtL");
    auto *physmtL  = new G4PVPlacement(0, G4ThreeVector( (L1barlen + sipmlen + pcblen + mtx/2 + 0.5)*mm, 0.0, (detZpos - 8*L1bardepth)*mm), logicmtL, "physmtL", ExpHall_log, false, true);
    auto *physmtL2 = new G4PVPlacement(0, G4ThreeVector(-(L1barlen + sipmlen + pcblen + mtx/2 + 0.5)*mm, 0.0, (detZpos - 8*L1bardepth)*mm), logicmtL, "physmtL2", ExpHall_log, false, true);
    auto *mtlVisAtt = new G4VisAttributes(G4Color(0.0, 1.0, 1.0, tp)); // Different color for second detector
    mtlVisAtt->SetForceSolid(true);
    logicmtL->SetVisAttributes(mtlVisAtt);


    //G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    G4double slabX = 3.0*cm;  
    G4double slabY = 7.0*cm;   
    G4double slabZ = 0.5*cm;//mtx  

    G4double middleSlabX = 1.5*cm;  
    G4double middleSlabY = 10.0*cm;  
    G4double middleSlabZ = 0.5*cm;  

    ////C-shape
    G4Box* topSlab = new G4Box("topSlab", slabX/2, slabY/2, slabZ/2);
    G4Box* middleSlab = new G4Box("middleSlab", middleSlabX/2, middleSlabY/2, middleSlabZ/2);
    G4Box* bottomSlab = new G4Box("bottomSlab", slabX/2, slabY/2, slabZ/2);

    G4LogicalVolume* logicTopSlab = new G4LogicalVolume(topSlab, mtMat, "logicTopSlab");
    G4LogicalVolume* logicMiddleSlab = new G4LogicalVolume(middleSlab, mtMat, "logicMiddleSlab");
    G4LogicalVolume* logicBottomSlab = new G4LogicalVolume(bottomSlab, mtMat, "logicBottomSlab");

    G4double shift = (slabX - slabZ)/2 + (L1barlen + sipmlen + pcblen + mtx/2 + 0.5)*mm;
    G4double zshift = (detZpos - 8 * L1bardepth - mtz/2 - mtx/2)*mm;
    auto *physTopSlab =new G4PVPlacement(0, G4ThreeVector(shift, (slabY + middleSlabY) / 2, zshift), logicTopSlab, "physTopSlab", ExpHall_log, false, true);
    auto *physMiddleSlab =new G4PVPlacement(0, G4ThreeVector(shift + slabX/4, 0, zshift), logicMiddleSlab, "physMiddleSlab", ExpHall_log, false, true);
    auto *physBottomSlab =new G4PVPlacement(0, G4ThreeVector(shift, -(slabY + middleSlabY) / 2, zshift), logicBottomSlab, "physBottomSlab", ExpHall_log, false, true);

    auto *physTopSlab2 =new G4PVPlacement(0, G4ThreeVector(-shift, (slabY + middleSlabY) / 2, zshift), logicTopSlab, "physTopSlab2", ExpHall_log, false, true);
    auto *physMiddleSlab2 =new G4PVPlacement(0, G4ThreeVector(-(shift + slabX/4), 0, zshift), logicMiddleSlab, "physMiddleSlab2", ExpHall_log, false, true);
    auto *physBottomSlab2 =new G4PVPlacement(0, G4ThreeVector(-shift, -(slabY + middleSlabY) / 2, zshift), logicBottomSlab, "physBottomSlab2", ExpHall_log, false, true);

    G4VisAttributes* visAtt = new G4VisAttributes(G4Color(0.0, 1.0, 1.0, tp)); 
    visAtt->SetForceSolid(true);

    logicTopSlab->SetVisAttributes(visAtt);
    logicMiddleSlab->SetVisAttributes(visAtt);
    logicBottomSlab->SetVisAttributes(visAtt);
  }
  //////////////////
  //////Lead shield collimator
  if (shield){
    G4double shieldThickness = 500;//cm
    G4double shieldR = 0.75;
    G4double shieldRmax = 5;//5cm
    G4double shieldR2max = 10;//5cm
    
    G4double mtx2 = 0.5;//thickness cm
    G4double mtz2 = 3;//cm
    //G4double shieldZPos = (detZpos - 10*L1bardepth)*mm - 1.0*(mtz2/2 + mtx2 + shieldThickness/2)*cm;
    G4double shieldZPos = (detZpos - 8 * L1bardepth - mtz/2 - mtx)*mm - shieldThickness/2*cm;
    G4Material *shieldMat = materials->FindMaterial("BoratedTungsten");//"LithiumPolyethyl"
    G4Material *shieldMat2 = materials->FindMaterial("LithiumPolyethyl");

    auto *shielding = new G4Tubs("shielding", shieldR * cm, (shieldR + shieldRmax) * cm, (shieldThickness / 2) * cm, 0., 360.);
    auto *shielding2 = new G4Tubs("shielding2", (shieldR + shieldRmax) * cm, (shieldR + shieldRmax + shieldR2max) * cm, (shieldThickness / 2) * cm, 0., 360.);

    logicShield = new G4LogicalVolume(shielding, shieldMat, "logicShield");
    logicShield2 = new G4LogicalVolume(shielding2, shieldMat2, "logicShield2");

    auto *physShield = new G4PVPlacement(0, G4ThreeVector(0., 0., shieldZPos), logicShield, "physShield", ExpHall_log, false, true);
    auto *physShield2 = new G4PVPlacement(0, G4ThreeVector(0., 0., shieldZPos), logicShield2, "physShield2", ExpHall_log, false, true);

    auto *shieldVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 1.0, 0.3));
    auto *shieldVisAtt2 = new G4VisAttributes(G4Color(1.0, 1.0, 1.0, 0.7));

    shieldVisAtt->SetVisibility(true);
    shieldVisAtt->SetForceSolid(true);
    shieldVisAtt2->SetVisibility(true);
    shieldVisAtt2->SetForceSolid(true);

    logicShield->SetVisAttributes(shieldVisAtt);
    logicShield2->SetVisAttributes(shieldVisAtt2);
  }
  //////////////
  /////Detector Shield
  /*if(detshield){
    G4double detShieldThickness = 40;//cm
    G4double detShieldRmin = 1.0605;//10;//
    G4double detShieldRmax = 75;//5cm
    G4double detshldshift = 15;
    G4double detShieldZPos = - detShieldThickness/2 * cm - detshldshift * cm;//mm Move detector shield behind layer2

    G4Material *detShieldMat = materials->FindMaterial("BoratedPolyethyl");

    auto *detShield = new G4Tubs("detShield", detShieldRmin * cm, detShieldRmax * cm, (detShieldThickness / 2) * cm, 0., 360.);

    logicDetShield = new G4LogicalVolume(detShield, detShieldMat, "logicDetShield");
    auto *detShieldVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.3));

    detShieldVisAtt->SetVisibility(true);
    detShieldVisAtt->SetForceSolid(true);

    logicDetShield->SetVisAttributes(detShieldVisAtt);

    auto *physDetShield = new G4PVPlacement(0, G4ThreeVector(0., 0., detShieldZPos), logicDetShield, "physDetShield", ExpHall_log, false, 0, true);
    auto *physDetShield2 = new G4PVPlacement(0, G4ThreeVector(0., 0.,detShieldZPos + 110*cm), logicDetShield, "physDetShield2", ExpHall_log, false, 1, true);



    //centre piece
    G4double detCShieldThickness = 70;//cm
    G4double detCShieldRmin = 35;//10;//
    G4double detCShieldRmax = 75;//5cm
    G4double detCShieldZPos = detShieldZPos + 55*cm;
    auto *detCShield = new G4Tubs("detCShield", detCShieldRmin * cm, detCShieldRmax * cm, (detCShieldThickness / 2) * cm, 0., 360.);
    logicDet2Shield = new G4LogicalVolume(detCShield, detShieldMat, "logicDet2Shield");
    auto *det2ShieldVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.3));

    det2ShieldVisAtt->SetVisibility(true);
    det2ShieldVisAtt->SetForceSolid(false);

    logicDet2Shield->SetVisAttributes(det2ShieldVisAtt);
    auto *physCDetShield = new G4PVPlacement(0, G4ThreeVector(0., 0., detCShieldZPos), logicDet2Shield, "physCDetShield", ExpHall_log, false, true); 
  }*/

  if(detshield){
    G4double detShieldZPos = -100;
    G4double pDz = 60;
    G4double pRmin1 = 1.0605; 
    G4double extraR = 0;
    G4double pRmax1 = 75*(400+detShieldZPos)/400 + extraR; 
    G4double pRmax2 = 75*(400 + detShieldZPos + pDz)/400 + extraR; 
    G4double pSPhi = 0, pDPhi = 2*M_PI;
    G4Material *detShieldMat = materials->FindMaterial("BoratedPolyethyl");
    auto *detShield = new G4Cons("detShield", pRmin1 * cm, pRmax1 * cm, pRmin1 * cm, pRmax2 * cm, pDz/2 * cm, pSPhi, pDPhi);

    logicDetShield = new G4LogicalVolume(detShield, detShieldMat, "logicDetShield");
    auto *detShieldVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.3));
    detShieldVisAtt->SetVisibility(true);
    detShieldVisAtt->SetForceSolid(true);
    logicDetShield->SetVisAttributes(detShieldVisAtt);

    auto *physDetShield = new G4PVPlacement(0, G4ThreeVector(0., 0., detShieldZPos*cm), logicDetShield, "physDetShield", ExpHall_log, false, 0, true);

    G4double detShieldThickness2 = 10;//cm
    G4double detShieldRmin2 = 1.0605;//10;//
    G4double detShieldRmax2 = pRmax2;//5cm
    G4double detShieldZPos2 = detShieldZPos *cm + pDz/2 *cm + detShieldThickness2/2 * cm;//mm Move detector shield behind layer2
    G4Material *detShieldMat2 = materials->FindMaterial("Lead");

    auto *gamShield = new G4Tubs("gamShield", detShieldRmin2 * cm, detShieldRmax2 * cm, (detShieldThickness2 / 2) * cm, 0., 360.);
    logicGamShield = new G4LogicalVolume(gamShield, detShieldMat2, "logicGamShield");
    auto *detShieldVisAtt2 = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.3));
    detShieldVisAtt2->SetVisibility(true);
    detShieldVisAtt2->SetForceSolid(true);
    logicGamShield->SetVisAttributes(detShieldVisAtt2);

    auto *physGamShield = new G4PVPlacement(0, G4ThreeVector(0., 0., detShieldZPos2), logicGamShield, "physGamShield", ExpHall_log, false, 0, true);
    //auto *physGamShield2 = new G4PVPlacement(0, G4ThreeVector(0., 0., detShieldZPos2 - pDz*cm - detShieldThickness2*cm), logicGamShield, "physGamShield2", ExpHall_log, false, 1, true);
  }

  /*if(gamshield){
    G4double detShieldThickness2 = 10;//cm
    G4double detShieldRmin2 = 1.0605;//10;//
    G4double detShieldRmax2 = 35;//5cm
    G4double detshldshift2 = 5;
    G4double detShieldZPos2 = - detShieldThickness2/2 * cm - detshldshift2 * cm;//mm Move detector shield behind layer2
    G4Material *detShieldMat2 = materials->FindMaterial("Lead");

    auto *gamShield = new G4Tubs("gamShield", detShieldRmin2 * cm, detShieldRmax2 * cm, (detShieldThickness2 / 2) * cm, 0., 360.);
    logicGamShield = new G4LogicalVolume(gamShield, detShieldMat2, "logicGamShield");
    auto *detShieldVisAtt2 = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.3));
    detShieldVisAtt2->SetVisibility(true);
    detShieldVisAtt2->SetForceSolid(true);
    logicGamShield->SetVisAttributes(detShieldVisAtt2);

    auto *physGamShield = new G4PVPlacement(0, G4ThreeVector(0., 0., detShieldZPos2), logicGamShield, "physGamShield", ExpHall_log, false, 0, true);
    auto *physGamShield2 = new G4PVPlacement(0, G4ThreeVector(0., 0., detShieldZPos2 + 60*cm), logicGamShield, "physGamShield2", ExpHall_log, false, 1, true);

    //centre piece
    G4double gamCShieldThickness = 50;//cm
    G4double gamCShieldRmin = 25;//10;//18
    G4double gamCShieldRmax = 35;//5cm
    G4double gamCShieldZPos = detShieldZPos2 + 30*cm;
    auto *gamCShield = new G4Tubs("gamCShield", gamCShieldRmin * cm, gamCShieldRmax * cm, (gamCShieldThickness / 2) * cm, 0., 360.);
    logicGam2Shield = new G4LogicalVolume(gamCShield, detShieldMat2, "logicGam2Shield");
    auto *gam2ShieldVisAtt = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.7));

    gam2ShieldVisAtt->SetVisibility(true);
    gam2ShieldVisAtt->SetForceSolid(false);

    logicGam2Shield->SetVisAttributes(gam2ShieldVisAtt);
    auto *physCGamShield = new G4PVPlacement(0, G4ThreeVector(0., 0., gamCShieldZPos), logicGam2Shield, "physCGamShield", ExpHall_log, false, true);

  }*/
  return ExpHall_phys;
}

void DetectorConstruction::ConstructSDandField()
{
  TrackerNeutronSD *detectorBar = new TrackerNeutronSD("detectorBar");
  logicLayer1->SetSensitiveDetector(detectorBar);
  G4SDManager::GetSDMpointer()->AddNewDetector(detectorBar);

  TrackerFluxSheet *fluxSheet = new TrackerFluxSheet("fluxSheet");
  logicLayer2->SetSensitiveDetector(fluxSheet);
  G4SDManager::GetSDMpointer()->AddNewDetector(fluxSheet);
}
