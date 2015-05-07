#include "BaseTypesRandom.h"
#include <random>
#include <ctime>
#include "BaseTypes.h"
#include "BaseTypesMACRO.h"

namespace MCSAS
{
	namespace Types
	{
		namespace BaseTypes
		{
			void StartRandom()
			{
				std::srand((size_t)std::time(0));
			}

			template<>
			double RandomBaseDispersion(double Base, double Dispersion)
			{
				double x = ((double)rand() / (double)(RAND_MAX)) - 0.5;			// Случайное число [-0.5,0.5]
				return (Base + (Dispersion + Dispersion) * x);
			}

			template<>
			float RandomBaseDispersion(float Base, float Dispersion)
			{
				return (double)RandomBaseDispersion((double)Base, (double)Dispersion);
			}


			template<>
			int RandomBaseDispersion(int Base, int Dispersion)
			{
				return (double)RandomBaseDispersion((double)Base, (double)Dispersion);
			}

			template<>
			size_t RandomBaseDispersion(size_t Base, size_t Dispersion)
			{
				return (double)RandomBaseDispersion((double)Base, (double)Dispersion);
			}

			template<>
			ComplexDoubleCartesian RandomBaseDispersion(ComplexDoubleCartesian Base, ComplexDoubleCartesian Dispersion)
			{
				ComplexDoubleCartesian Ret;
				Ret.Re = RandomBaseDispersion(Base.Re, Dispersion.Re);
				Ret.Im = RandomBaseDispersion(Base.Im, Dispersion.Im);
				return Ret;
			}

			template<>
			ComplexDoubleHybrid RandomBaseDispersion(ComplexDoubleHybrid Base, ComplexDoubleHybrid Dispersion)
			{
				ComplexDoubleCartesian Base_;
				H2C(Base.Distance, Base.AngleSin, Base.AngleCos, Base_.Re, Base_.Im);
				ComplexDoubleCartesian Dispersion_;
				H2C(Dispersion.Distance, Dispersion.AngleSin, Dispersion.AngleCos, Dispersion_.Re, Dispersion_.Im);

				ComplexDoubleCartesian Ret_ = RandomBaseDispersion(Base_, Dispersion_);

				ComplexDoubleHybrid Ret;
				C2H(Ret_.Re, Ret_.Im, Ret.Distance, Ret.AngleSin, Ret.AngleCos);

				return Ret;
			}

			template<>
			ComplexDoublePolar RandomBaseDispersion(ComplexDoublePolar Base, ComplexDoublePolar Dispersion)
			{
				ComplexDoubleCartesian Base_;
				P2C(Base.Distance, Base.Angle, Base_.Re, Base_.Im);
				ComplexDoubleCartesian Dispersion_;
				P2C(Dispersion.Distance, Dispersion.Angle, Dispersion_.Re, Dispersion_.Im);

				ComplexDoubleCartesian Ret_ = RandomBaseDispersion(Base_, Dispersion_);

				ComplexDoublePolar Ret;
				C2P(Ret_.Re, Ret_.Im, Ret.Distance, Ret.Angle);

				return Ret;
			}

			template<>
			ComplexFloatCartesian RandomBaseDispersion(ComplexFloatCartesian Base, ComplexFloatCartesian Dispersion)
			{
				ComplexFloatCartesian Ret;
				Ret.Re = RandomBaseDispersion(Base.Re, Dispersion.Re);
				Ret.Im = RandomBaseDispersion(Base.Im, Dispersion.Im);
				return Ret;
			}

			template<>
			ComplexFloatHybrid RandomBaseDispersion(ComplexFloatHybrid Base, ComplexFloatHybrid Dispersion)
			{
				ComplexFloatCartesian Base_;
				H2C(Base.Distance, Base.AngleSin, Base.AngleCos, Base_.Re, Base_.Im);
				ComplexFloatCartesian Dispersion_;
				H2C(Dispersion.Distance, Dispersion.AngleSin, Dispersion.AngleCos, Dispersion_.Re, Dispersion_.Im);

				ComplexFloatCartesian Ret_ = RandomBaseDispersion(Base_, Dispersion_);

				ComplexFloatHybrid Ret;
				C2H(Ret_.Re, Ret_.Im, Ret.Distance, Ret.AngleSin, Ret.AngleCos);

				return Ret;
			}

			template<>
			ComplexFloatPolar RandomBaseDispersion(ComplexFloatPolar Base, ComplexFloatPolar Dispersion)
			{
				ComplexFloatCartesian Base_;
				P2C(Base.Distance, Base.Angle, Base_.Re, Base_.Im);
				ComplexFloatCartesian Dispersion_;
				P2C(Dispersion.Distance, Dispersion.Angle, Dispersion_.Re, Dispersion_.Im);

				ComplexFloatCartesian Ret_ = RandomBaseDispersion(Base_, Dispersion_);

				ComplexFloatPolar Ret;
				C2P(Ret_.Re, Ret_.Im, Ret.Distance, Ret.Angle);

				return Ret;
			}

			template<>
			double RandomFromTo(double From, double To)
			{
				if (To < From)
				{
					double temp = To;
					To = From;
					From = temp;
				}
				if (To == From)
					return To;

				double Dispersion = To - From;
				Dispersion *= ((double)rand() / (double)(RAND_MAX));
				double Random = From + Dispersion;
				return Random;
			}

			template<>
			int RandomFromTo(int From, int To)
			{
				return (int)RandomFromTo((double)From, (double)To);
			}

			template<>
			size_t RandomFromTo(size_t From, size_t To)
			{
				return (size_t)RandomFromTo((double)From, (double)To);
			}
		}
	}
}