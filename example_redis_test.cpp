//
// Created by root on 17-9-17.
//

#include "hiredis/hiredis.h"
#include "stdlib.h"
#include "hiredis/async.h"
#include <unistd.h>
#include "string.h"
#include <string>
#include <map>
using namespace std;
//#include <hiredis/adapters/libev.h>
//void disconnectCallback(const struct redisAsyncContext*, int status){
//    printf("dis conn %s\n", "callback");
//}
//
//void connectCallback(const struct redisAsyncContext*, int status){
//    printf("conn ballback\n");
//}
//
//void getCallback(redisAsyncContext *c, void *r, void *privdata) {
//    redisReply *reply = (redisReply*)r;
//    if (reply == NULL) return;
//    printf("argv[%s]: %s\n", (char*)privdata, reply->str);
//    sleep(3);
//
//    /* Disconnect after receiving the reply to GET */
//}

int fn1() {
    redisContext* connect;
    redisReply* reply;
    connect = redisConnect("127.0.0.1", 6379);
    if (connect != NULL && connect->err){
        printf("connect error:%s\n", connect->errstr);
        exit(1);
    }
    reply = (redisReply*)redisCommand(connect, "setex %b %d %b", "d", 1, 100, "c", 1);
    if (strncmp(reply->str, "OK", reply->len) == 0 ) {
        printf("123\n");
    }
    printf("%s   %d %d\n", reply->str, reply->elements, reply->type);
    //REDIS_REPLY_ERROR
    printf("===========================\n");
    freeReplyObject(reply);
    redisFree(connect);
}

//
// Created by ivanxwwu on 2020/6/5.
//

#include <string>
#include <queue>
#include "hiredis/hiredis.h"

using namespace std;



#define REDIS_ERRORCODE_POOL_NO_CONNECTION  -5000001
#define REDIS_ERRORCODE_SET_ERR -5100002

class HiredisHelper
{
public:
    void init();
    void run();

    HiredisHelper();
    ~HiredisHelper();

    int SetEx(const string& key, const string& value, int ttl);

private:
    void connect();
    redisContext* getConntection();
    void releaseConnection(redisContext* ctx, bool active);
    bool checkStatus(redisContext* ctx);

private:
    int m_timeout;
    int m_port;
    string m_ip;
    string m_authstring;
    string m_onsaddr;
    queue<redisContext*> m_ctxs;
    int m_poolsize;
    int m_maxpoolsize;
    int m_useNum;
};

#include <sstream>
#include "string.h"

#define CHECK_FREE_REDIS_REPLY(reply) \
    if (reply) {\
        freeReplyObject(reply);\
    } else {\
        cout << "freeReplyObject Fail" <<endl;\
    }

HiredisHelper::HiredisHelper() {

}

HiredisHelper::~HiredisHelper() {
    for (size_t i=0; i<m_ctxs.size(); i++) {
        redisContext *ctx = m_ctxs.front();
        redisFree(ctx);
        ctx = NULL;
        m_ctxs.pop();
    }
}

void HiredisHelper::init()
{
    m_onsaddr = "sz4260.adsstat_completion.redis.com";
    m_ip = "127.0.0.1";
    m_port = 6379;
    m_timeout = 5000;
    m_authstring = "DiX%X/UT9OY2rthS";
    m_poolsize = 10;
    m_maxpoolsize = 100;
    m_useNum = 0;
    connect();
}

void HiredisHelper::run()
{
    while(true) {
        sleep(1000);
    }
}
#include <iostream>
using namespace std;
int HiredisHelper::SetEx(const string& key, const string& value, int ttl) {
    redisContext *ctx = getConntection();
    if (ctx == NULL) {
        return REDIS_ERRORCODE_POOL_NO_CONNECTION;
    }

    redisReply *reply = (redisReply*)redisCommand(ctx, "setex %b %d %b", key.c_str(), key.size(), ttl, value.c_str(), value.size());

    if (reply) {
        if (strncmp(reply->str, "OK", reply->len) != 0) {
            cout << "set fail" << endl;
        }
        releaseConnection(ctx, true);
        return 0;
    } else {
        cout << "set reply null" << endl;
        releaseConnection(ctx, false);
        return REDIS_ERRORCODE_SET_ERR;
    }
}

void HiredisHelper::connect() {
    timeval tv;
    tv.tv_sec = m_timeout/1000;
    //轮询建立连接
    for (int i=0; i<m_poolsize; i++) {
        redisContext* ctx = redisConnectWithTimeout(m_ip.c_str(), m_port, tv);
        if (ctx != NULL && ctx->err) {
            cout << "connect fail" << endl;
            continue;
        }
        m_ctxs.push(ctx);
    }
}

redisContext* HiredisHelper::getConntection()
{
    {
        if(!m_ctxs.empty())
        {
            m_useNum++;
            redisContext *ctx = m_ctxs.front();
            m_ctxs.pop();
            return ctx;
        }
        //连接池无连接则新建连接
        //已使用的连接大于连接池最大数直接拒绝
        if (m_useNum >= m_maxpoolsize) {
            return NULL;
        }
    }

    timeval tv;
    tv.tv_sec = m_timeout/1000;
    redisContext *ctx = redisConnectWithTimeout(m_ip.c_str(), m_port, tv);
    if(ctx == NULL || ctx->err != 0)
    {
        if(ctx != NULL) redisFree(ctx);
        return NULL;
    }
    {
        m_useNum++;
    }

    return ctx;
}

