#ifndef INONCOPYABLE_H
#define INONCOPYABLE_H


/**
 * ����COPY��,�̳л����������಻��COPY
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
