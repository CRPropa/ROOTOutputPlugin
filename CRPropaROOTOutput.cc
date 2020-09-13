#include "CRPropaROOTOutput.h"
#include <crpropa/Units.h>
#include <crpropa/ParticleID.h>
#include <crpropa/Cosmology.h>

#include <string>



/////////////////////// ROOT EVENT OUTPUT 1D //////////////////////////////////
ROOTEventOutput1D::ROOTEventOutput1D(std::string filename) {
	setDescription("ROOTEventOutput1D, filename: " + filename);
	TThread::Lock();
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");
	Tree = new TTree("events", "CRPropa 1D events");
	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("TrajectoryLength_Mpc", &TrajectoryLength_Mpc, "TrajectoryLength_Mpc/F" );
	Tree->Branch("Initial_Type", &Initial_Type, "Initial_Type/I" );
	Tree->Branch("Initial_Energy_EeV", &Initial_Energy_EeV, "Initial_Energy_EeV/F" );
	TThread::UnLock();
}

void ROOTEventOutput1D::close(){
	TThread::Lock();
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		delete Tree;
		Tree = 0;
	}
	TThread::UnLock();
}


ROOTEventOutput1D::~ROOTEventOutput1D() {
	close();
}

void ROOTEventOutput1D::process(Candidate *c) const {
	TThread::Lock();
	#pragma omp critical
	{
		Particle_Type = c->current.getId();
		Energy_EeV = c->current.getEnergy() / EeV;
		TrajectoryLength_Mpc = c->getTrajectoryLength() / Mpc;
		Initial_Type = c->source.getId();
		Initial_Energy_EeV = c->source.getEnergy() / EeV;
		Tree->Fill();
	}
	TThread::UnLock();
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////// ROOT PHOTON OUTPUT 1D //////////////////////////////////
ROOTPhotonOutput1D::ROOTPhotonOutput1D(std::string filename) {
	setDescription("ROOTPHOTONOutput1D, filename: " + filename);
	TThread::Lock();
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa photon output data file");
	Tree = new TTree("events", "CRPropa 1D photons");

	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("ComovingDistance_Mpc", &ComovingDistance_Mpc, "ComovingDistance_Mpc/F" );
	Tree->Branch("Initial_Type", &Initial_Type, "Initial_Type/I" );
	Tree->Branch("Initial_Energy_EeV", &Initial_Energy_EeV, "Initial_Energy_EeV/F" );
	Tree->Branch("Parent_Type", &Parent_Type, "Parent_Type/I" );
	Tree->Branch("Parent_Energy_EeV", &Parent_Energy_EeV, "Parent_Energy_EeV/F" );

	TThread::UnLock();
}

void ROOTPhotonOutput1D::close() {
	TThread::Lock();
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		delete Tree;
		Tree = 0;
	}
	TThread::UnLock();
}

ROOTPhotonOutput1D::~ROOTPhotonOutput1D() {
	close();
}

void ROOTPhotonOutput1D::process(Candidate *c) const {
	int pid = c->current.getId();
	if ((pid != 22) and (abs(pid) != 11))
		return;

	TThread::Lock();
#pragma omp critical
	{
		Particle_Type = pid;
		Energy_EeV = c->current.getEnergy() / EeV;
		ComovingDistance_Mpc = c->created.getPosition().getR() / Mpc;
		Parent_Type = c->created.getId();
		Parent_Energy_EeV = c->created.getEnergy();
		Initial_Type = c->source.getId();
		Initial_Energy_EeV = c->source.getEnergy();
		Tree->Fill();
	}
	TThread::UnLock();
}
////////////////////////////////////////////////////////////////////////////////


/////////////////////// ROOT TRAJECTORY OUTPUT 1D //////////////////////////////
ROOTTrajectoryOutput1D::ROOTTrajectoryOutput1D(std::string filename) {
	setDescription("ROOTTrajectoryOutput1D, filename: " + filename);
	TThread::Lock();
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");

	Tree = new TTree("events", "CRPropa 1D trajectories");
	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("Position_Mpc", &Position_Mpc, "Position_Mpc/F" );

	TThread::UnLock();
}

void ROOTTrajectoryOutput1D::close() {
	TThread::Lock();
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		delete Tree;
		Tree = 0;
	}
	TThread::UnLock();
}

ROOTTrajectoryOutput1D::~ROOTTrajectoryOutput1D() {
	close();
}

