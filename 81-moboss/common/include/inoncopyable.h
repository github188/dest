#ifndef INONCOPYABLE_H
#define INONCOPYABLE_H


/**
 * 不可COPY类,继承或包含本类的类不可COPY
 */
class INonCopyable
{
public:
    INonCopyable()
    {
    }
    ~INonCopyable()
    {
    }
private:  // emphasize the following members are private
    INonCopyable( const INonCopyable& );
    const INonCopyable& operator=( const INonCopyable& );
};

#endif
