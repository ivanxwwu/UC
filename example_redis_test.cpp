//
// Created by root on 17-9-17.
//

#include "hiredis/hiredis.h"
#include "stdlib.h"
#include "hiredis/async.h"
#include <unistd.h>
#include <hiredis/adapters/libev.h>
void disconnectCallback(const struct redisAsyncContext*, int status){
    printf("dis conn %s\n", "callback");
}

void connectCallback(const struct redisAsyncContext*, int status){
    printf("conn ballback\n");
}

void getCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = (redisReply*)r;
    if (reply == NULL) return;
    printf("argv[%s]: %s\n", (char*)privdata, reply->str);
    sleep(3);

    /* Disconnect after receiving the reply to GET */
}

int main(){
    redisContext* connect;
    redisReply* reply;
    connect = redisConnect("127.0.0.1", 6379);
    if (connect != NULL && connect->err){
        printf("connect error:%s\n", connect->errstr);
        exit(1);
    }

    reply = (redisReply*)redisCommand(connect, "get %s", "c");
    printf("%s   %d %d\n", reply->str, reply->len, reply->type);

    freeReplyObject(reply);
    char value[4] = {0x20, 0x20, 0x44, 0x55};
    reply = (redisReply*)redisCommand(connect, "set b %b", value, (size_t)4);

    printf("%s\n", reply->str);
    freeReplyObject(reply);

    reply = (redisReply*)redisCommand(connect, "get 1 %s", "c");
    printf("%s   %d\n", reply->str, reply->type);

    freeReplyObject(reply);
    char* argv[2] = {"info", "info"};
    size_t argvlen[2] = {5, 5};
    reply = (redisReply*)redisCommandArgv(connect, 2, (const char**)argv, argvlen);
    printf("%s   %d\n", reply->str, reply->type);
    freeReplyObject(reply);

    redisAppendCommand(connect, "SET foo 2");
    redisAppendCommand(connect, "SET foo2 2");
    redisAppendCommand(connect, "GET foo2");

    printf("%d\n",redisGetReply(connect, (void**)&reply));
    printf("reply size:%s\n", reply->str);
    printf("%d\n",redisGetReply(connect, (void**)&reply));
    printf("reply size:%s\n", reply->str);
    printf("%d\n",redisGetReply(connect, (void**)&reply));
    printf("reply size:%s\n", reply->str);
//    freeReplyObject(reply);
//    redisGetReply(connect, (void**)&reply);
//    freeReplyObject(reply);
    redisFree(connect);

    redisAsyncContext* c = redisAsyncConnect("127.0.0.1", 6379);
    if (c->err){
        printf("Error:%s\n", c->errstr);
        exit(1);
    }
    redisLibevAttach(EV_DEFAULT_ c);
    redisAsyncSetConnectCallback(c, connectCallback);
    redisAsyncSetDisconnectCallback(c, disconnectCallback);
    redisAsyncCommand(c, getCallback, (char*)"end-1", "GET a");
    redisAsyncCommand(c, getCallback, (char*)"end-1", "GET b");
    redisAsyncCommand(c, getCallback, (char*)"end-1", "GET c");
    ev_loop(EV_DEFAULT_ 0);
    sleep(3);

    redisAsyncDisconnect(c);
}