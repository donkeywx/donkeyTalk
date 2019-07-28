#ifndef __NON_COPYABLE_H__
#define __NON_COPYABLE_H__

class NonCopyable
{
public:
    NonCopyable(){}
    ~NonCopyable(){}
private:
    NonCopyable(NonCopyable& nonCopyable);
    const NonCopyable& operator=(const NonCopyable&);
};

#endif 