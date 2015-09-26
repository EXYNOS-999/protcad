//*******************************************************************************************************
//*******************************************************************************************************
//*****************************                         *************************************************
//*****************************  intraSoluteEnergy 1.0  *************************************************
//*****************************                         *************************************************
//*******************************************************************************************************
//*******************************************************************************************************

#include <iostream>
#include <string>
#include "ensemble.h"
#include "PDBInterface.h"
#include <time.h>

//--Program setup-------------------------------------------------------------
int main (int argc, char* argv[])
{	
    clock_t t;
	string infile = argv[1];
	enum aminoAcid {A, R, N, D, Dh, C, Q, E, Eh, G, H, O, I, L, K, M, F, P, S, T, W, Y, V, dA, dR, dN, dD, dDh, dC, dQ, dE, dEh, dH, dO, dI, dL, dK, dM, dF, dP, dS, dT, dW, dY, dV};
	PDBInterface* thePDB = new PDBInterface(infile);
	ensemble* theEnsemble = thePDB->getEnsemblePointer();
	molecule* pMol = theEnsemble->getMoleculePointer(0);
	protein* bundle = static_cast<protein*>(pMol);
	residue::setCutoffDistance(9.0);
	rotamer::setScaleFactor(0.0);
    amberVDW::setScaleFactor(1.0);
    amberVDW::setRadiusScaleFactor(1.0);
	amberVDW::setLinearRepulsionDampeningOff();
    amberElec::setScaleFactor(1.0);

    t=clock();
    cout << bundle->intraSoluteEnergy(true) << endl;
    t=clock()-t;

    cout << "Time to run intraSoluteEnergy: " << ((float)t)/CLOCKS_PER_SEC << endl;

    t=clock();
    cout << bundle->protEnergy() << endl;
    t=clock()-t;

    cout << "Time to run first protEnergy: " << ((float)t)/CLOCKS_PER_SEC << endl;

    t=clock();
    cout << bundle->protEnergy() << endl;
    t=clock()-t;

    cout << "Time to run second protEnergy: " << ((float)t)/CLOCKS_PER_SEC << endl;

    t=clock();
    double aveE = bundle->getAverageResEnergy();
    t=clock()-t;

    cout << "Time to run second averageResEnergy: " << ((float)t)/CLOCKS_PER_SEC << endl;
    cout << aveE << " " << bundle->resEnergy(0,0) << endl;

	return 0;
}
