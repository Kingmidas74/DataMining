#pragma once

namespace DataMining
{
	namespace Normalization
	{
		template<typename Type>
		class MeanNormalization:public NormalizationBase<Type>
		{

		public:
			MeanNormalization() :NormalizationBase<Type>() {};

			virtual bool Normalize(Type * array, size_t length)
			{
				Type sum = 0;
				for (size_t i = 0; i < length; i++) {
					sum += array[i];
				}
				for (size_t i = 0; i < length; i++) {
					array[i] /= sum;
				}
				return true;
			};

			virtual ~MeanNormalization()
			{
			}
		};
	}
}
