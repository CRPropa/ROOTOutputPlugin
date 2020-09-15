#include "CRPropaROOTOutput.h"
#include <crpropa/Units.h>
#include <crpropa/ParticleID.h>
#include <crpropa/Cosmology.h>
#include "TROOT.h" 

#include <string>


struct init_thread_safety
{
  init_thread_safety() 
  {

    ROOT::EnableThreadSafety(); 
  }
};

static init_thread_safety doitonstartup; 


/////////////////////// ROOT EVENT OUTPUT 1D //////////////////////////////////
ROOTEventOutput1D::ROOTEventOutput1D(std::string filename) : crpropa::Module() {
	setDescription("ROOTEventOutput1D, filename: " + filename);
  omp_init_lock(&lock); 
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");
	Tree = new TTree("events", "CRPropa 1D events");
  Tree->SetDirectory(ROOTFile); 
	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("TrajectoryLength_Mpc", &TrajectoryLength_Mpc, "TrajectoryLength_Mpc/F" );
	Tree->Branch("Initial_Type", &Initial_Type, "Initial_Type/I" );
	Tree->Branch("Initial_Energy_EeV", &Initial_Energy_EeV, "Initial_Energy_EeV/F" );
}

void ROOTEventOutput1D::close(){

  omp_set_lock(&lock); 
  if (ROOTFile) {
      ROOTFile->Write();
      ROOTFile->Close();
      delete ROOTFile;
      ROOTFile = 0;
      Tree = 0;
  }

  omp_unset_lock(&lock); 
  omp_destroy_lock(&lock); 
}


ROOTEventOutput1D::~ROOTEventOutput1D() {
	close();
}

void ROOTEventOutput1D::process(crpropa::Candidate *c) const {

  omp_set_lock(&lock); 
		Particle_Type = c->current.getId();
		Energy_EeV = c->current.getEnergy() / crpropa::EeV;
		TrajectoryLength_Mpc = c->getTrajectoryLength() / crpropa::Mpc;
		Initial_Type = c->source.getId();
		Initial_Energy_EeV = c->source.getEnergy() / crpropa::EeV;
		Tree->Fill();
  omp_unset_lock(&lock); 
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////// ROOT PHOTON OUTPUT 1D //////////////////////////////////
ROOTPhotonOutput1D::ROOTPhotonOutput1D(std::string filename)  : crpropa::Module(){
	setDescription("ROOTPHOTONOutput1D, filename: " + filename);
  omp_init_lock(&lock); 
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa photon output data file");
	Tree = new TTree("events", "CRPropa 1D photons");
  Tree->SetDirectory(ROOTFile); 

	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("ComovingDistance_Mpc", &ComovingDistance_Mpc, "ComovingDistance_Mpc/F" );
	Tree->Branch("Initial_Type", &Initial_Type, "Initial_Type/I" );
	Tree->Branch("Initial_Energy_EeV", &Initial_Energy_EeV, "Initial_Energy_EeV/F" );
	Tree->Branch("Parent_Type", &Parent_Type, "Parent_Type/I" );
	Tree->Branch("Parent_Energy_EeV", &Parent_Energy_EeV, "Parent_Energy_EeV/F" );

}

void ROOTPhotonOutput1D::close() {
  omp_set_lock(&lock); 
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		Tree = 0;
	}
  omp_unset_lock(&lock); 
  omp_destroy_lock(&lock); 
}

ROOTPhotonOutput1D::~ROOTPhotonOutput1D() {
	close();
}

void ROOTPhotonOutput1D::process(crpropa::Candidate *c) const {
	int pid = c->current.getId();
	if ((pid != 22) and (abs(pid) != 11))
		return;

  omp_set_lock(&lock); 
		Particle_Type = pid;
		Energy_EeV = c->current.getEnergy() / crpropa::EeV;
		ComovingDistance_Mpc = c->created.getPosition().getR() / crpropa::Mpc;
		Parent_Type = c->created.getId();
		Parent_Energy_EeV = c->created.getEnergy();
		Initial_Type = c->source.getId();
		Initial_Energy_EeV = c->source.getEnergy();
		Tree->Fill();
  omp_unset_lock(&lock); 
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////// ROOT TRAJECTORY OUTPUT 1D //////////////////////////////
ROOTTrajectoryOutput1D::ROOTTrajectoryOutput1D(std::string filename) : crpropa::Module() {
	setDescription("ROOTTrajectoryOutput1D, filename: " + filename);
  omp_init_lock(&lock); 
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");

	Tree = new TTree("events", "CRPropa 1D trajectories");
  Tree->SetDirectory(ROOTFile); 
	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("Position_Mpc", &Position_Mpc, "Position_Mpc/F" );

	TThread::UnLock();
}

void ROOTTrajectoryOutput1D::close() {
  omp_set_lock(&lock); 
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		Tree = 0;
	}
  omp_unset_lock(&lock); 
  omp_destroy_lock(&lock); 
}

ROOTTrajectoryOutput1D::~ROOTTrajectoryOutput1D() {
	close();
}