void HiredisHelper::releaseConnection(redisContext* ctx, bool active)
{
    m_useNum--;
    if (ctx == NULL) return;
    if (!active) {
        redisFree(ctx);
        return;
    }
    m_ctxs.push(ctx);
}

//检查连接健康状态
bool HiredisHelper::checkStatus(redisContext* ctx)
{
    redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
    if(reply == NULL) return false;
    if(reply->type != REDIS_REPLY_STATUS) {
        CHECK_FREE_REDIS_REPLY(reply);
        return false;
    }
    if(strcasecmp(reply->str,"PONG") != 0) {
        CHECK_FREE_REDIS_REPLY(reply);
        return false;
    }
    return true;
}

void fn() {
    HiredisHelper helper;
    helper.init();
    int ret = helper.SetEx("xxx", "yyy", 100);
    cout << ret << endl;
}

void testPipeLine() {
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL) {
        printf("Can't allocate redis context\n");
        return;
    }
    if (c->err) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("redisConnect success!\n");

    redisReply *reply = NULL;
    redisAppendCommand(c,"set hello redis");
    redisAppendCommand(c,"GET hello");

    redisGetReply(c,(void**)(&reply));
    if(reply->type == REDIS_REPLY_STATUS) {
        printf("set hello redis\n");
    }
    freeReplyObject(reply);

    redisGetReply(c,(void**)&reply);
    if (reply->type == REDIS_REPLY_STRING) {
        printf("get %s\n", reply->str);
    }
    freeReplyObject(reply);

    redisFree(c);
}


void testPipeLine2() {
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL) {
        printf("Can't allocate redis context\n");
        return;
    }
    if (c->err) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("redisConnect success!\n");

    redisReply *reply = NULL;
    int ret = 0;
    ret = redisAppendCommand(c,"setex %b %d %b", "bbbb", 4, 100, "cccc", 4);
    printf("%redisAppendCommand ret:", ret);
    redisAppendCommand(c,"setex %b %d %b", "bbbb5554", 8, 100, "cccc", 4);


    ret = redisGetReply(c,(void**)(&reply));
    printf("%d\n", ret);
    if(reply->type == REDIS_REPLY_STATUS) {
        printf("set hello redis:%s\n", reply->str);
    }
    freeReplyObject(reply);

    ret = redisGetReply(c,(void**)&reply);
    printf("%d\n", ret);
    if (reply->type == REDIS_REPLY_STATUS) {
        printf("set %s\n", reply->str);
    }
    freeReplyObject(reply);


    redisFree(c);
}

int main(){
    //fn();
    testPipeLine2();
//    redisContext* connect;
//    redisReply* reply;
//    connect = redisConnect("127.0.0.1", 6379);
//    if (connect != NULL && connect->err){
//        printf("connect error:%s\n", connect->errstr);
//        exit(1);
//    }
//
//    reply = (redisReply*)redisCommand(connect, "smembers %s", "c");
//    printf("%s   %d %d\n", reply->str, reply->elements, reply->type);
//    //REDIS_REPLY_ERROR
//    printf("===========================\n");
//    freeReplyObject(reply);
//    char value[4] = {0x20, 0x20, 0x44, 0x55};
//    reply = (redisReply*)redisCommand(connect, "set b %b", value, (size_t)4);
//
//    printf("%s\n", reply->str);
//    freeReplyObject(reply);
//
//    reply = (redisReply*)redisCommand(connect, "get 1 %s", "c");
//    printf("%s   %d\n", reply->str, reply->type);
//
//    freeReplyObject(reply);
//    char* argv[2] = {"info", "info"};
//    size_t argvlen[2] = {5, 5};
//    reply = (redisReply*)redisCommandArgv(connect, 2, (const char**)argv, argvlen);
//    printf("%s   %d\n", reply->str, reply->type);
//    freeReplyObject(reply);
//
//    redisAppendCommand(connect, "SET foo 2");
//    redisAppendCommand(connect, "SET foo2 2");
//    redisAppendCommand(connect, "GET foo2");
//
//    printf("%d\n",redisGetReply(connect, (void**)&reply));
//    printf("reply size:%s\n", reply->str);
//    printf("%d\n",redisGetReply(connect, (void**)&reply));
//    printf("reply size:%s\n", reply->str);
//    printf("%d\n",redisGetReply(connect, (void**)&reply));
//    printf("reply size:%s\n", reply->str);
//    freeReplyObject(reply);
//    redisGetReply(connect, (void**)&reply);
//    freeReplyObject(reply);
    //redisFree(connect);

//    redisAsyncContext* c = redisAsyncConnect("127.0.0.1", 6379);
//    if (c->err){
//        printf("Error:%s\n", c->errstr);
//        exit(1);
//    }
////    redisLibevAttach(EV_DEFAULT_ c);
////    redisAsyncSetConnectCallback(c, connectCallback);
////    redisAsyncSetDisconnectCallback(c, disconnectCallback);
////    redisAsyncCommand(c, getCallback, (char*)"end-1", "GET a");
////    redisAsyncCommand(c, getCallback, (char*)"end-1", "GET b");
////    redisAsyncCommand(c, getCallback, (char*)"end-1", "GET c");
////    ev_loop(EV_DEFAULT_ 0);
////    sleep(3);
//
//    redisAsyncDisconnect(c);
}