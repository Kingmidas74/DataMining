#pragma once

namespace DataMining
{
	namespace Normalization
	{
		class NormalizationFactory
		{
		public:
			template<typename Type, typename ... Args>
			static NormalizationBase<Type> * GetNormalization(NormalizationTypes type, Args... args)
			{
				NormalizationBase<Type> * result;
				switch (type)
				{
					case NormalizationTypes::ZeroMean: result = new ZeroMeanNormalization<Type>(); break;
					case NormalizationTypes::Mean: result = new MeanNormalization<Type>(); break;
					case NormalizationTypes::None: result = new NormalizationBase<Type>(); break;
					default:  result = new NormalizationBase<Type>(); break;
				}
				return result;
			}
		};
	}
}
