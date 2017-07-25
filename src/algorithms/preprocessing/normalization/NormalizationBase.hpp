#pragma once

namespace DataMining
{
	namespace Normalization
	{
		template<typename Type>
		class NormalizationBase
		{

		public:
			NormalizationBase() {};

			virtual bool Normalize(Type* array, size_t length) 
			{ 
				return true; 
			};

			virtual ~NormalizationBase()
			{
			}
		};
	}
}
