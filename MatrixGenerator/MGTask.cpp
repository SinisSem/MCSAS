#include "MGTask.h"
using namespace std;

MCSAS::MatrixGenerator::MGTask<double> GetStandardTask( size_t Number = 1 )
{
	MCSAS::MatrixGenerator::MGTask<double> Def;
	if (Number == 0)
	{
		vector<size_t> Chains;
		// 2  x 128
		Chains.push_back(128);
		Chains.push_back(128);
		// 2  x 64
		Chains.push_back(64);
		Chains.push_back(64);
		// 20 x 16
		for (int i = 0; i < 20; i++)
			Chains.push_back(16);
		// 50 x 8
		for (int i = 0; i < 50; i++)
			Chains.push_back(8);

		Def.ChainNodesNumber = Chains;
		Def.AdmittancesDispersion = 0.3;
		Def.AdditionDispersion = 1;
		Def.BaseAdmittance = 1;
		Def.BaseMainDiagonalAddition = 0.5;
		Def.RandomNetAdmittancesNumber = 50;
		Def.AdditionsNumber = 0.3;
		Def.EDSAdmittanceBase = 1;
		Def.EDSAdmittanceDispersion = 0;
		Def.EDSDispersion = 0;
		Def.EDSBase = 1;
		Def.EDSNumber = 0.1;
//		Def.CheckConductivity = false;
		Def.MaximumLinksPerChain = 32;

	}
	else if (Number == 1)
	{
		vector<size_t> Chains;

		for (size_t i = 0; i < 35; i++)
			Chains.push_back(32);

		Def.ChainNodesNumber = Chains;
		Def.AdmittancesDispersion = 0.3;
		Def.AdditionDispersion = 1;
		Def.BaseAdmittance = 1;
		Def.BaseMainDiagonalAddition = 0.5;
		Def.RandomNetAdmittancesNumber = 200;
		Def.AdditionsNumber = 0.3;
		Def.EDSAdmittanceBase = 1;
		Def.EDSAdmittanceDispersion = 0;
		Def.EDSDispersion = 0;
		Def.EDSBase = 1;
		Def.EDSNumber = 0.1;
//		Def.CheckConductivity = false;
		Def.MaximumLinksPerChain = 32;
	}


	return Def;
}
