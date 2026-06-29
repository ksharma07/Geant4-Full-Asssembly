#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include <TH2D.h>
#include <TCutG.h>
#include <TF1.h>
#include <iostream>
#include <cmath>
#include <TMath.h>
#include "TVector3.h"

double GetCentroid(const std::vector<double>& values)
{
    if (values.empty()) return -999.0; // or any error value

    double sum = 0.0;
    // Loop through each value and add to sum
    for (size_t i = 0; i < values.size(); i++) {
        sum = sum + values[i];
    }

    // Calculate average (centroid)
    double average = sum / values.size();
    return average;
}

double GetEsum(const std::vector<double>& energies)
{
    if (energies.empty()) return -999.0; // or any error value

    double sum = 0.0;
    // Loop through each value and add to sum
    for (size_t i = 0; i < energies.size(); i++) {
        sum = sum + energies[i];
    }
    return sum;
}

double SigmaTime(double timestamp, double sigma_ns)
{
    // Seed the RNG only once
    static bool seeded = false;
    if (seeded==false) {
        srand(static_cast<unsigned int>(time(NULL)));
        seeded = true;
    }

    // Generate Gaussian noise using Box-Muller transform
    double u1 = (rand() + 1.0) / (RAND_MAX + 2.0); // avoid log(0) and log(1)
    double u2 = (rand() + 1.0) / (RAND_MAX + 2.0);
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);//=(x-u)/sigma
    //a second z 
    return timestamp + z * sigma_ns;//x
}

