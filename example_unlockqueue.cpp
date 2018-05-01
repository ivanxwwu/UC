//
// Created by root on 17-10-19.
//


#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
//http://www.linuxidc.com/Linux/2016-12/137937.htm?foxhandler=RssReadRenderProcessHandler
class UnlockQueue
{
public:
    UnlockQueue(int nSize);
    virtual ~UnlockQueue();

    bool Initialize();

    unsigned int Put(const unsigned char *pBuffer, unsigned int nLen);
    unsigned int Get(unsigned char *pBuffer, unsigned int nLen);

    inline void Clean() { m_nIn = m_nOut = 0; }
    inline unsigned int GetDataLen() const { return  m_nIn - m_nOut; }

private:
    inline bool is_power_of_2(unsigned long n) { return (n != 0 && ((n & (n - 1)) == 0)); };
    inline unsigned long roundup_power_of_two(unsigned long val);

private:
    unsigned char *m_pBuffer;    /* the buffer holding the data */
    unsigned int   m_nSize;        /* the size of the allocated buffer */
    unsigned int   m_nIn;        /* data is added at offset (in % size) */
    unsigned int   m_nOut;        /* data is extracted from off. (out % size) */
};

UnlockQueue::UnlockQueue(int nSize)
        :m_pBuffer(NULL)
        ,m_nSize(nSize)
        ,m_nIn(0)
        ,m_nOut(0)
{
    //round up to the next power of 2
    if (!is_power_of_2(nSize))
    {
        m_nSize = roundup_power_of_two(nSize);
    }
}

UnlockQueue::~UnlockQueue()
{
    if(NULL != m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = NULL;
    }
}

bool UnlockQueue::Initialize()
{
    m_pBuffer = new unsigned char[m_nSize];
    if (!m_pBuffer)
    {
        return false;
    }

    m_nIn = m_nOut = 0;

    return true;
}

unsigned long UnlockQueue::roundup_power_of_two(unsigned long val)
{
    if((val & (val-1)) == 0)
        return val;

    unsigned long maxulong = (unsigned long)((unsigned long)~0);
    unsigned long andv = ~(maxulong&(maxulong>>1));
    while((andv & val) == 0)
        andv = andv>>1;

    return andv<<1;
}

unsigned int UnlockQueue::Put(const unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    len = std::min(len, m_nSize - m_nIn + m_nOut);

    /*
     * Ensure that we sample the m_nOut index -before- we
     * start putting bytes into the UnlockQueue.
     */
    __sync_synchronize();

    /* first put the data starting from fifo->in to buffer end */
    l = std::min(len, m_nSize - (m_nIn  & (m_nSize - 1)));
    memcpy(m_pBuffer + (m_nIn & (m_nSize - 1)), buffer, l);

    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(m_pBuffer, buffer + l, len - l);

    /*
     * Ensure that we add the bytes to the kfifo -before-
     * we update the fifo->in index.
     */
    __sync_synchronize();

    m_nIn += len;

    return len;
}

unsigned int UnlockQueue::Get(unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    len = std::min(len, m_nIn - m_nOut);

    /*
     * Ensure that we sample the fifo->in index -before- we
     * start removing bytes from the kfifo.
     */
    __sync_synchronize();

    /* first get the data from fifo->out until the end of the buffer */
    l = std::min(len, m_nSize - (m_nOut & (m_nSize - 1)));
    memcpy(buffer, m_pBuffer + (m_nOut & (m_nSize - 1)), l);

    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + l, m_pBuffer, len - l);

    /*
     * Ensure that we remove the bytes from the kfifo -before-
     * we update the fifo->out index.
     */
    __sync_synchronize();

    m_nOut += len;

    return len;
}


struct student_info
{
    long stu_id;
    unsigned int age;
    unsigned int score;
};

void print_student_info(const student_info *stu_info)
{
    if(NULL == stu_info)
        return;

    printf("id:%ld\t",stu_info->stu_id);
    printf("age:%u\t",stu_info->age);
    printf("score:%u\n",stu_info->score);
}

student_info * get_student_info(time_t timer)
{
    student_info *stu_info = (student_info *)malloc(sizeof(student_info));
    if (!stu_info)
    {
        fprintf(stderr, "Failed to malloc memory.\n");
        return NULL;
    }
    srand(timer);
    stu_info->stu_id = 10000 + rand() % 9999;
    stu_info->age = rand() % 30;
    stu_info->score = rand() % 101;
    //print_student_info(stu_info);
    return stu_info;
}

void * consumer_proc(void *arg)
{
    UnlockQueue* queue = (UnlockQueue *)arg;
    student_info stu_info;
    while(1)
    {
        sleep(1);
        unsigned int len = queue->Get((unsigned char *)&stu_info, sizeof(student_info));
        if(len > 0)
        {
            printf("------------------------------------------\n");
            printf("UnlockQueue length: %u\n", queue->GetDataLen());
            printf("Get a student\n");
            print_student_info(&stu_info);
            printf("------------------------------------------\n");
        }
    }
    return (void *)queue;
}

void * producer_proc(void *arg)
{
    time_t cur_time;
    UnlockQueue *queue = (UnlockQueue*)arg;
    while(1)
    {
        time(&cur_time);
        srand(cur_time);
        int seed = rand() % 11111;
        printf("******************************************\n");
        student_info *stu_info = get_student_info(cur_time + seed);
        printf("put a student info to queue.\n");
        queue->Put( (unsigned char *)stu_info, sizeof(student_info));
        free(stu_info);
        printf("UnlockQueue length: %u\n", queue->GetDataLen());
        printf("******************************************\n");
        sleep(1);
    }
    return (void *)queue;
}


int main()
{
    UnlockQueue unlockQueue(1024);
    if(!unlockQueue.Initialize())
    {
        return -1;
    }

    pthread_t consumer_tid, producer_tid;

    printf("multi thread test.......\n");

    if(0 != pthread_create(&producer_tid, NULL, producer_proc, (void*)&unlockQueue))
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
                errno, strerror(errno));
        return -1;
    }

    if(0 != pthread_create(&consumer_tid, NULL, consumer_proc, (void*)&unlockQueue))
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
                errno, strerror(errno));
        return -1;
    }

    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    return 0;
}
