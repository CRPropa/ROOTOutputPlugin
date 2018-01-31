#include "CRPropaROOTOutput.h"

#include <string>

// The parent's constructor need to be called on initialization!
ROOTOutput::ROOTOutput() : crpropa::Module()
{
	setDescription("ROOTOutput");
}

void ROOTOutput::process(crpropa::Candidate *candidate) const
{
	// To enable parallelization, the modules have to be stateless - the
	// process method should thus not modify internal variables!
}


