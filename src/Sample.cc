#include "Sample.hh"

Sample::Sample(G4LogicalVolume *experimentalHall_log, Materials *mat, std::string matSource, double posX, double posY, double posZ, double rOuter, double rInner, double halfLength, G4String material, G4Colour colour)
{
  materials = mat;
  objColour = colour;
  expHall_log = experimentalHall_log;
  BTrmin = rInner * cm;
  BTrmax = rOuter * cm;
  BTDz = halfLength * cm;
  BTSPhi = 0. * deg;
  BTDPhi = 360 * deg;
  Pos0 = new G4ThreeVector(posX, posY, posZ * cm);

  // Get material depending on source (either the user, NIST, or CMS file)
  if (matSource == "nist")
  {
    G4NistManager *nist = G4NistManager::Instance();
    SampleMaterial = nist->FindOrBuildMaterial(material);
  }
  else if (matSource == "mine")
  {
    SampleMaterial = materials->FindMaterial(material);
  }
  else if (matSource == "cms")
  {
    CCalMaterialFactory *matfact = CCalMaterialFactory::getInstance("material.cms");
    SampleMaterial = matfact->findMaterial(material);
  }
}

Sample::~Sample()
{
  ;
}
//-----------------------------------------------------------------------------
G4VPhysicalVolume *Sample::Construct()
{

  Sample_tube = new G4Tubs("Sample", BTrmin, BTrmax, BTDz, BTSPhi, BTDPhi);

  Sample_log = new G4LogicalVolume(Sample_tube, SampleMaterial, "Sample_log", 0, 0, 0);

  Sample_phys = new G4PVPlacement(G4Transform3D(NoRot, *Pos0),
                                  Sample_log, "Sample", expHall_log, false, 0);

  // Visualization Attributes
  G4VisAttributes *Vis = new G4VisAttributes(objColour);
  Vis->SetVisibility(true);
  Vis->SetForceSolid(false);

  Sample_log->SetVisAttributes(Vis);

  return Sample_phys;
}

//-----------------------------------------------------------------------------
void Sample::setRmax(G4double Rmax)
{
  if (Rmax > BTrmin)
  {
    BTrmax = Rmax;
    G4cout << "----> Sample outer radius set to " << G4BestUnit(BTrmax, "Length") << G4endl;
    ;
    Sample_tube->SetOuterRadius(BTrmax);
  }
  else
    G4cout << "----> radius " << G4BestUnit(Rmax, "Length") << " has to be larger than " << G4BestUnit(BTrmin, "Length") << G4endl;
}
//-----------------------------------------------------------------------------
void Sample::setRmin(G4double Rmin)
{
  if (Rmin < BTrmax)
  {
    BTrmin = Rmin;
    G4cout << "----> Sample inner radius set to " << G4BestUnit(BTrmin, "Length") << G4endl;
    ;
    Sample_tube->SetInnerRadius(BTrmin);
  }
  else
    G4cout << "----> radius " << G4BestUnit(Rmin, "Length") << " has to be smaller than " << G4BestUnit(BTrmax, "Length") << G4endl;
}
//-----------------------------------------------------------------------------
void Sample::setLength(G4double length)
{
  BTDz = length / 2.;
  G4cout << "----> Sample length set to " << G4BestUnit(2. * BTDz, "Length") << G4endl;
  ;
  Sample_tube->SetZHalfLength(BTDz);
}
//-----------------------------------------------------------------------------
void Sample::Report()
{
  G4cout << "----> Sample material set to     " << SampleMaterial->GetName() << G4endl;
  G4cout << "----> Sample inner radius set to " << G4BestUnit(BTrmin, "Length") << G4endl;
  G4cout << "----> Sample outer radius set to " << G4BestUnit(BTrmax, "Length") << G4endl;
  G4cout << "----> Sample length set to       " << G4BestUnit(2. * BTDz, "Length") << G4endl;
}
//---------------------------------------------------------------------
void Sample::setMaterial(G4String materialName)
{
  // search the material by its name
  SampleMaterial = materials->FindMaterial(materialName);
  Sample_log->SetMaterial(SampleMaterial);
  G4cout << "----> Sample material set to     " << Sample_log->GetMaterial()->GetName() << G4endl;
}
