
#include "Materials.hh"

Materials::Materials()
{
  // Elements

  deuter = new G4Isotope("Deuterium", 1, 2, 2 * CLHEP::g / CLHEP::mole);
  B10 = new G4Isotope("B10", 5, 10, 10.0129 * CLHEP::g / CLHEP::mole);
  B11 = new G4Isotope("B11", 5, 11, 11.0093 * CLHEP::g / CLHEP::mole);
  
  elementH = new G4Element("Hydrogen", "H", 1., 1.0079 * CLHEP::g / CLHEP::mole);
  elementN = new G4Element("Nitrogen", "N", 7., 14.01 * CLHEP::g / CLHEP::mole);
  elementO = new G4Element("Oxygen", "O", 8., 15.9994 * CLHEP::g / CLHEP::mole);
  elementS = new G4Element("Sulphur", "S", 16., 32.066 * CLHEP::g / CLHEP::mole);
  elementFe = new G4Element("Iron", "Fe", 26., 55.845 * CLHEP::g / CLHEP::mole);
  elementCa = new G4Element("Calcium", "Ca", 20., 40.078 * CLHEP::g / CLHEP::mole);
  elementSi = new G4Element("Silicon", "Si", 14., 28.0855 * CLHEP::g / CLHEP::mole);
  elementAl = new G4Element("Aluminum", "Al", 13., 26.981539 * CLHEP::g / CLHEP::mole);
  elementPb = new G4Element("Lead", "Pb", 82., 207.2 * CLHEP::g / CLHEP::mole);
  elementC = new G4Element("Carbon", "C", 6., 12.011 * CLHEP::g / CLHEP::mole);
  elementB = new G4Element("Boron", "B", 5., 10.811 * CLHEP::g / CLHEP::mole);
  elementLi = new G4Element("Lithium", "Li", 3., 6.9410 * CLHEP::g / CLHEP::mole);
  elementW = new G4Element("Tungsten", "W", 74., 183.84 * CLHEP::g / CLHEP::mole);


  elementD = new G4Element("Deuter", "D", 1);
  elementD->AddIsotope(deuter, 1);

  elB10 = new G4Element("Boron10", "B10", 1);
  elB10->AddIsotope(B10, 1.0);

  elB11 = new G4Element("Boron11", "B11", 1);
  elB11->AddIsotope(B11, 1.0);
  // Materials

  vacuum = new G4Material("Vacuum", 1, 1.00794 * CLHEP::g / CLHEP::mole,
                          1.0E-25 * CLHEP::g / CLHEP::cm3, kStateGas, 0.1 * CLHEP::kelvin, 1.0E-19 * CLHEP::pascal);

  water = new G4Material("Water", 1 * CLHEP::g / CLHEP::cm3, 2);
  water->AddElement(elementH, 2);
  water->AddElement(elementO, 1);

  lead = new G4Material("Lead", 11.348 * CLHEP::g / CLHEP::cm3, 1);
  lead->AddElement(elementPb, 1);

  boratedPE = new G4Material("BoratedPolyethyl", 0.95 * CLHEP::g / CLHEP::cm3, 5);
  boratedPE->AddElement(elementH,   116);
  boratedPE->AddElement(elementC,   612);
  boratedPE->AddElement(elB11,  40);
  boratedPE->AddElement(elB10,  10);
  boratedPE->AddElement(elementO,   222);

  lithiumPE = new G4Material("LithiumPolyethyl", 0.95 * CLHEP::g / CLHEP::cm3, 4);
  lithiumPE->AddElement(elementH,  113);
  lithiumPE->AddElement(elementC,  596);
  lithiumPE->AddElement(elementLi, 75);
  lithiumPE->AddElement(elementO,  216);

  boratedW = new G4Material("BoratedTungsten", 15.26 * CLHEP::g / CLHEP::cm3, 2);
  boratedW->AddElement(elementW, 90);       // 90% Tungsten by weight
  boratedW->AddElement(elB10, 10); // 10% enriched B

  iron = new G4Material("Iron", 7.874 * CLHEP::g / CLHEP::cm3, 1);
  iron->AddElement(elementFe, 1);

  silicon = new G4Material("silicon", 2.33 * CLHEP::g / CLHEP::cm3, 1);//this 1 is just how many elements in this material. Change if adding more materials.
  silicon->AddElement(elementSi, 1);
  // Can add other elements to make a precise SiPM with AddElement and the "1" divided in resp. element ratios. 
  
  // Materials for FR4
  glassFiber = new G4Material("GlassFiber", 2.55 * CLHEP::g / CLHEP::cm3, 2);
  glassFiber->AddElement(elementSi, 1);
  glassFiber->AddElement(elementO, 2);

  epoxyResin = new G4Material("EpoxyResin", 1.2 * CLHEP::g / CLHEP::cm3, 2);
  epoxyResin->AddElement(elementC, 10);
  epoxyResin->AddElement(elementH, 12);

  // FR4 Material (30% glass fibers, 70% epoxy resin, for example)
  FR4 = new G4Material("FR4", 1.8 * CLHEP::g / CLHEP::cm3, 2);
  FR4->AddMaterial(glassFiber, 0.3); // 30% glass fibers
  FR4->AddMaterial(epoxyResin, 0.7); // 70% epoxy resin
  
  carbon = new G4Material("Carbon", 2.266 * CLHEP::g / CLHEP::cm3, 1);
  carbon->AddElement(elementC, 1);

  boron = new G4Material("Boron", 2.08 * CLHEP::g / CLHEP::cm3, 1);
  boron->AddElement(elementB, 1);

  aluminium = new G4Material("Aluminium", 2.7 * CLHEP::g / CLHEP::cm3, 1);
  aluminium->AddElement(elementAl, 1);

  boricAcid = new G4Material("boricAcid", 1.435 * CLHEP::g / CLHEP::cm3, 3);
  boricAcid->AddElement(elementH, 3);
  boricAcid->AddElement(elementO, 3);
  boricAcid->AddElement(elementB, 1);

  polyEth = new G4Material("polyEth", 0.96 * CLHEP::g / CLHEP::cm3, 2);
  polyEth->AddElement(elementH, 4);
  polyEth->AddElement(elementC, 2);

  /////// --- Define ABS Polymer (Acrylonitrile-Butadiene-Styrene) ---/////////////
  ABS = new G4Material("ABS", 1.04 * CLHEP::g / CLHEP::cm3, 3);
  ABS->AddElement(elementC, 15);
  ABS->AddElement(elementH, 17);
  ABS->AddElement(elementN, 1);

  // --- Define Glass Fiber (SiO2) ---
  SiO2 = new G4Material("SiO2", 2.2 * CLHEP::g / CLHEP::cm3, 2);
  SiO2->AddElement(elementSi, 1);
  SiO2->AddElement(elementO, 2);

  // --- Define ABS-Glass Fiber Composite ---
  ABS_GF = new G4Material("ABS_GF", 1.15 * CLHEP::g / CLHEP::cm3, 2);
  ABS_GF->AddMaterial(ABS, 80 * CLHEP::perCent);  // 80% ABS
  ABS_GF->AddMaterial(SiO2, 20 * CLHEP::perCent); // 20% Glass Fiber
///////////////////////////////////////////////////////////////////////////////////
  eljen230 = new G4Material("eljen230", 1.023 * CLHEP::g / CLHEP::cm3, 2);
  eljen230->AddElement(elementH, 515);
  eljen230->AddElement(elementC, 469);

  eljen232q = new G4Material("eljen232q", 1.023 * CLHEP::g / CLHEP::cm3, 2);
  eljen232q->AddElement(elementH, 512);
  eljen232q->AddElement(elementC, 466);

  heavywater = new G4Material("HeavyWater", 1 * CLHEP::g / CLHEP::cm3, 2);
  heavywater->AddElement(elementD, 2);
  heavywater->AddElement(elementO, 1);

  // air at 20 degree C and 1 atm for the ambiet air.

  density = 1.205e-03 * CLHEP::g / CLHEP::cm3;
  pressure = 1. * CLHEP::atmosphere;
  temperature = 293. * CLHEP::kelvin;
  air = new G4Material("Air", density, 2,
                       kStateGas, temperature, pressure);
  air->AddElement(elementN, 0.7);
  air->AddElement(elementO, 0.3);

  // concrete
  density = 2.3 * CLHEP::g / CLHEP::cm3;
  concrete = new G4Material("Concrete", density = 2.3 * CLHEP::g / CLHEP::cm3, 6);
  concrete->AddElement(elementH, 0.09972);
  concrete->AddElement(elementO, 0.60541);
  concrete->AddElement(elementFe, 0.00285);
  concrete->AddElement(elementCa, 0.04986);
  concrete->AddElement(elementAl, 0.014245);
  concrete->AddElement(elementSi, 0.227915);


}

Materials::~Materials()
{
  ;
}
//-----------------------------------------------------------------------------
G4Material *Materials::FindMaterial(G4String materialName)
{

  // search the material by its name
  G4Material *pttoMaterial = G4Material::GetMaterial(materialName);

  return pttoMaterial;
}
