#include <iostream>
#include <string>
#include "ensemble.h"
#include "PDBInterface.h"

int main (int argc, char* argv[])
{
	string infile = argv[1];
	// read in protein
	PDBInterface* thePDB = new PDBInterface(infile);
	ensemble* theEnsemble = thePDB->getEnsemblePointer();
	molecule* pMol = theEnsemble->getMoleculePointer(0);
	protein* prot = static_cast<protein*>(pMol);

	// center on backbone centroid
	dblVec center = prot->getBackBoneCentroid();
	center = center * -1.0;
	prot->translate(center);
	double bestProjection = 100000;
	double bestPhi, bestTheta, bestPsi;

	double phimin = -180.0; double phimax = 180.0; double step = 20.0;
	double thetamin = -180.0; double thetamax = 180.0;
	double psimin = -180.0; double psimax = 180.0;
	for (UInt i = 0; i < 5; i ++)
	{
		cout << endl;
		cout << "*********" << endl;
		cout << " CYCLE " << i+1 << endl;
		cout << endl;

		for (double phi = phimin; phi <= phimax; phi += step)
		{
			for (double theta = thetamin; theta <= thetamax; theta += step)
			{
				for (double psi = psimin; psi <= psimax; psi += step)
				{
					prot->eulerRotate(phi, theta, psi);
					double projection = 0.0;
					for (UInt i = 0; i < prot->getNumChains(); i ++)
					{
						chain* tempChain = prot->getChain(i);
						dblVec chainCentroid = tempChain->getBackBoneCentroid();
						dblVec chainSum(3);
						chainSum[0] = 0.0; chainSum[1] = 0.0; chainSum[2] = 0.0;
						for (UInt j = 0; j < tempChain->itsResidues.size(); j++)
						{
							residue* tempRes = tempChain->getResidue(j);
							chainSum = chainSum + (chainCentroid - tempRes->getBackBoneCentroid());
							chainSum[1] = 0.0;
							double magnitude = sqrt(CMath::dotProduct(chainSum,chainSum));
							projection += magnitude;
						}
					}
					prot->undoEulerRotate(phi, theta, psi);
					//cout << phi << " " << theta << " " << psi << " " << projection << endl;
					if (projection < bestProjection)
					{
						bestProjection = projection;
						bestPhi = phi;
						bestTheta = theta;
						bestPsi = psi;
					}
				}
			}
		}
		cout << "phi " << bestPhi << endl;
		cout << "psi " << bestPsi << endl;
		cout << "theta " << bestTheta << endl;
		cout << "best projection " << bestProjection << endl;
		cout << endl;
		phimin = bestPhi - step;
		phimax = bestPhi + step;
		thetamin = bestTheta - step;
		thetamax = bestTheta + step;
		psimin = bestPsi - step;
		psimax = bestPsi + step;
		step = step/5;
	}

	cout << "**************BEST***************" << endl;
	cout << "phi " << bestPhi << endl;
	cout << "psi " << bestPsi << endl;
	cout << "theta " << bestTheta << endl;
	prot->eulerRotate(bestPhi, bestTheta, bestPsi);
	pdbWriter(prot, infile);
	return 0;
}



