//
// Created by root on 17-9-1.
//



//-------------------------------------------
//Cola's Hash Shm Library 1.03
//colaliang( SNG Instant Messaging Application Department )
//last update: 2012-12-27

//Makefile
//	g++ -O3 -c hash_shm.cpp
// ar cq libchs.a hash_shm.o

//Enable extra function:
//1. CAS
// -DHASH_SHM_ENABLE_CAS

//2. TIMER_CALL_BACK
//-DHASH_SHM_ENABLE_TCB

//多级hash实现解释
//
//
//					i
//PRIMER_TABLE:	2	3	5	7	11	13	17	( bucket size, using primes )
//PRIMER_TABLE_TATAL:	0	2	5	10	17	28	41	( line address begin position )
//
//1. 冲突解决:通过检查 ( _key + lines*factor ) % PRIMER_TABLE[i]; 的位置是否已经设置, 否则检查下一个位置
//2. hash 多级取模, 通过计算素数来尽量散列
//3. lines 指定了最多的可能冲突次数
//4. maxline 确定共享内级的冲突最大级数
//5.初始化时需要指定 valueType, 元素固定长度, 不能为stl

//------------------------------------------

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<sys/shm.h>
#include <string.h>
#include<fstream>
#include<vector>
#include<string>
using  std::cout;
using  std::cerr;
using  std::string;
using  std::vector;
using  std::ifstream;
using  std::ofstream;
using  std::ios;

extern const int PRIMER_TABLE_LEN;
extern const int PRIMER_TABLE[ ] ;

enum HASH_RETURN_CODE
{
    HASHSHM_OK = 0,
    HASHSHM_KEYEXIST,
    HASHSHM_ERROR,
    HASHSHM_NOTFOUND,
    HASHSHM_INSERTERROR,
    HASHSHM_OUTOFMEM,
    HASHSHM_UPDATE_ERROR,
};

enum HASH_STATUS
{
    HASH_STATUS_NORMAL			= 0x1,
    HASH_STATUS_WRITE_ONLY	= 0x2,
    HASH_STATUS_READ_ONLY		= 0x4
};

enum HASH_ENABLE_FLAG
{
    HASH_ENABLE_NONE = 0x0,
    HASH_ENABLE_DETACH = 0x1,		//detach from shm, when all process detach,the shm will release

};

//max conflict time, max items = MAX_LINES * BaseBucketLen ( default 100w )
const int MAX_LINES = 32;

//hash factor, use for better hash distrubition
const int FACTOR = 5381;

//use for thread magic id
const int THREAD_STEP = 1;

//string hash time33
unsigned long hash_time33(char const *str, int len =-1 );

template< typename valueType, int lines = MAX_LINES, int thread_step = THREAD_STEP >
class CHashShm
{
public:

    CHashShm():bInitOk(false){};
    virtual ~CHashShm();

    //init with the share memory key,it will get share memory
    //if fail,exit
    bool Init( key_t shm_key );

public:
    //set node into shm
    //	1) if the _key exists,return HASHSHM_KEYEXIST
    //	2) if set success,return HASHSHM_OK
    //	3) if fail return HASHSHM_ERROR
    int Set( const unsigned long _key ,const valueType &_value);
    int Set( const char* skey, const int len ,const valueType &_value );
    int Set( const string& strkey ,const valueType &_value );

    //get node from shm
    int Get( const unsigned long _key,  valueType& _value );
    int Get( const char* skey, const int len, valueType& _value );
    int Get(  const string& strkey , valueType &_value );

    int Replace( const unsigned long _key ,const valueType& _value );
    int Replace( const char* skey, const int len,const valueType& _value );
    int Replace(  const string& strkey ,const valueType &_value );

    //if _key not in the table,return HASHSHM_NOTFOUND, else remove the node,set the node key 0 and return HASHSHM_OK
    int Remove( const unsigned long _key );
    int Remove( const char* skey, const int len );
    int Remove( const string& strkey );

    //callback function/param for execute, param_in will be pass to callback function as param_out
    void Foreach( void (*callback)( const unsigned long _key, valueType& _value, void* param_out ), void* param_in );

    //remove all the data
    void Clear();

    //operation enable behavior
    int SetBehavior( unsigned int iflag );
    int UnsetBehavior( unsigned int iflag );

    bool IsInitOK(){ return bInitOk; }
public:
    bool BinDump( char* filename = "./chsbin" );
    bool BinRestore( char* filename = "./chsbin" );

public:
    //the rate of the space used
    double GetFullRate() const;

    //the bucket size( begin 0 )
    void GetBucketSize( unsigned int index ) const ;

