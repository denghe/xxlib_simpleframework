using System;
using System.Collections.Generic;
using System.Text;

namespace xx
{
    /// <summary>
    /// 这是从 System.Random 复制的源代码. 并加上了序列化功能( 与 C++ 那边保持一致 )
    /// </summary>
    public class Random : IBBuffer
    {
        public ushort GetPackageId()
        {
            return TypeIdMaps<Random>.typeId;
        }

        public void ToBBuffer(BBuffer bb)
        {
            bb.Reserve(bb.dataLen + 58 * 4);
            var nexts = new int[2];
            nexts[0] = inext;
            nexts[1] = inextp;
            Buffer.BlockCopy(nexts, 0, bb.buf, bb.dataLen, 8);
            Buffer.BlockCopy(SeedArray, 0, bb.buf, bb.dataLen + 8, 56 * 4);
            bb.dataLen += 58 * 4;
        }

        public void FromBBuffer(BBuffer bb)
        {
            if (bb.offset + 58 * 4 > bb.dataLen) throw new IndexOutOfRangeException();
            var nexts = new int[2];
            inext = nexts[0];
            inextp = nexts[1];
            Buffer.BlockCopy(bb.buf, bb.offset, nexts, 0, 8);
            Buffer.BlockCopy(bb.buf, bb.offset + 8, SeedArray, 0, 56 * 4);
            bb.offset += 58 * 4;
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
        public void ToString(ref StringBuilder s)
        {
            s.Append("{ \"type\":\"Random\" }");
        }
        public void ToStringCore(ref StringBuilder sb) { }

        bool toStringFlag;
        public void SetToStringFlag(bool doing)
        {
            toStringFlag = doing;
        }
        public bool GetToStringFlag()
        {
            return toStringFlag;
        }



        //public string FindDiff(Random o, string rootName = "")
        //{
        //    string rtv = null;
        //    if (inext != o.inext) return rootName + " inext is diff! this is " + inext + ", other is " + o.inext;
        //    if (inextp != o.inextp) return rootName + " inextp is diff! this is " + inextp + ", other is " + o.inextp;
        //    for (int i = 0; i < SeedArray.Length; i++)
        //    {
        //        if (SeedArray[i] != o.SeedArray[i]) return rootName + " SeedArray[" + i + "] is diff!";
        //    }
        //    return rtv;
        //}


        /*************************************************************************************************/
        // 下面全是 MS 的代码, 改了一下 throw, private -> public, 删了 Debug 断点
        /*************************************************************************************************/


        //
        // Private Constants 
        //
        public const int MBIG = Int32.MaxValue;
        public const int MSEED = 161803398;
        public const int MZ = 0;


        //
        // Member Variables
        //
        public int inext;
        public int inextp;
        public int[] SeedArray = new int[56];


        //
        // Public Constants
        //

        //
        // Native Declarations
        //

        //
        // Constructors
        //

        public Random()
          : this(Environment.TickCount)
        {
        }

        public Random(int Seed)
        {
            int ii;
            int mj, mk;

            //Initialize our Seed array.
            //This algorithm comes from Numerical Recipes in C (2nd Ed.)
            int subtraction = (Seed == Int32.MinValue) ? Int32.MaxValue : Math.Abs(Seed);
            mj = MSEED - subtraction;
            SeedArray[55] = mj;
            mk = 1;
            for (int i = 1; i < 55; i++)
            {  //Apparently the range [1..55] is special (Knuth) and so we're wasting the 0'th position.
                ii = (21 * i) % 55;
                SeedArray[ii] = mk;
                mk = mj - mk;
                if (mk < 0) mk += MBIG;
                mj = SeedArray[ii];
            }
            for (int k = 1; k < 5; k++)
            {
                for (int i = 1; i < 56; i++)
                {
                    SeedArray[i] -= SeedArray[1 + (i + 30) % 55];
                    if (SeedArray[i] < 0) SeedArray[i] += MBIG;
                }
            }
            inext = 0;
            inextp = 21;
            Seed = 1;
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
        protected virtual double Sample()
        {
            //Including this division at the end gives us significantly improved
            //random number distribution.
            return (InternalSample() * (1.0 / MBIG));
        }

        private int InternalSample()
        {
            int retVal;
            int locINext = inext;
            int locINextp = inextp;

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


        /*=====================================Next=====================================
        **Returns: An int [0..Int32.MaxValue)
        **Arguments: None
        **Exceptions: None.
        ==============================================================================*/
        public virtual int Next()
        {
            return InternalSample();
        }

        private double GetSampleForLargeRange()
        {
            // The distribution of double value returned by Sample 
            // is not distributed well enough for a large range.
            // If we use Sample for a range [Int32.MinValue..Int32.MaxValue)
            // We will end up getting even numbers only.

            int result = InternalSample();
            // Note we can't use addition here. The distribution will be bad if we do that.
            bool negative = (InternalSample() % 2 == 0) ? true : false;  // decide the sign based on second sample
            if (negative)
            {
                result = -result;
            }
            double d = result;
            d += (Int32.MaxValue - 1); // get a number in range [0 .. 2 * Int32MaxValue - 1)
            d /= 2 * (uint)Int32.MaxValue - 1;
            return d;
        }


        /*=====================================Next=====================================
        **Returns: An int [minvalue..maxvalue)
        **Arguments: minValue -- the least legal value for the Random number.
        **           maxValue -- One greater than the greatest legal return value.
        **Exceptions: None.
        ==============================================================================*/
        public virtual int Next(int minValue, int maxValue)
        {
            if (minValue > maxValue)
            {
                throw new ArgumentOutOfRangeException("minValue > maxValue");
            }

            long range = (long)maxValue - minValue;
            if (range <= (long)Int32.MaxValue)
            {
                return ((int)(Sample() * range) + minValue);
            }
            else
            {
                return (int)((long)(GetSampleForLargeRange() * range) + minValue);
            }
        }


        /*=====================================Next=====================================
        **Returns: An int [0..maxValue)
        **Arguments: maxValue -- One more than the greatest legal return value.
        **Exceptions: None.
        ==============================================================================*/
        public virtual int Next(int maxValue)
        {
            if (maxValue < 0)
            {
                throw new ArgumentOutOfRangeException("maxValue < 0");
            }
            return (int)(Sample() * maxValue);
        }


        /*=====================================Next=====================================
        **Returns: A double [0..1)
        **Arguments: None
        **Exceptions: None
        ==============================================================================*/
        public virtual double NextDouble()
        {
            return Sample();
        }


        /*==================================NextBytes===================================
        **Action:  Fills the byte array with random bytes [0..0x7f].  The entire array is filled.
        **Returns:Void
        **Arugments:  buffer -- the array to be filled.
        **Exceptions: None
        ==============================================================================*/
        public virtual void NextBytes(byte[] buffer)
        {
            for (int i = 0; i < buffer.Length; i++)
            {
                buffer[i] = (byte)(InternalSample() % (Byte.MaxValue + 1));
            }
        }

    }
}
