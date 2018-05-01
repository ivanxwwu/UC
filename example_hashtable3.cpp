//
// Created by root on 17-8-31.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define HASHSIZE 30000
#define MAXHASHVALUESIZE 1024
typedef struct _HashNode HashNode;
typedef struct _HashNode{
    char * key;
    char * value;
    HashNode* pNext;
} HashNode, *PHashNode;

PHashNode hashTable[HASHSIZE];
int hashTableSize;

int time33(const char * key, int size){
    unsigned long hash = 0;
    for (int i = 0; i<size; ++i){
        hash = hash*33 + (unsigned long)key[i];
    }
    return hash;
}

int hashFn(const char * key, int size){
    return time33(key, size)%HASHSIZE;
}

void hashInit(){
    hashTableSize = 0;
    memset(hashTable, 0, sizeof(PHashNode)*HASHSIZE);
}

void hashInsert(const char* key, const char * value){
    if (!key){
        return ;
    }
    int pos = hashFn(key, strlen(key));
    PHashNode pHashNode = hashTable[pos];
    while(pHashNode){
        if (strcmp(pHashNode->key, key)==0) {
            printf("%s existed", key);
            return;
        }
        pHashNode->pNext = pHashNode;
    }
    PHashNode pNewNode = (PHashNode)malloc(sizeof(HashNode));
    pNewNode->key = (char*)malloc(strlen(key));
    strcpy(pNewNode->key, key);
    pNewNode->value = (char*)malloc(strlen(value));
    strcpy(pNewNode->value, value);
    pNewNode->pNext = hashTable[pos];
    hashTable[pos] = pNewNode;
}

void hashDelete(const char* key){
    if (!key)
        return ;
    int pos = hashFn(key, strlen(key));
    PHashNode pHashNode = hashTable[pos];
    PHashNode pLast = NULL;
    while(pHashNode){
        if (strcmp(pHashNode->key, key)==0){
            if (pLast){
                pLast->pNext = pHashNode->pNext;
            }
            else{
                hashTable[pos] = pHashNode->pNext;
            }
            free(pHashNode->key);
            free(pHashNode->value);
            free(pHashNode);
        }
        pLast = pHashNode;
        pHashNode = pHashNode->pNext;
    }
}

int hashSearch(const char* key, char* value){
    if (!key)
        return -1;
    int pos = hashFn(key, strlen(key));
    PHashNode pHashNode = hashTable[pos];
    while(pHashNode){
        if (strcmp(key, pHashNode->key) == 0){
            //...check...
            strcpy(value, pHashNode->value);
            return pos;
        }
        pHashNode = pHashNode->pNext;
    }
    return -1;
}

void shmHashInit(){

}

int  main(){
    const char * path = ".";
    key_t key = ftok(path, 1);
    if (key == -1){
        perror("ftok fail");
        exit(1);
    }
    int shmid = shmget(key, sizeof(PHashNode)*HASHSIZE, IPC_CREAT|IPC_EXCL|0600);
    if (shmid< 0){
        perror("shget fail!\n");
        exit(1);
    }
    PHashNode  pHashNode = (PHashNode )shmat(shmid, NULL, 0);
    
}