    //get one bucket's item count
    int GetBucketUseSize( unsigned int index ) const ;

    unsigned long GetCurSize() { return m_hashHead->currentSize; }

    unsigned long GetSize() { return maxSize; };

private:
    //the start position of the share memory
    //  1) the begin mem space used to storage the runtime data, reserved 16 byte
    //  2) currentSize = (unsigned long *)((long)mem)
    void *mem;

    //current size of the table ,the pointer of the shm begin
    struct hash_head{
        unsigned long currentSize;
        unsigned long status;
        unsigned long reservered2;
        unsigned long reservered3;
    };
    hash_head * m_hashHead;

    //the size of the share memory
    unsigned long memSize;

    //PRIMER_TABLE_TATAL[i] is the summary of the PRIMER_TABLE when x<=i
    unsigned long PRIMER_TABLE_TATAL[lines];

    //the size of the table
    unsigned long maxSize;

    //write by the find function,record the last find place
    void *lastFound;

    //Init flag
    bool bInitOk;

    //enable operation flag
    unsigned int flag;

    //the node of the hash table
    //	1) when key==0,the node is empty
    //	2) name-value pair
    struct hash_node{
        unsigned long key;
        valueType value;
    };

private:
    //if _key in the table,return HASHSHM_OK,and set lastFound the position,otherwise return HASHSHM_NOTFOUND
    int find( const unsigned long _key );

    //get share memory,used by the constructor
    bool getShm( key_t shm_key );

    //get the positon with the (row,col), map to line pos
    void *getPos( const unsigned int _row,  const unsigned long _col )
    {
        //calculate the positon from the start
        unsigned long pos =  PRIMER_TABLE_TATAL[_row] + _col;

        if ( pos >= maxSize + sizeof(hash_head))
            return NULL;

        return (void *)((long)mem+ sizeof(hash_head) + pos*sizeof(hash_node));
    }

};

template< typename vT, int lines, int thread_step >
bool CHashShm<vT,lines,thread_step>::Init( key_t shm_key )
{
    if( lines > PRIMER_TABLE_LEN )
        return false;

    //constructor with get share memory
    maxSize=0;

    int i;
    for(i=0;i<lines;i++)
    {
        //caculate the PRIMER_TABLE_TATAL
        maxSize+=PRIMER_TABLE[i];
        if(i!=0)
            PRIMER_TABLE_TATAL[i] = PRIMER_TABLE_TATAL[i-1]+PRIMER_TABLE[i-1];
        else
            PRIMER_TABLE_TATAL[i]=0;
    }

    //extra 16byte for use
    memSize=sizeof(hash_node)*maxSize + sizeof(hash_head);
    if(!getShm( shm_key ))
        bInitOk = false;
    else
    {

        m_hashHead = (hash_head*)((long)mem );
        m_hashHead->currentSize = 0;

        //initialize as normal
        m_hashHead->status = HASH_STATUS_NORMAL;

        //init operation enable to default none
        flag = HASH_ENABLE_NONE;

        bInitOk = true;
    }

    return bInitOk;
}

template< typename vT, int lines, int thread_step >
CHashShm<vT,lines,thread_step>::~CHashShm()
{
    //detach from share mem if HASH_ENABLE_DETACH setd. Because
    if( flag& HASH_ENABLE_DETACH )
        shmdt( mem );
}

template< typename vT, int lines, int thread_step >
void CHashShm<vT,lines,thread_step>::Clear()
{
    memset(mem,0,memSize);
    m_hashHead->currentSize=0;
}

