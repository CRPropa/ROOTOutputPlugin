/// Example plugin for CRPropa.
///
/// Please consider sharing the awesome plugin with you fellow researchers by
/// creating a eperate repository for your project. We maintain a list of
/// plugins to CRPropa on our webpage and are happy to add a link to your
/// project, just send us: (name of the plugin, short description, url)

#include <crpropa/Module.h>


/// A custom C++ module
class ROOTOutput : public crpropa::Module
{
public:
	/// The parent's constructor need to be called on initialization!
	ROOTOutput();
	void process(crpropa::Candidate *candidate) const;
};


