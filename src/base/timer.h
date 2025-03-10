
#ifndef  _EOKAS_BASE_TIMER_H_
#define  _EOKAS_BASE_TIMER_H_

#include "header.h"

namespace eokas {
    
    class Timer {
        _ForbidCopy(Timer);
        _ForbidAssign(Timer);
    
    public:
        Timer();
        ~Timer();
    
    public:
        void reset();
        i64_t elapse(bool isReset = true);
    
    private:
        struct TimerImpl* mImpl;
    };
    
}

#endif//_EOKAS_BASE_TIMER_H_