void ROOTTrajectoryOutput1D::process(crpropa::Candidate *c) const {
  omp_set_lock(&lock);
  Position_Mpc = c->current.getPosition().getX() / crpropa::Mpc;
  Particle_Type = c->current.getId();
  Energy_EeV = c->current.getEnergy() / crpropa::EeV;
  Tree->Fill();
  omp_unset_lock(&lock); 
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////// ROOT EVENT OUTPUT 3D ///////////////////////////////////
ROOTEventOutput3D::ROOTEventOutput3D(std::string filename)  : crpropa::Module(){
	setDescription("ROOTEventOutput3D, filename: " + filename);
  omp_init_lock(&lock);
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");

	Tree = new TTree("events", "CRPropa 3D events");
  Tree->SetDirectory(ROOTFile); 
	Tree->Branch("TrajectoryLength_Mpc", &TrajectoryLength_Mpc, "TrajectoryLength_Mpc/F" );
	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Initial_Type", &Initial_Type, "Initial_Type/I" );
	Tree->Branch("Momentum_E_EeV", &Momentum_E_EeV, "Momentum_E_EeV/F" );
	Tree->Branch("Initial_Momentum_E_EeV", &Initial_Momentum_E_EeV, "Initial_Momentum_E_EeV/F" );
	Tree->Branch("Position_X_Mpc", &Position_X_Mpc, "Position_X_Mpc/F" );
	Tree->Branch("Position_Y_Mpc", &Position_Y_Mpc, "Position_Y_Mpc/F" );
	Tree->Branch("Position_Z_Mpc", &Position_Z_Mpc, "Position_Z_Mpc/F" );
	Tree->Branch("Initial_Position_X_Mpc", &Initial_Position_X_Mpc, "Initial_Position_X_Mpc/F" );
	Tree->Branch("Initial_Position_Y_Mpc", &Initial_Position_Y_Mpc, "Initial_Position_Y_Mpc/F" );
	Tree->Branch("Initial_Position_Z_Mpc", &Initial_Position_Z_Mpc, "Initial_Position_Z_Mpc/F" );
	Tree->Branch("Direction_X_Mpc", &Direction_X_Mpc, "Direction_X_Mpc/F" );
	Tree->Branch("Direction_Y_Mpc", &Direction_Y_Mpc, "Direction_Y_Mpc/F" );
	Tree->Branch("Direction_Z_Mpc", &Direction_Z_Mpc, "Direction_Z_Mpc/F" );

}

void ROOTEventOutput3D::close() {
  omp_set_lock(&lock); 
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		Tree = 0;
	}
  omp_unset_lock(&lock); 
  omp_destroy_lock(&lock); 
}

ROOTEventOutput3D::~ROOTEventOutput3D() {
	close();
}

void ROOTEventOutput3D::process(crpropa::Candidate *c) const {
  omp_set_lock(&lock); 
	TrajectoryLength_Mpc = c->getTrajectoryLength() / crpropa::Mpc;
	Particle_Type = c->current.getId();
	Initial_Type = c->source.getId();
	Momentum_E_EeV = c->current.getEnergy() / crpropa::EeV;
	Initial_Momentum_E_EeV = c->source.getEnergy() / crpropa::EeV;
	Position_X_Mpc = c->current.getPosition().x / crpropa::Mpc;
	Position_Y_Mpc = c->current.getPosition().y / crpropa::Mpc;
	Position_Z_Mpc = c->current.getPosition().z / crpropa::Mpc;
	Initial_Position_X_Mpc = c->source.getPosition().x / crpropa::Mpc;
	Initial_Position_Y_Mpc = c->source.getPosition().y / crpropa::Mpc;
	Initial_Position_Z_Mpc = c->source.getPosition().z / crpropa::Mpc;
	Direction_X_Mpc	= c->current.getDirection().x;
	Direction_Y_Mpc	= c->current.getDirection().y;
	Direction_Z_Mpc	= c->current.getDirection().z;

	Tree->Fill();
  omp_unset_lock(&lock); 
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////// ROOT TRAJECTORY OUTPUT 3D //////////////////////////////
ROOTTrajectoryOutput3D::ROOTTrajectoryOutput3D(std::string filename) : crpropa::Module() {
	setDescription("ROOTTrajectoryOutput3D, filename: " + filename);
  omp_init_lock(&lock);
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");
	Tree = new TTree("traj", "CRPropa 3D trajectories");
  Tree->SetDirectory(ROOTFile); 

	Tree->Branch("TrajectoryLength_Mpc", &TrajectoryLength_Mpc, "TrajectoryLength_Mpc/F" );
	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("Position_X_Mpc", &Position_X_Mpc, "Position_X_Mpc/F" );
	Tree->Branch("Position_Y_Mpc", &Position_Y_Mpc, "Position_Y_Mpc/F" );
	Tree->Branch("Position_Z_Mpc", &Position_Z_Mpc, "Position_Z_Mpc/F" );
	Tree->Branch("Direction_X_Mpc", &Direction_X_Mpc, "Direction_X_Mpc/F" );
	Tree->Branch("Direction_Y_Mpc", &Direction_Y_Mpc, "Direction_Y_Mpc/F" );
	Tree->Branch("Direction_Z_Mpc", &Direction_Z_Mpc, "Direction_Z_Mpc/F" );
}

void ROOTTrajectoryOutput3D::close() {
  omp_set_lock(&lock); 
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		Tree = 0;
	}
  omp_unset_lock(&lock); 
  omp_destroy_lock(&lock); 
}

ROOTTrajectoryOutput3D::~ROOTTrajectoryOutput3D() {
	close();
}

void ROOTTrajectoryOutput3D::process(crpropa::Candidate *c) const {
  omp_set_lock(&lock); 
		TrajectoryLength_Mpc = c->getTrajectoryLength() / crpropa::Mpc;
		Particle_Type = c->current.getId();
		Energy_EeV = c->current.getEnergy() / crpropa::EeV;
		Position_X_Mpc = c->current.getPosition().x / crpropa::Mpc;
		Position_Y_Mpc = c->current.getPosition().y / crpropa::Mpc;
		Position_Z_Mpc = c->current.getPosition().z / crpropa::Mpc;
		Direction_X_Mpc	= c->current.getDirection().x;
		Direction_Y_Mpc	= c->current.getDirection().y;
		Direction_Z_Mpc	= c->current.getDirection().z;
		Tree->Fill();
  omp_unset_lock(&lock); 
}