template< typename vT, int lines, int thread_step >
bool CHashShm<vT,lines,thread_step>::getShm( key_t shm_key )
{
    int shm_id=shmget(shm_key,memSize,0666);

    //check if the shm exists
    if( shm_id==-1 )
    {
        //create the shm
        shm_id=shmget(shm_key,memSize,0666|IPC_CREAT);
        if(shm_id==-1){
            cerr<<"Share memory get failed\n";
            return false;
        }

        //create the shm
        mem=shmat(shm_id,NULL,0);

        memset(mem,0,memSize);

        if(int(mem)==-1){
            cerr<<"shmat system call failed\n";
            return false;
        }
    }
    else
    {
        //exist, point to the shm
        mem=shmat(shm_id,NULL,0);

        if(int(mem)==-1){
            cerr<<"shmat system call failed\n";
            return false;
        }
    }

    return true;
}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::find( const unsigned long _key)
{
    unsigned long hash;
    hash_node *pH=NULL;
    for(int i=0;i<lines;i++)
    {
        //calculate the col position
        hash = ( _key + lines * FACTOR ) % PRIMER_TABLE[i];
        pH = ( hash_node *)getPos( i, hash );

        //position exceed the shm size, just break
        if( NULL == pH )
            break;

        if( pH->key == _key )
        {
            lastFound=pH;
            return HASHSHM_OK;
        }
    }

    return HASHSHM_NOTFOUND;
}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Set( const unsigned long _key,const vT&_value)
{
    //if the key exists
    if( find(_key )== HASHSHM_OK )
        return HASHSHM_KEYEXIST;

    unsigned long hash;
    hash_node *pH=NULL;

    for(int i=0;i<lines;i++)
    {
        //minimize conflict using primes
        // 1) firs hash pos( calculate the col position )
        hash=( _key + lines * FACTOR ) % PRIMER_TABLE[i];

        // 2) second hash pos( row, col )
        pH=(hash_node *)getPos( i,hash );

        // insert position exceed the shm size
        if( NULL == pH )
            return HASHSHM_OUTOFMEM;

        //find the insert position,insert the value
        if( pH->key== 0 )
        {

            pH->key = _key;
            pH->value = _value;

            m_hashHead->currentSize++;

            return HASHSHM_OK;
        }
    }

    //all the appropriate position filled
    return HASHSHM_ERROR;
}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Set( const char* skey, const int len, const vT &_value )
{
    unsigned long ulHashKey = hash_time33( skey, len );
    return Set( ulHashKey, _value );

}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Set( const string& strkey, const vT &_value )
{
    unsigned long ulHashKey = hash_time33( strkey.data(), strkey.size() );
    return Set( ulHashKey, _value );

}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Get( const unsigned long _key, vT& _value )
{
    if( find( _key ) != HASHSHM_OK )
        return HASHSHM_NOTFOUND;

    //memset( &_value, &((hash_node*)lastFound)->value, sizeof(_value) );
    //Do I need memset?( c++' bitwise copy )
    _value = ((hash_node*)lastFound)->value;

    return HASHSHM_OK;
}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Get( const char* skey, const int len, vT&_value )
{
    unsigned long ulHashKey = hash_time33( skey, len );
    return Get( ulHashKey, _value );

}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Get( const string& strkey, vT&_value )
{
    unsigned long ulHashKey = hash_time33( strkey.data(), strkey.size() );
    return Get( ulHashKey, _value );

}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Replace( const unsigned long _key,const vT &_value)
{
    //if the key exists, replace the value
    if( find(_key )== HASHSHM_OK )
    {
        ((hash_node*)lastFound)->value = _value;
        return HASHSHM_OK;
    }

    //if not found, the find a hash place
    unsigned long hash;
    hash_node *pH=NULL;

    for(int i=0;i<lines;i++)
    {
        //minimize conflict using primes
        // 1) firs hash pos( calculate the col position )
        hash=( _key + lines * FACTOR ) % PRIMER_TABLE[i];

        // 2) second hash pos( row, col )
        pH=(hash_node *)getPos( i,hash );

        // insert position exceed the shm size
        if( NULL == pH )
            return HASHSHM_OUTOFMEM;

        //find the insert position,insert the value
        if( pH->key== 0 )
        {
            pH->key = _key;
            pH->value = _value;
            m_hashHead->currentSize++;
            return HASHSHM_OK;
        }
    }

    //all the appropriate position filled
    return HASHSHM_ERROR;
}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Replace( const char* skey, const int len, const vT&_value )
{
    unsigned long ulHashKey = hash_time33( skey, len );
    return Replace( ulHashKey, _value );

}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Replace( const string& strkey, const vT&_value )
{
    unsigned long ulHashKey = hash_time33( strkey.data(), strkey.size() );
    return Replace( ulHashKey, _value );

}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Remove( const unsigned long _key)
{
    //not found
    if( find(_key) != HASHSHM_OK )
        return HASHSHM_NOTFOUND;

    hash_node *pH=(hash_node *)lastFound;

    //only set the key 0
    pH->key=0;
    m_hashHead->currentSize--;

    return HASHSHM_OK;
}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Remove( const char* skey, const int len )
{
    unsigned long ulHashKey = hash_time33( skey, len );
    return Remove( ulHashKey );

}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::Remove( const string& strkey )
{
    unsigned long ulHashKey = hash_time33( strkey.data(), strkey.size() );
    return Remove( ulHashKey );

}