double SigmaPosition(double position, double sigma_x) 
{
    // Seed the RNG only once
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(NULL)));
        seeded = true;
    }

    // Generate Gaussian noise using Box-Muller transform
    double u1 = (rand() + 1.0) / (RAND_MAX + 2.0);
    double u2 = (rand() + 1.0) / (RAND_MAX + 2.0);
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    // Apply Gaussian smearing
    return position + z * sigma_x;
}
///////////////////////////MAIN//////////////////////////////////////////////
void evtreehisto() {
    // Open the ROOT file
    TFile *inputFile = TFile::Open("../eventoutput_32deg.root");

    // Access the tree
    TTree *tree = (TTree*) inputFile->Get("Events");
    if (!tree) {
        std::cerr << "Error: Tree not found!" << std::endl;
        return;
    }

    // Variables to hold the values of branches
    double primaryTheta, primaryPhi, primaryTime;
    int    eventID, scintID, barID, det1Interactions, det2Interactions;
    int    heatInteractions, shldInteractions, detshldInteractions , gamshldInteractions, wallInteractions, allprimarykills, cuprimarykills, mtprimarykills, shldprimarykills, gamshldprimarykills, wallprimarykills, otherVolumes, n2Interactions;
    double det1Energy, det2Energy, det1Time, det2Time, det1ScatAngle, shldEnergy, detshldEnergy, gamshldEnergy, shldTime, gamTime;
    double scintL1X, scintL1Y, scintL1Z, scintL2X, scintL2Y, scintL2Z, det1PositionX; 
    double det1PositionY, det1PositionZ, det2PositionX, det2PositionY, det2PositionZ, shldPositionZ, gamPositionZ;
    
    std::vector<double>* det1Energies = nullptr;
    std::vector<double>* det1Times    = nullptr;
    std::vector<double>* det1IDPosX   = nullptr;
    std::vector<double>* det1IDPosY   = nullptr;
    std::vector<double>* det1IDPosZ   = nullptr;
    std::vector<double>* det1AllPosX  = nullptr;
    std::vector<double>* det1AllPosY  = nullptr;
    std::vector<double>* det1AllPosZ  = nullptr;

    std::vector<double>* det2Energies = nullptr;
    std::vector<double>* det2Times    = nullptr;
    std::vector<double>* det2IDPosX   = nullptr;
    std::vector<double>* det2IDPosY   = nullptr;
    std::vector<double>* det2IDPosZ   = nullptr;
    std::vector<double>* det2AllPosX  = nullptr;
    std::vector<double>* det2AllPosY  = nullptr;
    std::vector<double>* det2AllPosZ  = nullptr;

    std::vector<double>* cupltEnergies = nullptr;
    std::vector<double>* secL1Energies = nullptr;
    std::vector<double>* secL2Energies = nullptr;

    // Set the branch addresses to read data
    tree->SetBranchAddress("eventID", &eventID);
    
    tree->SetBranchAddress("primaryTheta",&primaryTheta);
    tree->SetBranchAddress("primaryPhi",&primaryPhi);
    tree->SetBranchAddress("primaryTime",&primaryTime);
    
    tree->SetBranchAddress("otherVolumes", &otherVolumes);//primary interactions
    tree->SetBranchAddress("det1Interactions", &det1Interactions);
    tree->SetBranchAddress("det2Interactions", &det2Interactions);
    tree->SetBranchAddress("heatInteractions", &heatInteractions);
    //tree->SetBranchAddress("shldInteractions", &shldInteractions);
    tree->SetBranchAddress("detshldInteractions", &detshldInteractions);
    tree->SetBranchAddress("gamshldInteractions", &gamshldInteractions);
    tree->SetBranchAddress("wallInteractions", &wallInteractions);

    tree->SetBranchAddress("det1Energy", &det1Energy);
    tree->SetBranchAddress("det2Energy", &det2Energy);
    //tree->SetBranchAddress("shldEnergy", &shldEnergy);
    
    tree->SetBranchAddress("detshldEnergy", &detshldEnergy);
    tree->SetBranchAddress("gamshldEnergy", &gamshldEnergy);
    tree->SetBranchAddress("shldPositionZ", &shldPositionZ);
    tree->SetBranchAddress("gamPositionZ",  &gamPositionZ);
    tree->SetBranchAddress("shldTime", &shldTime);
    tree->SetBranchAddress("gamTime",  &gamTime);

    tree->SetBranchAddress("det1ScatAngle", &det1ScatAngle);
    tree->SetBranchAddress("scintID",  &scintID);
    tree->SetBranchAddress("det1Time", &det1Time);
    tree->SetBranchAddress("scintL1X", &scintL1X);
    tree->SetBranchAddress("scintL1Y", &scintL1Y);
    tree->SetBranchAddress("scintL1Z", &scintL1Z);
    tree->SetBranchAddress("det1PositionX", &det1PositionX);
    tree->SetBranchAddress("det1PositionY", &det1PositionY);
    tree->SetBranchAddress("det1PositionZ", &det1PositionZ);
    
    tree->SetBranchAddress("barID",    &barID);
    tree->SetBranchAddress("det2Time", &det2Time);
    tree->SetBranchAddress("scintL2X", &scintL2X);
    tree->SetBranchAddress("scintL2Y", &scintL2Y);
    tree->SetBranchAddress("scintL2Z", &scintL2Z);
    tree->SetBranchAddress("det2PositionX", &det2PositionX);
    tree->SetBranchAddress("det2PositionY", &det2PositionY);
    tree->SetBranchAddress("det2PositionZ", &det2PositionZ);
    //individual step info
    tree->SetBranchAddress("det1IDPosX", &det1IDPosX);
    tree->SetBranchAddress("det1IDPosY", &det1IDPosY);
    tree->SetBranchAddress("det1IDPosZ", &det1IDPosZ);
    tree->SetBranchAddress("det1AllPosX", &det1AllPosX);
    tree->SetBranchAddress("det1AllPosY", &det1AllPosY);
    tree->SetBranchAddress("det1AllPosZ", &det1AllPosZ);
    tree->SetBranchAddress("det1Energies", &det1Energies);
    tree->SetBranchAddress("det1Times", &det1Times);

    tree->SetBranchAddress("det2IDPosX", &det2IDPosX);
    tree->SetBranchAddress("det2IDPosY", &det2IDPosY);
    tree->SetBranchAddress("det2IDPosZ", &det2IDPosZ);
    tree->SetBranchAddress("det2AllPosX", &det2AllPosX);
    tree->SetBranchAddress("det2AllPosY", &det2AllPosY);
    tree->SetBranchAddress("det2AllPosZ", &det2AllPosZ);
    tree->SetBranchAddress("det2Energies", &det2Energies);
    tree->SetBranchAddress("det2Times", &det2Times);

    tree->SetBranchAddress("cupltEnergies", &cupltEnergies);
    tree->SetBranchAddress("cuprimarykills", &cuprimarykills);
    tree->SetBranchAddress("mtprimarykills", &mtprimarykills);
    tree->SetBranchAddress("shldprimarykills", &shldprimarykills);
    tree->SetBranchAddress("gamshldprimarykills", &gamshldprimarykills);
    tree->SetBranchAddress("wallprimarykills", &wallprimarykills);
    //primary kills and secondary interactions
    tree->SetBranchAddress("allprimarykills", &allprimarykills);
    tree->SetBranchAddress("n2Interactions", &n2Interactions);
    tree->SetBranchAddress("secL1Energies", &secL1Energies);
    tree->SetBranchAddress("secL2Energies", &secL2Energies);

    // Load graphical cut
    TFile *cutFile = TFile::Open("test.cuts");
    TCutG *ecut = (TCutG*)cutFile->Get("ecut");
    cutFile->Close();

    TFile *outputFile       = new TFile("hist_1M_0.25l1_l2_32deg.root", "RECREATE");
    
    TDirectory *dir0 = outputFile->mkdir("Constraints");
    dir0->cd();
    TH1D *sigmaen_01 = new TH1D("sigmaen_01", "Energy Resolution vs N (delta_T/T = 0.1)", 9800, 201, 10001);
    TH1D *sigmaen_02 = new TH1D("sigmaen_02", "Energy Resolution vs N (delta_T/T = 0.2)", 9951, 51, 10002);
    
    for(int ibin = 1; ibin <= sigmaen_01->GetNbinsX(); ibin++) {
        double x1 = sigmaen_01->GetBinCenter(ibin);
        double val1 = 0.11079 * sqrt((0.1/2) * sqrt(2*x1 - 2) - 1);
        sigmaen_01->SetBinContent(ibin, val1);
    }
    for(int ibin = 1; ibin <= sigmaen_02->GetNbinsX(); ibin++) {
        double x2 = sigmaen_02->GetBinCenter(ibin);
        double val2 = 0.11079 * sqrt((0.2/2) * sqrt(2*x2 - 2) - 1);
        sigmaen_02->SetBinContent(ibin, val2);
    }
    
    TH2F *Error_r   = new TH2F("Error_r",   "Error in r vs scattering angle",   180, 0, 180, 2000, 0, 2000);
    TH2F *Error_z   = new TH2F("Error_z",   "Error in z vs scattering angle",   180, 0, 180, 2000, 0, 2000);
    TH2F *Error_tof = new TH2F("Error_tof", "Error in TOF vs scattering angle", 180, 0, 180, 2000, 0, 2000);
    TH2F *Error_En  = new TH2F("Error_En",  "Error in En vs scattering angle", 180, 0, 180, 2000, 0, 2000);
    outputFile->cd();

    TDirectory *dir1 = outputFile->mkdir("n-Generator");
    dir1->cd();
    TH1F *n_theta           = new TH1F("n_theta", "Neutron incident theta", 1000, 0.0, 10.0);
    TH1F *n_phi             = new TH1F("n_phi", "Neutron incident phi",     360, 0.0, 360.0);
    outputFile->cd();

    TDirectory *dir2 = outputFile->mkdir("Layer1-coincidences");
    dir2->cd();
    TH2F *l1_l2             = new TH2F("l1_l2",   "Layer1 Energy vs Layer2 Energy no other vol.",                      60, 0.0, 3.0, 60, 0.0, 3.0);
    TH2F *l1sh_l2           = new TH2F("l1sh_l2", "Layer1 Energy vs Layer2 Energy for only 1 hit in L1 no other vol.", 60, 0.0, 3.0, 60, 0.0, 3.0);
    TH2F *l1_l2_othervolume = new TH2F("l1_l2_othervolume",   "Layer1 Energy vs Layer2 Energy through other vol.",     60, 0.0, 3.0, 60, 0.0, 3.0);
    TH2F *l1_copl2          = new TH2F("l1_copl2",   "Layer1 Energy vs Layer2 energy with primary neutron with Cu interaction",                        60, 0.0, 3.0, 60, 0.0, 3.0);
    TH2F *l1_secl2          = new TH2F("l1_secl2",   "Primary Layer1 Energy vs Secondary Layer2 Energy",                   60, 0.0, 3.0, 60, 0.0, 3.0);
    outputFile->cd();

    TH1F *tote_l1l2         = new TH1F("tote_l1l2", "Total Layer1+2 energy loss no other vol",      60, 0.0, 3.0);
    TH1F *tote_l1l2othervol = new TH1F("tote_l1l2othervol", "Total Layer1+2 energy loss other vol", 60, 0.0, 3.0);
    //TH1F *tote_shld         = new TH1F("tote_shld", "Total energy loss in shield",                  60, 0.0, 3.0);

    TDirectory *dir3 = outputFile->mkdir("Interaction Counters");
    dir3->cd();
    TH1F *layer1_interactions = new TH1F("layer1_interactions", "Layer 1 Interactions",     100, -0.5, 99.5);
    TH1F *layer2_interactions = new TH1F("layer2_interactions", "Layer 2 Interactions",     100, -0.5, 99.5);
    TH1F *other_interactions  = new TH1F("other_interactions" , "Other Volume Interactions",100, -0.5, 99.5);
    TH1F *cuplt_interactions  = new TH1F("cuplt_interactions" , "Copper plate primary Interactions",        100, -0.5, 99.5);
    TH1F *wall_interactions   = new TH1F("wall_interactions" , "Concrete Wall primary Interactions",        100, -0.5, 99.5);
    TH1F *primary_kills       = new TH1F("primary_kills" , "Number of primary neutron kills",               100, -0.5, 99.5);
    TH1F *cuplate_kills       = new TH1F("cuplate_kills" , "Number of primary neutron killed in copper cooling plate",100, -0.5, 99.5);
    TH1F *mount_kills         = new TH1F("mount_kills" , "Number of primary neutron killed in aluminium mount",100, -0.5, 99.5);
    TH1F *shld_kills          = new TH1F("shld_kills" , "Number of primary neutron killed in B-HDPE shield",100, -0.5, 99.5);
    TH1F *gamshld_kills       = new TH1F("gamshld_kills" , "Number of primary neutron killed in gamma shield",100, -0.5, 99.5);
    TH1F *wall_kills          = new TH1F("wall_kills" , "Number of primary neutron killed in walls",100, -0.5, 99.5);
    TH1F *secN2_interactions  = new TH1F("secN2_interactions" , "Copper plate Secondary neutron generated", 100, -0.5, 99.5);
    outputFile->cd();

    TDirectory *dir4 = outputFile->mkdir("Energy parameters");
    dir4->cd();
    TH1F *layer1_zpos      = new TH1F("layer1_zpos", "Layer 1 z-position",             1000, -500.0, 500.0);
    TH1F *scintl1_zpos     = new TH1F("scintl1_zpos", "Layer 1 centralz-position",     1000, -500.0, 500.0);
    TH1F *barl2_zpos       = new TH1F("barl2_zpos", "Layer 2 centralz-position",       1000, -500.0, 500.0);
    TH1F *layer2_zpos      = new TH1F("layer2_zpos", "Layer 2 z-position",             1000, -500.0, 500.0);
    TH1F *layer2_zpos_coinc= new TH1F("layer2_zpos_coinc", "Layer 2 z-position",       1000, -500.0, 500.0);  
    TH1F *l1l2_zdiff       = new TH1F("l1l2_zdiff", "Difference in zpos bw L1 L2",     1000, -500.0, 500.0);
    TH1F *l1l2_dist        = new TH1F("l1l2_dist", "Distance between L1 L2 first hit", 2000, -1000.0, 1000.0);
    TH1F *scintl1barl2_dist= new TH1F("scintl1barl2_dist", "Distance between L1 block central position and L2 bar central position first hit", 2000, -1000.0, 1000.0);

    TH1F *l1_gt       = new TH1F("l1_gt", "Global Time in L1",                1010, -10.0, 1000.0);//1000
    TH1F *l2_gt       = new TH1F("l2_gt", "Global Time in L2",                1010, -10.0, 1000.0);
    TH1F *l1l2_tdiff  = new TH1F("l1l2_tdiff", "Difference in Time bw L1 L2", 1010, -10.0, 1000.0);
    TH1F *sigmatof    = new TH1F("sigmatof", "Resolution in time",            2000, -10.0, 10.0);
    TH1F *l1_zshift   = new TH1F("l1_zshift", "Difference in central z wrt actual zpos",            2000, -10.0, 10.0);
    TH2F *E2dE_recon  = new TH2F("E2dE_recon", "E2(x) vs dE(y) for energy reconstruction", 100, 0.0, 5.0, 100, 0.0, 5.0);
    TH1F *det2Times_diff = new TH1F("det2Times_diff", "L2 hit times relative to first L2 hit", 1010, -10.0, 1000.0);
    TH1F *vscat_hist = new TH1F("vscat_hist", "Scattered neutron velocity (v/c)", 1000, 0.0, 1.0);
    outputFile->cd();

    TDirectory *dir5 = outputFile->mkdir("Energy reconstructs");
    dir5->cd();
    TH1F *energy_recon1 = new TH1F("energy_recon1", "Reconstruction of neutron energy by direct adding", 1000, 0.0, 10.0);
    TH1F *energy_recon2 = new TH1F("energy_recon2", "Reconstruction of neutron energy by scattering formula", 1000, 0.0, 10.0);
    TH2F *energy_recon2_par = new TH2F("energy_recon2_par", "Reconstruction of neutron energy by scattering formula vs Paramter", 1000, 0.0, 10.0, 1000, 0.0, 10.0);
    TH1F *energy_recon3 = new TH1F("energy_recon3", "Reconstruction of neutron energy added position res.L1", 1000, 0.0, 10.0);
    TH1F *energy_reconb = new TH1F("energy_reconb", "Reconstruction of neutron energy added position res.L1(x,y,z) n L2(x,y)", 1000, 0.0, 10.0);
    TH1F *energy_recon3b= new TH1F("energy_recon3b","Reconstruction of neutron energy added time reso.L1", 1000, 0.0, 10.0);
    TH1F *energy_recon3f= new TH1F("energy_recon3f","Reconstruction of neutron energy added time n pos(-L2z)", 1000, 0.0, 10.0);
    TH1F *energy_reconall= new TH1F("energy_reconall","Reconstruction of neutron energy added time n pos", 1000, 0.0, 10.0);
    TH1F *energy_recon4 = new TH1F("energy_recon4", "Reconstruction of neutron energy", 1000, 0.0, 10.0);
    //TH1F *energy_recon1 = new TH1F("energy_recon1", "Reconstruction of neutron energy",     500, 0.0, 5.0);

    TDirectory *dir6 = outputFile->mkdir("Layer tests");
    dir6->cd();
    TH1F *l1_scatangle      = new TH1F("l1_scatangle", "Scattering Angle in L1 for 1 interaction only", 180, 0.0, 180.0);
    TH2F *l1_energyangle    = new TH2F("l1_energyangle", "Layer1 Energy vs Scattering Angle", 180, 0.0, 180.0, 60, 0.0, 3.0);
    TH1F *l1_scatanglel2    = new TH1F("l1_scatanglel2", "Scattering Angle in L1 for 1 interaction only", 180, 0.0, 180.0);
    TH2F *l1scatl2_energyangle    = new TH2F("l1scatl2_energyangle", "Layer1 Energy vs Scattering Angle if scattered on L2", 180, 0.0, 180.0, 60, 0.0, 3.0);
    TH2F *l1_interzpos      = new TH2F("l1_interzpos","Layer 1 interactions vs position",10, 10.0, 77.0, 100, -0.5, 99.5 );
    
    TDirectory *dir7 = outputFile->mkdir("Shields");
    dir7->cd();
    //TH1F *shield_interactions = new TH1F("shield_interactions", "Shield Interactions",      100, -0.5, 99.5);
    TH2D *detshield_interactions_energy = new TH2D("detshield_interactions_energy","Detector shield interactions vs energy", 100, -0.5, 99.5, 3000, 0.0, 3.0);
    TH2D *gamshield_interactions_energy = new TH2D("gamshield_interactions_energy","Detector lead shield interactions vs energy", 100, -0.5, 99.5, 3000, 0.0, 3.0);
    TH1F *l1shld_zpos                   = new TH1F("l1shld_zpos", "L1 trigger Shield z-position", 3000, -5000.0, -2000.0);
    TH2F *l1shld_energy_zpos            = new TH2F("l1shld_energy_zpos", "Shield energy vs z-position for L2 trigger", 3000, -3000.0, 0.0, 3000, 0.0, 3.0);
    TH1F *l2shld_zpos                   = new TH1F("l2shld_zpos", "L2 trigger Shield z-position", 3000, -5000.0, -2000.0);
    TH2F *l2shld_energy_zpos            = new TH2F("l2shld_energy_zpos", "Shield energy vs z-position for L2 trigger", 3000, -3000.0, 0.0, 3000, 0.0, 3.0);
    TH1F *l1l2shld_zpos                 = new TH1F("l1l2shld_zpos", "L1-L2 trigger Shield z-position", 3000, -3000.0, 0.0);
    TH2F *l1l2shld_energy_zpos          = new TH2F("l1l2shld_energy_zpos", "Shield energy vs z-position for L2 trigger", 3000, -3000.0, 0.0, 3000, 0.0, 3.0);
    TH1F *shld_zpos                     = new TH1F("shld_zpos", "No trigger Shield z-position", 3000, -3000.0, 0.0);
    TH2F *shld_energy_zpos              = new TH2F("shld_energy_zpos", "Shield n energy vs z-position for no L2 trigger", 3000, -3000.0, 0.0, 3000, 0.0, 3.0);
    TH2F *gam_energy_zpos               = new TH2F("gam_energy_zpos", "Lead n energy vs z-position for no L2 trigger", 3000, -3000.0, 0.0, 3000, 0.0, 3.0);
    TH1F *total_shld_energy             = new TH1F("total_shld_energy", "Lead n energy vs z-position for no L2 trigger", 3000, 0.0, 3.0);
    TH2F *l1_l2_shld                    = new TH2F("l1_l2_shld",   "Layer1 Energy vs Layer2 Energy shld involved", 60, 0.0, 3.0, 60, 0.0, 3.0);
    TH1F *energy_recon2_shld            = new TH1F("energy_recon2_shld", "Reconstruction of neutron energy by scattering formula when shld involved", 1000, 0.0, 10.0);
    //Case I
    TH2F *tof_shldL2                    = new TH2F("tof_shldL2", "Shield interaction time wrt L1 vs ToF", 1010, -10.0, 1000.0, 1010, -10.0, 1000.0);
    TH2F *energy_shldL2                 = new TH2F("energy_shldL2", "Shield interaction time wrt L1 vs Neutron energy recons", 3000, 0.0, 3.0, 1010, -10.0, 1000.0);
    //Case II
    TH1F *l2shld_tdiff                  = new TH1F("l2shld_tdiff", "Difference in Time bw L2 and B-PE", 1010, -10.0, 1000.0);
    TH1F *l2gam_tdiff                   = new TH1F("l2gam_tdiff", "Difference in Time bw L2 and Lead", 1010, -10.0, 1000.0);
    TH1F *det2Times_diffshld            = new TH1F("det2Times_diffshld", "L2 hit times relative to first L2 hit followed by shld", 1010, -10.0, 1000.0);
    
    outputFile->cd();
    // Loop over all events
    Long64_t nEntries = tree->GetEntries();
    double tsigma =0.050;//ns from 4x4 sipm 0.050
    double tsigma2 =0.200;//
    double l2sigmaz=5.0;//6.7mm
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        n_theta->Fill(primaryTheta);
        n_phi->Fill(primaryPhi);
        layer1_interactions->Fill(det1Interactions);
        layer2_interactions->Fill(det2Interactions);
        cuplt_interactions->Fill(heatInteractions);
        wall_interactions->Fill(wallInteractions);
        detshield_interactions_energy->Fill(detshldInteractions, detshldEnergy);
        gamshield_interactions_energy->Fill(gamshldInteractions, gamshldEnergy);

        other_interactions->Fill(otherVolumes);
        primary_kills->Fill(allprimarykills);
        cuplate_kills->Fill(cuprimarykills);
        mount_kills->Fill(mtprimarykills);
        shld_kills->Fill(shldprimarykills);
        gamshld_kills->Fill(gamshldprimarykills);
        wall_kills->Fill(wallprimarykills);
        secN2_interactions->Fill(n2Interactions);
        //tote_shld->Fill(shldEnergy);
        // Check if the condition is satisfied (otherVolumes == 0)
        if (det1Energy > 0.2){
            l1_scatangle->Fill(det1ScatAngle);
            if(det1Interactions==1){
                l1_energyangle->Fill(det1ScatAngle,det1Energy);
            }
            l1_interzpos->Fill(det1AllPosZ->at(det1AllPosZ->size()-1),det1Interactions);
            if(GetEsum(*secL2Energies)>0){//looking into cases where secondary neutron produced leaves a false primL1-secL2 coincidence
                l1_secl2->Fill(det1Energy,GetEsum(*secL2Energies));
            }
            if(det2Interactions==0){
                l1shld_zpos->Fill(shldPositionZ);
                l1shld_energy_zpos->Fill(shldPositionZ,detshldEnergy);
            }
            if (det2Energy > 0.2){
                l1l2shld_zpos->Fill(shldPositionZ);
                l1l2shld_energy_zpos->Fill(shldPositionZ,detshldEnergy);
                if(GetEsum(*cupltEnergies)>0){
                    l1_copl2->Fill(det1Energy,det2Energy);
                }
                // Fill the 2D histogram with det1Energy and det2Energy
                double deltaTime = det2Time-det1Time;
                double deltaTimeb= det2Time-SigmaTime(det1Time,tsigma);
                double deltaTimeb2= SigmaTime(deltaTime,tsigma2);
                
                double hits_dist2 = std::pow((det2PositionX - det1PositionX), 2) + std::pow((det2PositionY - det1PositionY), 2) + std::pow((det2PositionZ - det1PositionZ), 2);
                double dZ = det2PositionZ-det1PositionZ;
                double dZsq= dZ * dZ;
                double r2 = std::pow((det2PositionX - det1PositionX), 2) + std::pow((det2PositionY - det1PositionY), 2);

                double angle = std::atan2(std::sqrt(r2), std::abs(dZ)) * 180.0 / TMath::Pi();
                
                double central_dist = std::sqrt(std::pow((det2PositionX - scintL1X), 2) + std::pow((det2PositionY - scintL1Y), 2) + std::pow((det2PositionZ - scintL1Z), 2));
                double central_dist2 = std::pow((det2PositionX - scintL1X), 2) + std::pow((det2PositionY - scintL1Y), 2) + std::pow((det2PositionZ - scintL1Z), 2);
                double central_dist2b = std::pow((scintL2X - scintL1X), 2) + std::pow((scintL2Y - scintL1Y), 2) + std::pow((det2PositionZ - scintL1Z), 2);
                double central_distall = std::pow((scintL2X - scintL1X), 2) + std::pow((scintL2Y - scintL1Y), 2) + std::pow((SigmaPosition(det2PositionZ,l2sigmaz) - scintL1Z), 2);

                double dZcentral = det2PositionZ - scintL1Z;
                double dZcsq = dZcentral*dZcentral;

                double dZall = SigmaPosition(det2PositionZ,l2sigmaz) - scintL1Z;
                double dZsqall = dZall*dZall;

                double drc2 = std::pow((det2PositionX - scintL1X), 2) + std::pow((det2PositionY - scintL1Y), 2);
                double drc2b= std::pow((scintL2X - scintL1X), 2) + std::pow((scintL2Y - scintL1Y), 2);
                
                double vscat = std::sqrt(hits_dist2)/(299.792458*deltaTime);//dist in mm, time in ns, velocity in no.*c
                double v2scat= std::sqrt(central_dist2)/(299.792458*deltaTime);
                double v2scatb= std::sqrt(central_dist2b)/(299.792458*deltaTime);
                //double v2bscat= central_dist/(299.792458*deltaTimeb);
                double v2bscat= std::sqrt(hits_dist2)/(299.792458*deltaTimeb2);
                double vfscat= std::sqrt(central_dist2b)/(299.792458*deltaTimeb2);
                double vscatall= std::sqrt(central_distall)/(299.792458*deltaTimeb2);

                double Escat = 0.5*939.565*std::pow(vscat,2);//mass of neutron in MeV/c2
                double Escat2 = 0.5*939.565*std::pow(v2scat,2);
                double Escatb = 0.5*939.565*std::pow(v2scatb,2);
                double Escat2b = 0.5*939.565*std::pow(v2bscat,2);
                double Escatf = 0.5*939.565*std::pow(vfscat,2);
                double Escatall = 0.5*939.565*std::pow(vscatall,2);
                //double Erecon = E2/std::pow(std::cos(det1ScatAngle*TMath::Pi()/180.0),2);
                double Erecon1 = Escat + det1Energy;//basic add
                double Erecon2 = Escat*(r2+dZsq)/dZsq;//scattering angle formula using R, Z, Tof
                double Erecon3 = Escat2*(drc2+dZcsq)/dZcsq;//same formula just L1 central position
                double Ereconb = Escatb*(drc2b+dZcsq)/dZcsq;//same formula just L1(x,y,z)-L2(x,y) central position
                double Erecon3b = Escat2b*(r2+dZsq)/dZsq;//same as 2, added time resolution
                double Erecon3f = Escatf*(drc2b+dZcsq)/dZcsq;//time and position minus L2z
                double Ereconall = Escatall*(drc2b+dZsqall)/dZsqall;
                //if(det1Interactions ==2 && det2Interactions==2){
                if(otherVolumes==0 && heatInteractions==0){//detshldInteractions==0 && gamshldInteractions==0 && //no interaction other than Layer1 and 2.
                    if(detshldInteractions==0 && gamshldInteractions==0){
                        l1_l2->Fill(det1Energy, det2Energy);
                    }
                    //layer1_zpos->Fill(det1PositionZ);
                    scintl1_zpos->Fill(scintL1Z);
                    barl2_zpos->Fill(scintL2Z);
                    layer2_zpos->Fill(det2PositionZ);
                    l1l2_zdiff->Fill(det2PositionZ-det1PositionZ);
                    l1_gt->Fill(det1Time-primaryTime);
                    l2_gt->Fill(det2Time-primaryTime);
                    //l1l2_tdiff->Fill(deltaTime);
                    tote_l1l2->Fill(det1Energy + det2Energy);
                    if(det1Interactions ==1 && det2Interactions>=1){
                        layer1_zpos->Fill(det1PositionZ);
                        l1scatl2_energyangle->Fill(det1ScatAngle,det1Energy);
                        l1_scatanglel2->Fill(det1ScatAngle);
                        
                        double En = 2450.0;//keV
                        double m = 939565.0;//keV/c2
                        double c = 29.9792458;//cm/ns
                        double R = 15;//cm
                        double SigmaTof = 0.200;//ns
                        double SigmaZ = 5.00;//cm
                        double SigmaR = 0.3;//cm
                        double thetarad = det1ScatAngle * M_PI / 180.0;
                        
                        if (det1ScatAngle <85){
                            double dEdR   = 2*(2* En * std::pow(sin(thetarad), 2))/R;
                            double dEdZ   =   (2* En * tan(thetarad)*cos(2*thetarad))/R;
                            double dEdTof = 2*(En*sin(2*thetarad)*c)*std::sqrt(En/(2*m*R*R));
                            
                            Error_r->Fill(det1ScatAngle, dEdR * SigmaR);
                            Error_z->Fill(det1ScatAngle, std::abs(dEdZ) * SigmaZ);
                            Error_tof->Fill(det1ScatAngle, dEdTof * SigmaTof);

                            double  SigmaEn = std::sqrt(std::pow(dEdR * SigmaR,2)+std::pow(dEdZ * SigmaZ,2)+std::pow(dEdTof * SigmaTof,2));
                            Error_En->Fill(det1ScatAngle, SigmaEn);
                        }
                        
                        l1sh_l2->Fill(det1Energy, det2Energies->at(0));//det2Energy);//
                        l1l2_dist->Fill(std::sqrt(central_dist2));//central_dist2
                        vscat_hist->Fill(vscat);//v/c
                        l1l2_tdiff->Fill(deltaTime);
                        sigmatof->Fill(deltaTime-deltaTimeb2);
                        l1_zshift->Fill(det1PositionZ-scintL1Z);
                        layer2_zpos_coinc->Fill(det2PositionZ);
                        scintl1barl2_dist->Fill(central_dist);
                        energy_recon1->Fill(Erecon1);
                        energy_recon2->Fill(Erecon2);
                        energy_recon2_par->Fill(Erecon2,vscat);
                        //energy_recon2->Fill(Escat/pow(cos(det1ScatAngle * M_PI / 180.0),2));
                        energy_recon3->Fill(Erecon3);
                        energy_reconb->Fill(Ereconb);
                        energy_recon3b->Fill(Erecon3b);
                        energy_recon3f->Fill(Erecon3f);
                        energy_reconall->Fill(Ereconall);
                        E2dE_recon->Fill(2.45-Escat, det1Energy);
                        //loop over det2Times and fill histo wrt first l2 hit: det2Time
                        for (size_t j = 0; j < det2Times->size(); j++) {
                            det2Times_diff->Fill(det2Times->at(j) - det2Time);
                        }
                        if(detshldEnergy>0 || gamshldEnergy>0){
                            l1_l2_shld->Fill(det1Energy, det2Energy);
                            energy_recon2_shld->Fill(Erecon2);
                            if(det2Time > shldTime || det2Time > gamTime){//Case I - L1->Shield->L2 or Shield->L1->L2
                                //if(ecut->IsInside(Erecon2,shldTime-det1Time)){
                                    tof_shldL2->Fill(deltaTime,shldTime-det1Time);
                                //}
                                energy_shldL2->Fill(Erecon2,shldTime-det1Time);
                            }
                            else if(det2Time < shldTime || det2Time < gamTime){//Case II - L1->L2->//(L1)Shield
                                //Iterate thru det2Times and which ones are after shield and how many are greater than 1ns
                                l2shld_tdiff->Fill(shldTime-det2Time);  
                                l2gam_tdiff->Fill(gamTime-det2Time); 
                                for (size_t j = 0; j < det2Times->size(); j++) {
                                    det2Times_diffshld->Fill(det2Times->at(j) - det2Time);
                                }      
                            }
                        }
                    }
                    else if(det1Interactions >1 && det2Interactions >1){ 
                        double det1centposX = GetCentroid(*det1AllPosX);
                        double det1centposY = GetCentroid(*det1AllPosY);
                        double det1centposZ = GetCentroid(*det1AllPosZ);
                        double det2centposX = GetCentroid(*det2AllPosX);
                        double det2centposY = GetCentroid(*det2AllPosY);
                        double det2centposZ = GetCentroid(*det2AllPosZ); 
                        double dZmulti = det2centposZ - det1centposZ; 
                        double avg_dist = std::sqrt(std::pow((det2centposX - det1centposX), 2) + std::pow((det2centposY - det1centposY), 2) + std::pow((det2centposZ - det1centposZ), 2));
                        double v3scat= avg_dist/(299.792458*deltaTime);
                        double Escat3 = 0.5*939.565*std::pow(v3scat,2);
                        double Erecon4 = Escat3/std::pow(dZmulti/avg_dist,2);
                        energy_recon4->Fill(Erecon4);
                    }
                } else {
                    l1_l2_othervolume->Fill(det1Energy, det2Energy);
                    tote_l1l2othervol->Fill(det1Energy + det2Energy);
                }
            }
        }//all Layer1 trigerred handled
        if(det1Interactions==0){
            //shield_interactions->Fill(shldInteractions);
            if(det2Interactions==0){
                shld_zpos->Fill(shldPositionZ);
                shld_energy_zpos->Fill(shldPositionZ,detshldEnergy);
                gam_energy_zpos->Fill(gamPositionZ,gamshldEnergy);
                total_shld_energy->Fill(detshldEnergy+gamshldEnergy);
            } 
            else if (det2Interactions > 0){
                l2shld_zpos->Fill(shldPositionZ);
                l2shld_energy_zpos->Fill(shldPositionZ,detshldEnergy);
            }
        }
    }

    outputFile->cd();  // Navigate to the current directory in the output file
    outputFile->Write();
    outputFile->Close();
    inputFile->Close();
}
