#include "BaseSetting.h"
#include "Work.h"


int main(int argc, char* argv[])
{
    //setLogging(argv[0]);
    LOG_INFO << "pid = " << getpid() << ", tid = " << muduo::CurrentThread::tid();

#if 1
    Work work;
    work.Start();
#endif
    return 0;
}

