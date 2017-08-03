#pragma once
#include "xx_mempool.h"
#include <limits>
#include <cmath>	// abs
#include <cassert>

namespace xx
{
	// 从 .NET System.Random 翻写, 理论上讲相同种子能输出相同结果. 支持序列化.
	// 必须传入种子
	struct Random : MPObject
	{
		Random(Random const&) = delete;
		Random& operator=(Random const&) = delete;

		// 并不清掉 o 的数据
		Random(Random && o)
		{
			inext = o.inext;
			inextp = o.inextp;
			memcpy(SeedArray, o.SeedArray, sizeof(SeedArray));
		}

		//
		// Private Constants 
		//
		static const int32_t MBIG = std::numeric_limits<int32_t>::max();
		static const int32_t MSEED = 161803398;
		static const int32_t MZ = 0;


		//
		// Member Variables
		//
		int32_t inext;
		int32_t inextp;
		int32_t SeedArray[56];


		//
		// Public Constants
		//

		//
		// Native Declarations
		//

		//
		// Constructors
		//

		explicit Random(int32_t seed = 0)
		{
			Init(seed);
		}

		void Init(int32_t seed = 0)
		{
			int32_t ii;
			int32_t mj, mk;

			//Initialize our Seed array.
			//This algorithm comes from Numerical Recipes in C (2nd Ed.)
			int32_t subtraction = (seed == std::numeric_limits<int32_t>::min()) ? std::numeric_limits<int32_t>::max() : std::abs(seed);
			mj = MSEED - subtraction;
			SeedArray[55] = mj;
			mk = 1;
			for (int32_t i = 1; i < 55; i++)
			{  //Apparently the range [1..55] is special (Knuth) and so we're wasting the 0'th position.
				ii = (21 * i) % 55;
				SeedArray[ii] = mk;
				mk = mj - mk;
				if (mk < 0) mk += MBIG;
				mj = SeedArray[ii];
			}
			for (int32_t k = 1; k < 5; k++)
			{
				for (int32_t i = 1; i < 56; i++)
				{
					SeedArray[i] -= SeedArray[1 + (i + 30) % 55];
					if (SeedArray[i] < 0) SeedArray[i] += MBIG;
				}
			}
			inext = 0;
			inextp = 21;
		}

		//
		// Package Private Methods
		//

		/*====================================Sample====================================
		**Action: Return a new random number [0..1) and reSeed the Seed array.
		**Returns: A double [0..1)
		**Arguments: None
		**Exceptions: None
		==============================================================================*/
	protected:
		inline double Sample()
		{
			//Including this division at the end gives us significantly improved
			//random number distribution.
			return (InternalSample() * (1.0 / MBIG));
		}
	private:
		inline int32_t InternalSample()
		{
			int32_t retVal;
			int32_t locINext = inext;
			int32_t locINextp = inextp;

			if (++locINext >= 56) locINext = 1;
			if (++locINextp >= 56) locINextp = 1;

			retVal = SeedArray[locINext] - SeedArray[locINextp];

			if (retVal == MBIG) retVal--;
			if (retVal < 0) retVal += MBIG;

			SeedArray[locINext] = retVal;

			inext = locINext;
			inextp = locINextp;

			return retVal;
		}

		//
		// Public Instance Methods
		// 


	public:
		/*=====================================Next=====================================
		**Returns: An int32_t [0..Int32.MaxValue)
		**Arguments: None
		**Exceptions: None.
		==============================================================================*/
		inline int32_t Next()
		{
			return InternalSample();
		}

	private:
		inline double GetSampleForLargeRange()
		{
			// The distribution of double value returned by Sample 
			// is not distributed well enough for a large range.
			// If we use Sample for a range [Int32.MinValue..Int32.MaxValue)
			// We will end up getting even numbers only.

			int32_t result = InternalSample();
			// Note we can't use addition here. The distribution will be bad if we do that.
			bool negative = (InternalSample() % 2 == 0) ? true : false;  // decide the sign based on second sample
			if (negative)
			{
				result = -result;
			}
			double d = result;
			d += (std::numeric_limits<int32_t>::max() - 1); // get a number in range [0 .. 2 * Int32MaxValue - 1)
			d /= 2 * (uint32_t)std::numeric_limits<int32_t>::max() - 1;
			return d;
		}


	public:
		/*=====================================Next=====================================
		**Returns: An int32_t [minvalue..maxvalue)
		**Arguments: minValue -- the least legal value for the Random number.
		**           maxValue -- One greater than the greatest legal return value.
		**Exceptions: None.
		==============================================================================*/
		inline int32_t Next(int32_t minValue, int32_t maxValue)
		{
			assert(minValue <= maxValue);

			int64_t range = (int64_t)maxValue - minValue;
			if (range <= (int64_t)std::numeric_limits<int32_t>::max())
			{
				return ((int32_t)(Sample() * range) + minValue);
			}
			else
			{
				return (int32_t)((int64_t)(GetSampleForLargeRange() * range) + minValue);
			}
		}


		/*=====================================Next=====================================
		**Returns: An int32_t [0..maxValue)
		**Arguments: maxValue -- One more than the greatest legal return value.
		**Exceptions: None.
		==============================================================================*/
		inline int32_t Next(int32_t maxValue)
		{
			assert(maxValue >= 0);
			return (int32_t)(Sample() * maxValue);
		}


		/*=====================================Next=====================================
		**Returns: A double [0..1)
		**Arguments: None
		**Exceptions: None
		==============================================================================*/
		inline double NextDouble()
		{
			return Sample();
		}


		///*==================================NextBytes===================================
		//**Action:  Fills the byte array with random bytes [0..0x7f].  The entire array is filled.
		//**Returns:Void
		//**Arugments:  buffer -- the array to be filled.
		//**Exceptions: None
		//==============================================================================*/
		//void NextBytes(BBuffer* buffer)
		//{
		//	for (uint32_t i = 0; i < buffer->dataLen; i++)
		//	{
		//		buffer->buf[i] = (uint8_t)(InternalSample() % (std::numeric_limits<uint8_t>::max() + 1));
		//	}
		//}

		inline double NextDouble(double minValue, double maxValue)
		{
			if (minValue == maxValue || maxValue - minValue <= 0) return minValue;
			return minValue + (maxValue - minValue) * NextDouble();
		}
	};



	/*************************************************************************/
	// 实现值类型使用类型声明
	/*************************************************************************/

	using Random_v = Dock<Random>;
	using Random_p = Ptr<Random>;

	struct MemmoveSupport<Random_v>
	{
		static const bool value = true;
	};
}
