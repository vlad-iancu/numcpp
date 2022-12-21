#ifndef NUMCPP_VEC_H
#define NUMCPP_VEC_H

#include <numcpp/defs.hpp>

namespace npp
{
	template<typename T>
	class vec
	{
	public:
		explicit vec(u64 n)
		{
			v = new T*[n];
		}
		~vec()
		{
			if(owns)
			{
				for(u64 i = 0;i < n; i++)
				{
					delete v[i];
				}
			}
			delete v;
		}

	protected:
		 explicit vec(u64 n, T **v)
		 {
			 this->v = v;
			 this->n = n;
			 owns = false;
		 }

	protected:
		bool owns;
		T *v;
		u64 n;
	
	public:
		T get(u64 i)
		{
			return v[i];
		}
		void set(u64 i, T value);
		vec<T>* slice(u64 start, u64 end)
		{

		}
	};
}

#endif