void ROOTTrajectoryOutput1D::process(Candidate *c) const {
	TThread::Lock();
	#pragma omp critical
	{
		Position_Mpc = c->current.getPosition().getX() / Mpc;
		Particle_Type = c->current.getId();
		Energy_EeV = c->current.getEnergy() / EeV;
		Tree->Fill();
	}
	TThread::UnLock();
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////// ROOT EVENT OUTPUT 3D ///////////////////////////////////
ROOTEventOutput3D::ROOTEventOutput3D(std::string filename) {
	setDescription("ROOTEventOutput3D, filename: " + filename);
	TThread::Lock();
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");

	Tree = new TTree("events", "CRPropa 3D events");
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

	TThread::UnLock();
}

void ROOTEventOutput3D::close() {
	TThread::Lock();
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		delete Tree;
		Tree = 0;
	}
	TThread::UnLock();
}

ROOTEventOutput3D::~ROOTEventOutput3D() {
	close();
}

void ROOTEventOutput3D::process(Candidate *c) const {
	TThread::Lock();
	#pragma omp critical
	{
		TrajectoryLength_Mpc = c->getTrajectoryLength() / Mpc;
		Particle_Type = c->current.getId();
		Initial_Type = c->source.getId();
		Momentum_E_EeV = c->current.getEnergy() / EeV;
		Initial_Momentum_E_EeV = c->source.getEnergy() / EeV;
		Position_X_Mpc = c->current.getPosition().x / Mpc;
		Position_Y_Mpc = c->current.getPosition().y / Mpc;
		Position_Z_Mpc = c->current.getPosition().z / Mpc;
		Initial_Position_X_Mpc = c->source.getPosition().x / Mpc;
		Initial_Position_Y_Mpc = c->source.getPosition().y / Mpc;
		Initial_Position_Z_Mpc = c->source.getPosition().z / Mpc;
		Direction_X_Mpc	= c->current.getDirection().x;
		Direction_Y_Mpc	= c->current.getDirection().y;
		Direction_Z_Mpc	= c->current.getDirection().z;

		Tree->Fill();
	}
	TThread::UnLock();
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////// ROOT TRAJECTORY OUTPUT 3D //////////////////////////////
ROOTTrajectoryOutput3D::ROOTTrajectoryOutput3D(std::string filename) {
	setDescription("ROOTTrajectoryOutput3D, filename: " + filename);
	TThread::Lock();
	ROOTFile = new TFile(filename.c_str(), "RECREATE",
			"CRPropa output data file");
	Tree = new TTree("traj", "CRPropa 3D trajectories");

	Tree->Branch("TrajectoryLength_Mpc", &TrajectoryLength_Mpc, "TrajectoryLength_Mpc/F" );
	Tree->Branch("Particle_Type", &Particle_Type, "Particle_Type/I");
	Tree->Branch("Energy_EeV", &Energy_EeV, "Energy_EeV/F" );
	Tree->Branch("Position_X_Mpc", &Position_X_Mpc, "Position_X_Mpc/F" );
	Tree->Branch("Position_Y_Mpc", &Position_Y_Mpc, "Position_Y_Mpc/F" );
	Tree->Branch("Position_Z_Mpc", &Position_Z_Mpc, "Position_Z_Mpc/F" );
	Tree->Branch("Direction_X_Mpc", &Direction_X_Mpc, "Direction_X_Mpc/F" );
	Tree->Branch("Direction_Y_Mpc", &Direction_Y_Mpc, "Direction_Y_Mpc/F" );
	Tree->Branch("Direction_Z_Mpc", &Direction_Z_Mpc, "Direction_Z_Mpc/F" );

	TThread::UnLock();
}

void ROOTTrajectoryOutput3D::close() {
	TThread::Lock();
	if (ROOTFile) {
		ROOTFile->Write();
		ROOTFile->Close();
		delete ROOTFile;
		ROOTFile = 0;
		delete Tree;
		Tree = 0;
	}
	TThread::UnLock();
}

ROOTTrajectoryOutput3D::~ROOTTrajectoryOutput3D() {
	close();
}

void ROOTTrajectoryOutput3D::process(Candidate *c) const {
	TThread::Lock();
#pragma omp critical
	{
		TrajectoryLength_Mpc = c->getTrajectoryLength() / Mpc;
		Particle_Type = c->current.getId();
		Energy_EeV = c->current.getEnergy() / EeV;
		Position_X_Mpc = c->current.getPosition().x / Mpc;
		Position_Y_Mpc = c->current.getPosition().y / Mpc;
		Position_Z_Mpc = c->current.getPosition().z / Mpc;
		Direction_X_Mpc	= c->current.getDirection().x;
		Direction_Y_Mpc	= c->current.getDirection().y;
		Direction_Z_Mpc	= c->current.getDirection().z;
		Tree->Fill();
	}
	TThread::UnLock();
}

