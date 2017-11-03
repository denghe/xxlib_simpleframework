#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

class Stopwatch
{
public:
    Stopwatch() : _begin( high_resolution_clock::now() ) {}
    void reset() { _begin = high_resolution_clock::now(); }

    // ∫¡√Î
    inline int64_t elapsedMillseconds() const
    {
        return duration_cast<milliseconds>( high_resolution_clock::now() - _begin ).count();
    }

    // Œ¢√Î
    inline int64_t elapsedMicroseconds() const
    {
        return duration_cast<microseconds>( high_resolution_clock::now() - _begin ).count();
    }

    // ƒ…√Î
    inline int64_t elapsedNanoseconds() const
    {
        return duration_cast<nanoseconds>( high_resolution_clock::now() - _begin ).count();
    }

    // √Î
    inline int64_t elapsedSeconds() const
    {
        return duration_cast<seconds>( high_resolution_clock::now() - _begin ).count();
    }

    // ∑÷
    inline int64_t elapsedMinutes() const
    {
        return duration_cast<minutes>( high_resolution_clock::now() - _begin ).count();
    }

    //  ±
    inline int64_t elapsedHours() const
    {
        return duration_cast<hours>( high_resolution_clock::now() - _begin ).count();
    }

private:
    time_point<high_resolution_clock> _begin;
};

#endif