template< typename vT, int lines, int thread_step >
void CHashShm<vT,lines,thread_step>::Foreach(void (*callback)( const unsigned long _key,vT &_value,void * param_out ), void* param_in )
{
    typedef  unsigned long u_long;
    u_long beg=(u_long)mem + sizeof(hash_head);
    u_long end=(u_long)mem+ sizeof(hash_head) + sizeof(hash_node)*(PRIMER_TABLE[lines-1]+PRIMER_TABLE_TATAL[lines-1]);

    hash_node *p=NULL;
    for(u_long pos=beg;pos<end;pos+=sizeof(hash_node))
    {
        //directly referece the actual memory place, so value can be modify outside directly
        p=(hash_node *)pos;
        if(p->key!=0)
            callback( p->key,p->value, param_in );
    }
}

template< typename vT, int lines, int thread_step >
bool CHashShm<vT,lines,thread_step>::BinDump( char* filename)
{

    ofstream os( filename, ios::out | ios::binary );
    if( !os )
        return false;

    os.write( (char*)mem, memSize );
    os.close();

    return true;
}

template< typename vT, int lines, int thread_step >
bool CHashShm<vT,lines,thread_step>::BinRestore( char* filename)
{

    ifstream ios( filename, ios::binary );
    if( !ios )
        return false;

    // get length of file:
    ios.seekg (0, ios::end);
    unsigned long   file_length = ios.tellg();
    ios.seekg (0, ios::beg);

    if( file_length != memSize )
        return false;

    ios.read( (char*)mem, memSize );

    ios.close();
    return true;
}

//the rate of the space used
template< typename vT, int lines, int thread_step >
double CHashShm<vT,lines,thread_step>::GetFullRate() const
{
    return double( m_hashHead->currentSize )/maxSize;
};

//the bucket size( begin 0 )
template< typename vT, int lines, int thread_step >
void CHashShm<vT,lines,thread_step>::GetBucketSize( unsigned int index ) const
{
    return index>=lines?0:PRIMER_TABLE[index];
} ;

//get one bucket's item count
template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::GetBucketUseSize( unsigned int index )  const
{
    if(  index>=lines )
        return 0;

    int sum = 0;
    hash_node * pNode = (hash_node *)((long)mem+ sizeof(hash_head) + index*sizeof(hash_node));

    for( int i=0; i<PRIMER_TABLE[index]; i++ )
    {
        if( pNode->key != 0 )
            ++sum;
    }

    return sum;
}

//set operation enable flag
template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::SetBehavior( unsigned int iflag )
{
    flag |= iflag;
    return flag;
}

template< typename vT, int lines, int thread_step >
int CHashShm<vT,lines,thread_step>::UnsetBehavior( unsigned int iflag )
{
    flag &= ~iflag;
    return flag;
}


//-------------------------------------------
//Cola's Hash Shm Library 1.03
//colaliang( SNG Instant Messaging Application Department )
//last update: 2012-12-27




const int PRIMER_TABLE_LEN = 64;
const int PRIMER_TABLE[ PRIMER_TABLE_LEN ] = {
        1511,1523,1531,1543,1549,1553,1559,1567,1571,1579,1583,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657,1663,1667,1669,1693,
        1697,1699,1709,1721,1723,1733,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811,1823,1831,1847,1861,1867,1871,1873,1877,1879,
        1889,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987,1993,1997,1999
};

unsigned long hash_time33(char const *str, int len  )
{
    //get from php
    unsigned long hash = 5381;

    //variant with the hash unrolled eight times
    // if len not specify, use default time33
    char const *p = str;
    if( len < 0 )
    {
        for(; *p; p++)
        {
            hash = hash * 33 + *p;
        }

        return hash;
    }

#define TIME33_HASH_MIXED_CH() hash = ((hash<<5)+hash) + *p++
    //use eighe alignment
    for (; len >= 8; len -= 8)
    {
        TIME33_HASH_MIXED_CH();	// 1
        TIME33_HASH_MIXED_CH(); // 2
        TIME33_HASH_MIXED_CH();	// 3
        TIME33_HASH_MIXED_CH(); // 4
        TIME33_HASH_MIXED_CH(); // 5
        TIME33_HASH_MIXED_CH(); // 6
        TIME33_HASH_MIXED_CH(); // 7
        TIME33_HASH_MIXED_CH(); // 8
    }
    switch (len)
    {
        case 7: TIME33_HASH_MIXED_CH();
        case 6: TIME33_HASH_MIXED_CH();
        case 5: TIME33_HASH_MIXED_CH();
        case 4: TIME33_HASH_MIXED_CH();
        case 3: TIME33_HASH_MIXED_CH();
        case 2: TIME33_HASH_MIXED_CH();
        case 1: TIME33_HASH_MIXED_CH(); break;
        case 0: break;
    }

    return hash;
}

