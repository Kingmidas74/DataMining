#pragma once

/**
IndexContainer.hpp
Purpose: Custom container for arrays.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/

namespace ParallelClustering
{
	template<class Content>
	class IndexContainer
	{

	private:

		Content* content;

		unsigned int my_size;
		unsigned int my_capacity;

	public:
		
		int Length;

		IndexContainer(): my_size(0), my_capacity(0), Length(0)
		{
			
		}

		explicit IndexContainer(unsigned int size)
		{
			content = allocateAlign<Content>(size);
			my_size = size;
			my_capacity = size;
			Length = 0;
		}

		explicit IndexContainer(unsigned int size, Content defaultValue)
		{
			content = allocateAlign<Content>(size);
			my_size = size;
			my_capacity = size;
			Length = 0;
			for(unsigned int i=0; i<size; i++)
			{
				content[i] = defaultValue;
				Length++;
			}
		}

		void Replace(IndexContainer<Content> old)
		{	
			for(auto element:old)
			{
				//Append(element);
			}
			//Length = old.Length;
		}

		~IndexContainer()
		{
			
		}
		
		Content * begin()
		{
			return Length>0 ? &content[0] : nullptr;
		}

		Content * end()
		{
			return Length>0 ? &content[Length] : nullptr;
		}


		Content & operator[](unsigned int index) {
			return content[index];
		}

		IndexContainer<Content> Substruct(unsigned int start, unsigned int count)
		{
			auto result = IndexContainer<Content>(count);
			//cout << "SUBSTRUCT:" << start << "_" << count << endl;
			for(unsigned int i=start; i<start+count; i++)
			{
				result.Append(content[i]);
			}
			return result;
		}

		void Clear()
		{
			if (sizeof(content) / sizeof(Content) > 0)
			{
				freeAlign<Content>(content);
				my_size = 0;
				Length = 0;
			}
		}


		void ReUse()
		{
			Length = 0;
		}

		void Append(Content &item)
		{
			if (my_size > 0)
			{
				content[Length] = item;
				Length++;
			}
			else
			{
				throw "Memory is not allocated!";
			}
		}

		


		

		void RemoveByIndex(int &index)
		{	
			if (Length == 0 || Length==index) throw "Invalid element index";
			if (Length == 1) 
			{
				ReUse();
				return;
			}
			for (int i = index+1;i < Length; i++)
			{
				content[i-1] = content[i];
			}
			Length--;
		}

	};

}