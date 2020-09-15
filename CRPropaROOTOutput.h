/// ROOT Output Plugin for crpropa  
///  Originally by Tobias Winchen 
//   Updated by Cosmin Deaconu <cozzyd@kicp.uchicago.edu> 2020 

#include <crpropa/Module.h>
#include <TFile.h>
#include <TTree.h>
#include <TThread.h>
#include <omp.h> 



/**
 @class ROOTTrajectoryOutput1D
 @brief Saves 1D trajectory information to a ROOT file.
 */
class ROOTTrajectoryOutput1D: public crpropa::Module {
	mutable TFile *ROOTFile;
	mutable TTree *Tree;

	mutable int Particle_Type;
	mutable float Energy_EeV;
	mutable float Position_Mpc;
public:
	ROOTTrajectoryOutput1D(std::string filename);
	~ROOTTrajectoryOutput1D();
	void process(crpropa::Candidate *candidate) const;
  mutable omp_lock_t lock; 
	void close();
};

/**
 @class ROOTTrajectoryOutput3D
 @brief Saves 3D trajectory information to a ROOT file.
 */
class ROOTTrajectoryOutput3D: public crpropa::Module {
	mutable TFile *ROOTFile;
	mutable TTree *Tree;
	mutable int Particle_Type;
	mutable float Energy_EeV;
	mutable float TrajectoryLength_Mpc;
	mutable float Position_X_Mpc, Position_Y_Mpc, Position_Z_Mpc;
	mutable float Direction_X_Mpc, Direction_Y_Mpc, Direction_Z_Mpc;

public:
	ROOTTrajectoryOutput3D(std::string filename);
	~ROOTTrajectoryOutput3D();
	void process(crpropa::Candidate *candidate) const;
  mutable omp_lock_t lock; 
	void close();
};

/**
 @class ROOTEventOutput1D
 @brief Saves 1D event information to a ROOT file.
 */
class ROOTEventOutput1D: public crpropa::Module {
	mutable TFile *ROOTFile;
	mutable TTree *Tree;

	mutable int Particle_Type, Initial_Type;
	mutable float Energy_EeV, Initial_Energy_EeV;
	mutable float TrajectoryLength_Mpc;
public:
	ROOTEventOutput1D(std::string filename);
	~ROOTEventOutput1D();
	void process(crpropa::Candidate *candidate) const;
  mutable omp_lock_t lock; 
	void close();
};

/**
 @class ROOTEventOutput3D
 @brief Saves 3D event information to a ROOT file.
 */
class ROOTEventOutput3D: public crpropa::Module {
	mutable TFile *ROOTFile;
	mutable TTree *Tree;

	mutable int Particle_Type, Initial_Type;
	mutable float Momentum_E_EeV, Initial_Momentum_E_EeV;
	mutable float TrajectoryLength_Mpc;

	mutable float Position_X_Mpc, Position_Y_Mpc, Position_Z_Mpc;
	mutable float Initial_Position_X_Mpc, Initial_Position_Y_Mpc, Initial_Position_Z_Mpc;
	mutable float Direction_X_Mpc, Direction_Y_Mpc, Direction_Z_Mpc;

public:
	ROOTEventOutput3D(std::string filename);
	~ROOTEventOutput3D();
	void process(crpropa::Candidate *candidate) const;
  mutable omp_lock_t lock; 
	void close();
};

/**
 @class ROOTPhotonOutput1D
 @brief Records EM-particles to a ROOT file.
 */
class ROOTPhotonOutput1D: public crpropa::Module {
	mutable TFile *ROOTFile;
	mutable TTree *Tree;

	mutable int Particle_Type, Initial_Type, Parent_Type;
	mutable float Energy_EeV, Initial_Energy_EeV, Parent_Energy_EeV;
	mutable float ComovingDistance_Mpc;
public:
	ROOTPhotonOutput1D(std::string filename);
	~ROOTPhotonOutput1D();
	void process(crpropa::Candidate *candidate) const;
  mutable omp_lock_t lock; 
	void close();
};





