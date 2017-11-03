#ifndef __RANDOM_H__
#define __RANDOM_H__

// 这个主要是 for lua
class Random : public Ref
{
public:
    explicit Random( int seed );
    static Random* create( int seed );
    int nextInt( int a, int b = 0 );
    double nextDouble();

protected:
    mt19937 _rnd;
};

#endif
