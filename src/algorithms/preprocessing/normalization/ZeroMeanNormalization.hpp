#pragma once

namespace DataMining
{
	namespace Normalization
	{
		template<typename Type>
		class ZeroMeanNormalization:public NormalizationBase<Type>
		{

		public:
			ZeroMeanNormalization() :NormalizationBase<Type>() {};

			virtual bool Normalize(Type* array, size_t length) 
			{ 
				Type avg=0;
				for(size_t i=0; i<length;i++)
				{
					avg=avg+array[i];
				}
				avg=avg/length;
				Type MAD = 0;
				for(size_t i=0; i<length;i++)
				{
					MAD=MAD+(abs(array[i]-avg));
				}
				MAD=MAD/length;
				if(abs(MAD-0)<0.00000001) return false;
				for(size_t i=0; i<length;i++)
				{
					array[i]=(array[i]-avg)/MAD;
				}
				return true;
			};

			virtual ~ZeroMeanNormalization()
			{
			}
		};
	}
}
