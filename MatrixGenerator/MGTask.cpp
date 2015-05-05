#include "MGTask.h"
#include "../Types/BaseTypes.h"
using namespace std;


template struct MCSAS::MatrixGenerator::MGTask<float>;
template struct MCSAS::MatrixGenerator::MGTask<double>;
template struct MCSAS::MatrixGenerator::MGTask<MCSAS::Types::BaseTypes::ComplexDoubleCartesian>;
template struct MCSAS::MatrixGenerator::MGTask<MCSAS::Types::BaseTypes::ComplexDoubleHybrid>;
template struct MCSAS::MatrixGenerator::MGTask<MCSAS::Types::BaseTypes::ComplexDoublePolar>;
template struct MCSAS::MatrixGenerator::MGTask<MCSAS::Types::BaseTypes::ComplexFloatCartesian>;
template struct MCSAS::MatrixGenerator::MGTask<MCSAS::Types::BaseTypes::ComplexFloatHybrid>;
template struct MCSAS::MatrixGenerator::MGTask<MCSAS::Types::BaseTypes::ComplexFloatPolar>;

template<>
MCSAS::MatrixGenerator::MGTask<double> MCSAS::MatrixGenerator::GetStandardTask( size_t Number )
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
