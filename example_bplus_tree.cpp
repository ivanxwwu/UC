//
// Created by root on 19-9-13.
//




#define NULL 0

enum NODE_TYPE{INTERNAL, LEAF};        // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION{LEFT, RIGHT};   // 兄弟结点方向：左兄弟结点、右兄弟结点
typedef float KeyType;                 // 键类型
typedef int DataType;                  // 值类型
const int ORDER = 7;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER-1;        // 最小键值个数
const int MAXNUM_KEY = 2*ORDER-1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY+1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY;    // 最大叶子结点键值个数

// 结点基类
class CNode{
public:
    CNode();
    virtual ~CNode();

    NODE_TYPE getType() const {return m_Type;}
    void setType(NODE_TYPE type){m_Type = type;}
    int getKeyNum() const {return m_KeyNum;}
    void setKeyNum(int n){m_KeyNum = n;}
    KeyType getKeyValue(int i) const {return m_KeyValues[i];}
    void setKeyValue(int i, KeyType key){m_KeyValues[i] = key;}
    int getKeyIndex(KeyType key)const;  // 找到键值在结点中存储的下标
    // 纯虚函数，定义接口
    virtual void removeKey(int keyIndex, int childIndex)=0;  // 从结点中移除键值
    virtual void split(CNode* parentNode, int childIndex)=0; // 分裂结点
    virtual void mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)=0;  // 合并结点
    virtual void clear()=0; // 清空结点，同时会清空结点所包含的子树结点
    virtual void borrowFrom(CNode* destNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)=0; // 从兄弟结点中借一个键值
    virtual int getChildIndex(KeyType key, int keyIndex)const=0;  // 根据键值获取孩子结点指针下标
protected:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyType m_KeyValues[MAXNUM_KEY];
};

// 内结点
class CInternalNode : public CNode{
public:
    CInternalNode();
    virtual ~CInternalNode();

    CNode* getChild(int i) const {return m_Childs[i];}
    void setChild(int i, CNode* child){m_Childs[i] = child;}
    void insert(int keyIndex, int childIndex, KeyType key, CNode* childNode);
    virtual void split(CNode* parentNode, int childIndex);
    virtual void mergeChild(CNode* parentNode, CNode* childNode, int keyIndex);
    virtual void removeKey(int keyIndex, int childIndex);
    virtual void clear();
    virtual void borrowFrom(CNode* destNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d);
    virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
    CNode* m_Childs[MAXNUM_CHILD];
};

// 叶子结点
class CLeafNode : public CNode{
public:
    CLeafNode();
    virtual ~CLeafNode();

    CLeafNode* getLeftSibling() const {return m_LeftSibling;}
    void setLeftSibling(CLeafNode* node){m_LeftSibling = node;}
    CLeafNode* getRightSibling() const {return m_RightSibling;}
    void setRightSibling(CLeafNode* node){m_RightSibling = node;}
    DataType getData(int i) const {return m_Datas[i];}
    void setData(int i, const DataType& data){m_Datas[i] = data;}
    void insert(KeyType key, const DataType& data);
    virtual void split(CNode* parentNode, int childIndex);
    virtual void mergeChild(CNode* parentNode, CNode* childNode, int keyIndex);
    virtual void removeKey(int keyIndex, int childIndex);
    virtual void clear();
    virtual void borrowFrom(CNode* destNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d);
    virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
    CLeafNode* m_LeftSibling;
    CLeafNode* m_RightSibling;
    DataType m_Datas[MAXNUM_LEAF];
};



CNode::CNode(){
    setType(LEAF);
    setKeyNum(0);
}

CNode::~CNode(){
    setKeyNum(0);
}

int CNode::getKeyIndex(KeyType key)const
{
    int left = 0;
    int right = getKeyNum()-1;
    int current;
    while(left!=right)
    {
        current = (left+right)/2;
        KeyType currentKey = getKeyValue(current);
        if (key>currentKey)
        {
            left = current+1;
        }
        else
        {
            right = current;
        }
    }
    return left;
}

// CInternalNode
CInternalNode::CInternalNode():CNode(){
    setType(INTERNAL);
}

CInternalNode::~CInternalNode(){

}

void CInternalNode::clear()
{
    for (int i=0; i<=m_KeyNum; ++i)
    {
        m_Childs[i]->clear();
        delete m_Childs[i];
        m_Childs[i] = NULL;
    }
}

void CInternalNode::split(CNode* parentNode, int childIndex)
{
    CInternalNode* newNode = new CInternalNode();//分裂后的右节点
    newNode->setKeyNum(MINNUM_KEY);
    int i;
    for (i=0; i<MINNUM_KEY; ++i)// 拷贝关键字的值
    {
        newNode->setKeyValue(i, m_KeyValues[i+MINNUM_CHILD]);
    }
    for (i=0; i<MINNUM_CHILD; ++i) // 拷贝孩子节点指针
    {
        newNode->setChild(i, m_Childs[i+MINNUM_CHILD]);
    }
    setKeyNum(MINNUM_KEY);  //更新左子树的关键字个数
    ((CInternalNode*)parentNode)->insert(childIndex, childIndex+1, m_KeyValues[MINNUM_KEY], newNode);
}

void CInternalNode::insert(int keyIndex, int childIndex, KeyType key, CNode* childNode){
    int i;
    for (i=getKeyNum(); i>keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
    {
        setChild(i+1,m_Childs[i]);
        setKeyValue(i,m_KeyValues[i-1]);
    }
    if (i==childIndex)
    {
        setChild(i+1, m_Childs[i]);
    }
    setChild(childIndex, childNode);
    setKeyValue(keyIndex, key);
    setKeyNum(m_KeyNum+1);
}

void CInternalNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
    // 合并数据
    insert(MINNUM_KEY, MINNUM_KEY+1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)childNode)->getChild(0));
    int i;
    for (i=1; i<=childNode->getKeyNum(); ++i)
    {
        insert(MINNUM_KEY+i, MINNUM_KEY+i+1, childNode->getKeyValue(i-1), ((CInternalNode*)childNode)->getChild(i));
    }
    //父节点删除index的key
    parentNode->removeKey(keyIndex, keyIndex+1);
    delete ((CInternalNode*)parentNode)->getChild(keyIndex+1);
}

void CInternalNode::removeKey(int keyIndex, int childIndex)
{
    for (int i=0; i<getKeyNum()-keyIndex-1; ++i)
    {
        setKeyValue(keyIndex+i, getKeyValue(keyIndex+i+1));
        setChild(childIndex+i, getChild(childIndex+i+1));
    }
    setKeyNum(getKeyNum()-1);
}

void CInternalNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
    switch(d)
    {
        case LEFT:  // 从左兄弟结点借
        {
            insert(0, 0, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum()-1));
            siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum());
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(getKeyNum(), getKeyNum()+1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(0));
            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
            siblingNode->removeKey(0, 0);
        }
            break;
        default:
            break;
    }
}

int CInternalNode::getChildIndex(KeyType key, int keyIndex)const
{
    if (key==getKeyValue(keyIndex))
    {
        return keyIndex+1;
    }
    else
    {
        return keyIndex;
    }
}

// CLeafNode
CLeafNode::CLeafNode():CNode(){
    setType(LEAF);
    setLeftSibling(NULL);
    setRightSibling(NULL);
}

CLeafNode::~CLeafNode(){

}

void CLeafNode::clear()
{
    for (int i=0; i<m_KeyNum; ++i)
    {
        // if type of m_Datas is pointer
        //delete m_Datas[i];
        //m_Datas[i] = NULL;
    }
}

void CLeafNode::insert(KeyType key, const DataType& data)
{
    int i;
    for (i=m_KeyNum; i>=1 && m_KeyValues[i-1]>key; --i)
    {
        setKeyValue(i, m_KeyValues[i-1]);
        setData(i, m_Datas[i-1]);
    }
    setKeyValue(i, key);
    setData(i, data);
    setKeyNum(m_KeyNum+1);
}

void CLeafNode::split(CNode* parentNode, int childIndex)
{
    CLeafNode* newNode = new CLeafNode();//分裂后的右节点
    setKeyNum(MINNUM_LEAF);
    newNode->setKeyNum(MINNUM_LEAF+1);
    newNode->setRightSibling(getRightSibling());
    setRightSibling(newNode);
    newNode->setLeftSibling(this);
    int i;
    for (i=0; i<MINNUM_LEAF+1; ++i)// 拷贝关键字的值
    {
        newNode->setKeyValue(i, m_KeyValues[i+MINNUM_LEAF]);
    }
    for (i=0; i<MINNUM_LEAF+1; ++i)// 拷贝数据
    {
        newNode->setData(i, m_Datas[i+MINNUM_LEAF]);
    }
    ((CInternalNode*)parentNode)->insert(childIndex, childIndex+1, m_KeyValues[MINNUM_LEAF], newNode);
}

void CLeafNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
    // 合并数据
    for (int i=0; i<childNode->getKeyNum(); ++i)
    {
        insert(childNode->getKeyValue(i), ((CLeafNode*)childNode)->getData(i));
    }
    setRightSibling(((CLeafNode*)childNode)->getRightSibling());
    //父节点删除index的key，
    parentNode->removeKey(keyIndex, keyIndex+1);
}

void CLeafNode::removeKey(int keyIndex, int childIndex)
{
    for (int i=keyIndex; i<getKeyNum()-1; ++i)
    {
        setKeyValue(i, getKeyValue(i+1));
        setData(i, getData(i+1));
    }
    setKeyNum(getKeyNum()-1);
}

void CLeafNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
    switch(d)
    {
        case LEFT:  // 从左兄弟结点借
        {
            insert(siblingNode->getKeyValue(siblingNode->getKeyNum()-1), ((CLeafNode*)siblingNode)->getData(siblingNode->getKeyNum()-1));
            siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum()-1);
            parentNode->setKeyValue(keyIndex, getKeyValue(0));
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(siblingNode->getKeyValue(0), ((CLeafNode*)siblingNode)->getData(0));
            siblingNode->removeKey(0, 0);
            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
        }
            break;
        default:
            break;
    }
}

int CLeafNode::getChildIndex(KeyType key, int keyIndex)const
{
    return keyIndex;
}

#include "BPlus_node.h"

// CNode
CNode::CNode(){
    setType(LEAF);
    setKeyNum(0);
}

CNode::~CNode(){
    setKeyNum(0);
}

int CNode::getKeyIndex(KeyType key)const
{
    int left = 0;
    int right = getKeyNum()-1;
    int current;
    while(left!=right)
    {
        current = (left+right)/2;
        KeyType currentKey = getKeyValue(current);
        if (key>currentKey)
        {
            left = current+1;
        }
        else
        {
            right = current;
        }
    }
    return left;
}

// CInternalNode
CInternalNode::CInternalNode():CNode(){
    setType(INTERNAL);
}

CInternalNode::~CInternalNode(){

}

void CInternalNode::clear()
{
    for (int i=0; i<=m_KeyNum; ++i)
    {
        m_Childs[i]->clear();
        delete m_Childs[i];
        m_Childs[i] = NULL;
    }
}

void CInternalNode::split(CNode* parentNode, int childIndex)
{
    CInternalNode* newNode = new CInternalNode();//分裂后的右节点
    newNode->setKeyNum(MINNUM_KEY);
    int i;
    for (i=0; i<MINNUM_KEY; ++i)// 拷贝关键字的值
    {
        newNode->setKeyValue(i, m_KeyValues[i+MINNUM_CHILD]);
    }
    for (i=0; i<MINNUM_CHILD; ++i) // 拷贝孩子节点指针
    {
        newNode->setChild(i, m_Childs[i+MINNUM_CHILD]);
    }
    setKeyNum(MINNUM_KEY);  //更新左子树的关键字个数
    ((CInternalNode*)parentNode)->insert(childIndex, childIndex+1, m_KeyValues[MINNUM_KEY], newNode);
}

void CInternalNode::insert(int keyIndex, int childIndex, KeyType key, CNode* childNode){
    int i;
    for (i=getKeyNum(); i>keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
    {
        setChild(i+1,m_Childs[i]);
        setKeyValue(i,m_KeyValues[i-1]);
    }
    if (i==childIndex)
    {
        setChild(i+1, m_Childs[i]);
    }
    setChild(childIndex, childNode);
    setKeyValue(keyIndex, key);
    setKeyNum(m_KeyNum+1);
}

void CInternalNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
    // 合并数据
    insert(MINNUM_KEY, MINNUM_KEY+1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)childNode)->getChild(0));
    int i;
    for (i=1; i<=childNode->getKeyNum(); ++i)
    {
        insert(MINNUM_KEY+i, MINNUM_KEY+i+1, childNode->getKeyValue(i-1), ((CInternalNode*)childNode)->getChild(i));
    }
    //父节点删除index的key
    parentNode->removeKey(keyIndex, keyIndex+1);
    delete ((CInternalNode*)parentNode)->getChild(keyIndex+1);
}

void CInternalNode::removeKey(int keyIndex, int childIndex)
{
    for (int i=0; i<getKeyNum()-keyIndex-1; ++i)
    {
        setKeyValue(keyIndex+i, getKeyValue(keyIndex+i+1));
        setChild(childIndex+i, getChild(childIndex+i+1));
    }
    setKeyNum(getKeyNum()-1);
}

void CInternalNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
    switch(d)
    {
        case LEFT:  // 从左兄弟结点借
        {
            insert(0, 0, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum()-1));
            siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum());
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(getKeyNum(), getKeyNum()+1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(0));
            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
            siblingNode->removeKey(0, 0);
        }
            break;
        default:
            break;
    }
}

int CInternalNode::getChildIndex(KeyType key, int keyIndex)const
{
    if (key==getKeyValue(keyIndex))
    {
        return keyIndex+1;
    }
    else
    {
        return keyIndex;
    }
}

// CLeafNode
CLeafNode::CLeafNode():CNode(){
    setType(LEAF);
    setLeftSibling(NULL);
    setRightSibling(NULL);
}

CLeafNode::~CLeafNode(){

}

void CLeafNode::clear()
{
    for (int i=0; i<m_KeyNum; ++i)
    {
        // if type of m_Datas is pointer
        //delete m_Datas[i];
        //m_Datas[i] = NULL;
    }
}

void CLeafNode::insert(KeyType key, const DataType& data)
{
    int i;
    for (i=m_KeyNum; i>=1 && m_KeyValues[i-1]>key; --i)
    {
        setKeyValue(i, m_KeyValues[i-1]);
        setData(i, m_Datas[i-1]);
    }
    setKeyValue(i, key);
    setData(i, data);
    setKeyNum(m_KeyNum+1);
}

void CLeafNode::split(CNode* parentNode, int childIndex)
{
    CLeafNode* newNode = new CLeafNode();//分裂后的右节点
    setKeyNum(MINNUM_LEAF);
    newNode->setKeyNum(MINNUM_LEAF+1);
    newNode->setRightSibling(getRightSibling());
    setRightSibling(newNode);
    newNode->setLeftSibling(this);
    int i;
    for (i=0; i<MINNUM_LEAF+1; ++i)// 拷贝关键字的值
    {
        newNode->setKeyValue(i, m_KeyValues[i+MINNUM_LEAF]);
    }
    for (i=0; i<MINNUM_LEAF+1; ++i)// 拷贝数据
    {
        newNode->setData(i, m_Datas[i+MINNUM_LEAF]);
    }
    ((CInternalNode*)parentNode)->insert(childIndex, childIndex+1, m_KeyValues[MINNUM_LEAF], newNode);
}

void CLeafNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
    // 合并数据
    for (int i=0; i<childNode->getKeyNum(); ++i)
    {
        insert(childNode->getKeyValue(i), ((CLeafNode*)childNode)->getData(i));
    }
    setRightSibling(((CLeafNode*)childNode)->getRightSibling());
    //父节点删除index的key，
    parentNode->removeKey(keyIndex, keyIndex+1);
}

void CLeafNode::removeKey(int keyIndex, int childIndex)
{
    for (int i=keyIndex; i<getKeyNum()-1; ++i)
    {
        setKeyValue(i, getKeyValue(i+1));
        setData(i, getData(i+1));
    }
    setKeyNum(getKeyNum()-1);
}

void CLeafNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
    switch(d)
    {
        case LEFT:  // 从左兄弟结点借
        {
            insert(siblingNode->getKeyValue(siblingNode->getKeyNum()-1), ((CLeafNode*)siblingNode)->getData(siblingNode->getKeyNum()-1));
            siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum()-1);
            parentNode->setKeyValue(keyIndex, getKeyValue(0));
        }
            break;
        case RIGHT:  // 从右兄弟结点借
        {
            insert(siblingNode->getKeyValue(0), ((CLeafNode*)siblingNode)->getData(0));
            siblingNode->removeKey(0, 0);
            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
        }
            break;
        default:
            break;
    }
}

int CLeafNode::getChildIndex(KeyType key, int keyIndex)const
{
    return keyIndex;
}

#include <vector>
using namespace std;

enum COMPARE_OPERATOR{LT, LE, EQ, BE, BT, BETWEEN}; // 比较操作符：<、<=、=、>=、>、<>
const int INVALID_INDEX = -1;

struct SelectResult
{
    int keyIndex;
    CLeafNode* targetNode;
};

class CBPlusTree{
public:
    CBPlusTree();
    ~CBPlusTree();
    bool insert(KeyType key, const DataType& data);
    bool remove(KeyType key);
    bool update(KeyType oldKey, KeyType newKey);
    // 定值查询，compareOperator可以是LT(<)、LE(<=)、EQ(=)、BE(>=)、BT(>)
    vector<DataType> select(KeyType compareKey, int compareOpeartor);
    // 范围查询，BETWEEN
    vector<DataType> select(KeyType smallKey, KeyType largeKey);
    bool search(KeyType key); // 查找是否存在
    void clear();             // 清空
    void print()const;        // 打印树关键字
    void printData()const;    // 打印数据
private:
    void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
    void recursive_remove(CNode* parentNode, KeyType key);
    void printInConcavo(CNode *pNode, int count)const;
    bool recursive_search(CNode *pNode, KeyType key)const;
    void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey);
    void search(KeyType key, SelectResult& result);
    void recursive_search(CNode* pNode, KeyType key, SelectResult& result);
    void remove(KeyType key, DataType& dataValue);
    void recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue);
private:
    CNode* m_Root;
    CLeafNode* m_DataHead;
    KeyType m_MaxKey;  // B+树中的最大键
};

#endif

#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include "BPlus_node.h"
#include <vector>
using namespace std;

enum COMPARE_OPERATOR{LT, LE, EQ, BE, BT, BETWEEN}; // 比较操作符：<、<=、=、>=、>、<>
const int INVALID_INDEX = -1;

struct SelectResult
{
 int keyIndex;
 CLeafNode* targetNode;
};

class CBPlusTree{
public:
 CBPlusTree();
 ~CBPlusTree();
 bool insert(KeyType key, const DataType& data);
 bool remove(KeyType key);
 bool update(KeyType oldKey, KeyType newKey);
 // 定值查询，compareOperator可以是LT(<)、LE(<=)、EQ(=)、BE(>=)、BT(>)
 vector<DataType> select(KeyType compareKey, int compareOpeartor);
 // 范围查询，BETWEEN
 vector<DataType> select(KeyType smallKey, KeyType largeKey);
 bool search(KeyType key); // 查找是否存在
 void clear();             // 清空
 void print()const;        // 打印树关键字
 void printData()const;    // 打印数据
private:
 void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
 void recursive_remove(CNode* parentNode, KeyType key);
 void printInConcavo(CNode *pNode, int count)const;
 bool recursive_search(CNode *pNode, KeyType key)const;
 void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey);
 void search(KeyType key, SelectResult& result);
 void recursive_search(CNode* pNode, KeyType key, SelectResult& result);
 void remove(KeyType key, DataType& dataValue);
 void recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue);
private:
 CNode* m_Root;
 CLeafNode* m_DataHead;
 KeyType m_MaxKey;  // B+树中的最大键
};

#endif

#include <iostream>
#include <algorithm>
using namespace std;

CBPlusTree::CBPlusTree(){
    m_Root = NULL;
    m_DataHead = NULL;
}

CBPlusTree::~CBPlusTree(){
    clear();
}

bool CBPlusTree::insert(KeyType key, const DataType& data){
    // 是否已经存在
    if (search(key))
    {
        return false;
    }
    // 找到可以插入的叶子结点，否则创建新的叶子结点
    if(m_Root==NULL)
    {
        m_Root = new CLeafNode();
        m_DataHead = (CLeafNode*)m_Root;
        m_MaxKey = key;
    }
    if (m_Root->getKeyNum()>=MAXNUM_KEY) // 根结点已满，分裂
    {
        CInternalNode* newNode = new CInternalNode();  //创建新的根节点
        newNode->setChild(0, m_Root);
        m_Root->split(newNode, 0);    // 叶子结点分裂
        m_Root = newNode;  //更新根节点指针
    }
    if (key>m_MaxKey)  // 更新最大键值
    {
        m_MaxKey = key;
    }
    recursive_insert(m_Root, key, data);
    return true;
}

void CBPlusTree::recursive_insert(CNode* parentNode, KeyType key, const DataType& data)
{
    if (parentNode->getType()==LEAF)  // 叶子结点，直接插入
    {
        ((CLeafNode*)parentNode)->insert(key, data);
    }
    else
    {
        // 找到子结点
        int keyIndex = parentNode->getKeyIndex(key);
        int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
        CNode* childNode = ((CInternalNode*)parentNode)->getChild(childIndex);
        if (childNode->getKeyNum()>=MAXNUM_LEAF)  // 子结点已满，需进行分裂
        {
            childNode->split(parentNode, childIndex);
            if (parentNode->getKeyValue(childIndex)<=key)   // 确定目标子结点
            {
                childNode = ((CInternalNode*)parentNode)->getChild(childIndex+1);
            }
        }
        recursive_insert(childNode, key, data);
    }
}

void CBPlusTree::clear()
{
    if (m_Root!=NULL)
    {
        m_Root->clear();
        delete m_Root;
        m_Root = NULL;
        m_DataHead = NULL;
    }
}

bool CBPlusTree::search(KeyType key)
{
    return recursive_search(m_Root, key);
}

bool CBPlusTree::recursive_search(CNode *pNode, KeyType key)const
{
    if (pNode==NULL)  //检测节点指针是否为空，或该节点是否为叶子节点
    {
        return false;
    }
    else
    {
        int keyIndex = pNode->getKeyIndex(key);
        int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
        if (keyIndex<pNode->getKeyNum() && key==pNode->getKeyValue(keyIndex))
        {
            return true;
        }
        else
        {
            if (pNode->getType()==LEAF)   //检查该节点是否为叶子节点
            {
                return false;
            }
            else
            {
                return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key);
            }
        }
    }
}

void CBPlusTree::print()const
{
    printInConcavo(m_Root, 10);
}

void CBPlusTree::printInConcavo(CNode *pNode, int count) const{
    if (pNode!=NULL)
    {
        int i, j;
        for (i=0; i<pNode->getKeyNum(); ++i)
        {
            if (pNode->getType()!=LEAF)
            {
                printInConcavo(((CInternalNode*)pNode)->getChild(i), count-2);
            }
            for (j=count; j>=0; --j)
            {
                cout<<"-";
            }
            cout<<pNode->getKeyValue(i)<<endl;
        }
        if (pNode->getType()!=LEAF)
        {
            printInConcavo(((CInternalNode*)pNode)->getChild(i), count-2);
        }
    }
}

void CBPlusTree::printData()const
{
    CLeafNode* itr = m_DataHead;
    while(itr!=NULL)
    {
        for (int i=0; i<itr->getKeyNum(); ++i)
        {
            cout<<itr->getData(i)<<" ";
        }
        cout<<endl;
        itr = itr->getRightSibling();
    }
}

bool CBPlusTree::remove(KeyType key)
{
    if (!search(key))  //不存在
    {
        return false;
    }
    if (m_Root->getKeyNum()==1)//特殊情况处理
    {
        if (m_Root->getType()==LEAF)
        {
            clear();
            return true;
        }
        else
        {
            CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
            CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
            if (pChild1->getKeyNum()==MINNUM_KEY && pChild2->getKeyNum()==MINNUM_KEY)
            {
                pChild1->mergeChild(m_Root, pChild2, 0);
                delete m_Root;
                m_Root = pChild1;
            }
        }
    }
    recursive_remove(m_Root, key);
    return true;
}

// parentNode中包含的键值数>MINNUM_KEY
void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key)
{
    int keyIndex = parentNode->getKeyIndex(key);
    int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (parentNode->getType()==LEAF)// 找到目标叶子节点
    {
        if (key==m_MaxKey&&keyIndex>0)
        {
            m_MaxKey = parentNode->getKeyValue(keyIndex-1);
        }
        parentNode->removeKey(keyIndex, childIndex);  // 直接删除
        // 如果键值在内部结点中存在，也要相应的替换内部结点
        if (childIndex==0 && m_Root->getType()!=LEAF && parentNode!=m_DataHead)
        {
            changeKey(m_Root, key, parentNode->getKeyValue(0));
        }
    }
    else // 内结点
    {
        CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex); //包含key的子树根节点
        if (pChildNode->getKeyNum()==MINNUM_KEY)                       // 包含关键字达到下限值，进行相关操作
        {
            CNode *pLeft = childIndex>0 ? ((CInternalNode*)parentNode)->getChild(childIndex-1) : NULL;                       //左兄弟节点
            CNode *pRight = childIndex<parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex+1) : NULL;//右兄弟节点
            // 先考虑从兄弟结点中借
            if (pLeft && pLeft->getKeyNum()>MINNUM_KEY)// 左兄弟结点可借
            {
                pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
            }
            else if (pRight && pRight->getKeyNum()>MINNUM_KEY)//右兄弟结点可借
            {
                pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
            }
                //左右兄弟节点都不可借，考虑合并
            else if (pLeft)                    //与左兄弟合并
            {
                pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
                pChildNode = pLeft;
            }
            else if (pRight)                   //与右兄弟合并
            {
                pChildNode->mergeChild(parentNode, pRight, childIndex);
            }
        }
        recursive_remove(pChildNode, key);
    }
}

void CBPlusTree::changeKey(CNode *pNode, KeyType oldKey, KeyType newKey)
{
    if (pNode!=NULL && pNode->getType()!=LEAF)
    {
        int keyIndex = pNode->getKeyIndex(oldKey);
        if (keyIndex<pNode->getKeyNum() && oldKey==pNode->getKeyValue(keyIndex))  // 找到
        {
            pNode->setKeyValue(keyIndex, newKey);
        }
        else   // 继续找
        {
            changeKey(((CInternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
        }
    }
}

bool CBPlusTree::update(KeyType oldKey, KeyType newKey)
{
    if (search(newKey)) // 检查更新后的键是否已经存在
    {
        return false;
    }
    else
    {
        int dataValue;
        remove(oldKey, dataValue);
        if (dataValue==INVALID_INDEX)
        {
            return false;
        }
        else
        {
            return insert(newKey, dataValue);
        }
    }
}

void CBPlusTree::remove(KeyType key, DataType& dataValue)
{
    if (!search(key))  //不存在
    {
        dataValue = INVALID_INDEX;
        return;
    }
    if (m_Root->getKeyNum()==1)//特殊情况处理
    {
        if (m_Root->getType()==LEAF)
        {
            dataValue = ((CLeafNode*)m_Root)->getData(0);
            clear();
            return;
        }
        else
        {
            CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
            CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
            if (pChild1->getKeyNum()==MINNUM_KEY && pChild2->getKeyNum()==MINNUM_KEY)
            {
                pChild1->mergeChild(m_Root, pChild2, 0);
                delete m_Root;
                m_Root = pChild1;
            }
        }
    }
    recursive_remove(m_Root, key, dataValue);
}

void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue)
{
    int keyIndex = parentNode->getKeyIndex(key);
    int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (parentNode->getType()==LEAF)// 找到目标叶子节点
    {
        if (key==m_MaxKey&&keyIndex>0)
        {
            m_MaxKey = parentNode->getKeyValue(keyIndex-1);
        }
        dataValue = ((CLeafNode*)parentNode)->getData(keyIndex);
        parentNode->removeKey(keyIndex, childIndex);  // 直接删除
        // 如果键值在内部结点中存在，也要相应的替换内部结点
        if (childIndex==0 && m_Root->getType()!=LEAF && parentNode!=m_DataHead)
        {
            changeKey(m_Root, key, parentNode->getKeyValue(0));
        }
    }
    else // 内结点
    {
        CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex); //包含key的子树根节点
        if (pChildNode->getKeyNum()==MINNUM_KEY)                       // 包含关键字达到下限值，进行相关操作
        {
            CNode *pLeft = childIndex>0 ? ((CInternalNode*)parentNode)->getChild(childIndex-1) : NULL;                       //左兄弟节点
            CNode *pRight = childIndex<parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex+1) : NULL;//右兄弟节点
            // 先考虑从兄弟结点中借
            if (pLeft && pLeft->getKeyNum()>MINNUM_KEY)// 左兄弟结点可借
            {
                pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
            }
            else if (pRight && pRight->getKeyNum()>MINNUM_KEY)//右兄弟结点可借
            {
                pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
            }
                //左右兄弟节点都不可借，考虑合并
            else if (pLeft)                    //与左兄弟合并
            {
                pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
                pChildNode = pLeft;
            }
            else if (pRight)                   //与右兄弟合并
            {
                pChildNode->mergeChild(parentNode, pRight, childIndex);
            }
        }
        recursive_remove(pChildNode, key, dataValue);
    }
}

vector<DataType> CBPlusTree::select(KeyType compareKey, int compareOpeartor)
{
    vector<DataType> results;
    if (m_Root!=NULL)
    {
        if (compareKey>m_MaxKey)   // 比较键值大于B+树中最大的键值
        {
            if (compareOpeartor==LE || compareOpeartor==LT)
            {
                for(CLeafNode* itr = m_DataHead; itr!=NULL; itr= itr->getRightSibling())
                {
                    for (int i=0; i<itr->getKeyNum(); ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                }
            }
        }
        else if (compareKey<m_DataHead->getKeyValue(0))  // 比较键值小于B+树中最小的键值
        {
            if (compareOpeartor==BE || compareOpeartor==BT)
            {
                for(CLeafNode* itr = m_DataHead; itr!=NULL; itr= itr->getRightSibling())
                {
                    for (int i=0; i<itr->getKeyNum(); ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                }
            }
        }
        else  // 比较键值在B+树中
        {
            SelectResult result;
            search(compareKey, result);
            switch(compareOpeartor)
            {
                case LT:
                case LE:
                {
                    CLeafNode* itr = m_DataHead;
                    int i;
                    while (itr!=result.targetNode)
                    {
                        for (i=0; i<itr->getKeyNum(); ++i)
                        {
                            results.push_back(itr->getData(i));
                        }
                        itr = itr->getRightSibling();
                    }
                    for (i=0; i<result.keyIndex; ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                    if (itr->getKeyValue(i)<compareKey ||
                        (compareOpeartor==LE && compareKey==itr->getKeyValue(i)))
                    {
                        results.push_back(itr->getData(i));
                    }
                }
                    break;
                case EQ:
                {
                    if (result.targetNode->getKeyValue(result.keyIndex)==compareKey)
                    {
                        results.push_back(result.targetNode->getData(result.keyIndex));
                    }
                }
                    break;
                case BE:
                case BT:
                {
                    CLeafNode* itr = result.targetNode;
                    if (compareKey<itr->getKeyValue(result.keyIndex) ||
                        (compareOpeartor==BE && compareKey==itr->getKeyValue(result.keyIndex))
                            )
                    {
                        results.push_back(itr->getData(result.keyIndex));
                    }
                    int i;
                    for (i=result.keyIndex+1; i<itr->getKeyNum(); ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                    itr = itr->getRightSibling();
                    while (itr!=NULL)
                    {
                        for (i=0; i<itr->getKeyNum(); ++i)
                        {
                            results.push_back(itr->getData(i));
                        }
                        itr = itr->getRightSibling();
                    }
                }
                    break;
                default:  // 范围查询
                    break;
            }
        }
    }
    sort<vector<DataType>::iterator>(results.begin(), results.end());
    return results;
}

vector<DataType> CBPlusTree::select(KeyType smallKey, KeyType largeKey)
{
    vector<DataType> results;
    if (smallKey<=largeKey)
    {
        SelectResult start, end;
        search(smallKey, start);
        search(largeKey, end);
        CLeafNode* itr = start.targetNode;
        int i = start.keyIndex;
        if (itr->getKeyValue(i)<smallKey)
        {
            ++i;
        }
        if (end.targetNode->getKeyValue(end.keyIndex)>largeKey)
        {
            --end.keyIndex;
        }
        while (itr!=end.targetNode)
        {
            for (; i<itr->getKeyNum(); ++i)
            {
                results.push_back(itr->getData(i));
            }
            itr = itr->getRightSibling();
            i = 0;
        }
        for (; i<=end.keyIndex; ++i)
        {
            results.push_back(itr->getData(i));
        }
    }
    sort<vector<DataType>::iterator>(results.begin(), results.end());
    return results;
}

void CBPlusTree::search(KeyType key, SelectResult& result)
{
    recursive_search(m_Root, key, result);
}

void CBPlusTree::recursive_search(CNode* pNode, KeyType key, SelectResult& result)
{
    int keyIndex = pNode->getKeyIndex(key);
    int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (pNode->getType()==LEAF)
    {
        result.keyIndex = keyIndex;
        result.targetNode = (CLeafNode*)pNode;
        return;
    }
    else
    {
        return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key, result);
    }
}

#include "BPlus_tree.h"
#include <iostream>
#include <algorithm>
using namespace std;

CBPlusTree::CBPlusTree(){
    m_Root = NULL;
    m_DataHead = NULL;
}

CBPlusTree::~CBPlusTree(){
    clear();
}

bool CBPlusTree::insert(KeyType key, const DataType& data){
    // 是否已经存在
    if (search(key))
    {
        return false;
    }
    // 找到可以插入的叶子结点，否则创建新的叶子结点
    if(m_Root==NULL)
    {
        m_Root = new CLeafNode();
        m_DataHead = (CLeafNode*)m_Root;
        m_MaxKey = key;
    }
    if (m_Root->getKeyNum()>=MAXNUM_KEY) // 根结点已满，分裂
    {
        CInternalNode* newNode = new CInternalNode();  //创建新的根节点
        newNode->setChild(0, m_Root);
        m_Root->split(newNode, 0);    // 叶子结点分裂
        m_Root = newNode;  //更新根节点指针
    }
    if (key>m_MaxKey)  // 更新最大键值
    {
        m_MaxKey = key;
    }
    recursive_insert(m_Root, key, data);
    return true;
}

void CBPlusTree::recursive_insert(CNode* parentNode, KeyType key, const DataType& data)
{
    if (parentNode->getType()==LEAF)  // 叶子结点，直接插入
    {
        ((CLeafNode*)parentNode)->insert(key, data);
    }
    else
    {
        // 找到子结点
        int keyIndex = parentNode->getKeyIndex(key);
        int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
        CNode* childNode = ((CInternalNode*)parentNode)->getChild(childIndex);
        if (childNode->getKeyNum()>=MAXNUM_LEAF)  // 子结点已满，需进行分裂
        {
            childNode->split(parentNode, childIndex);
            if (parentNode->getKeyValue(childIndex)<=key)   // 确定目标子结点
            {
                childNode = ((CInternalNode*)parentNode)->getChild(childIndex+1);
            }
        }
        recursive_insert(childNode, key, data);
    }
}

void CBPlusTree::clear()
{
    if (m_Root!=NULL)
    {
        m_Root->clear();
        delete m_Root;
        m_Root = NULL;
        m_DataHead = NULL;
    }
}

bool CBPlusTree::search(KeyType key)
{
    return recursive_search(m_Root, key);
}

bool CBPlusTree::recursive_search(CNode *pNode, KeyType key)const
{
    if (pNode==NULL)  //检测节点指针是否为空，或该节点是否为叶子节点
    {
        return false;
    }
    else
    {
        int keyIndex = pNode->getKeyIndex(key);
        int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
        if (keyIndex<pNode->getKeyNum() && key==pNode->getKeyValue(keyIndex))
        {
            return true;
        }
        else
        {
            if (pNode->getType()==LEAF)   //检查该节点是否为叶子节点
            {
                return false;
            }
            else
            {
                return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key);
            }
        }
    }
}

void CBPlusTree::print()const
{
    printInConcavo(m_Root, 10);
}

void CBPlusTree::printInConcavo(CNode *pNode, int count) const{
    if (pNode!=NULL)
    {
        int i, j;
        for (i=0; i<pNode->getKeyNum(); ++i)
        {
            if (pNode->getType()!=LEAF)
            {
                printInConcavo(((CInternalNode*)pNode)->getChild(i), count-2);
            }
            for (j=count; j>=0; --j)
            {
                cout<<"-";
            }
            cout<<pNode->getKeyValue(i)<<endl;
        }
        if (pNode->getType()!=LEAF)
        {
            printInConcavo(((CInternalNode*)pNode)->getChild(i), count-2);
        }
    }
}

void CBPlusTree::printData()const
{
    CLeafNode* itr = m_DataHead;
    while(itr!=NULL)
    {
        for (int i=0; i<itr->getKeyNum(); ++i)
        {
            cout<<itr->getData(i)<<" ";
        }
        cout<<endl;
        itr = itr->getRightSibling();
    }
}

bool CBPlusTree::remove(KeyType key)
{
    if (!search(key))  //不存在
    {
        return false;
    }
    if (m_Root->getKeyNum()==1)//特殊情况处理
    {
        if (m_Root->getType()==LEAF)
        {
            clear();
            return true;
        }
        else
        {
            CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
            CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
            if (pChild1->getKeyNum()==MINNUM_KEY && pChild2->getKeyNum()==MINNUM_KEY)
            {
                pChild1->mergeChild(m_Root, pChild2, 0);
                delete m_Root;
                m_Root = pChild1;
            }
        }
    }
    recursive_remove(m_Root, key);
    return true;
}

// parentNode中包含的键值数>MINNUM_KEY
void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key)
{
    int keyIndex = parentNode->getKeyIndex(key);
    int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (parentNode->getType()==LEAF)// 找到目标叶子节点
    {
        if (key==m_MaxKey&&keyIndex>0)
        {
            m_MaxKey = parentNode->getKeyValue(keyIndex-1);
        }
        parentNode->removeKey(keyIndex, childIndex);  // 直接删除
        // 如果键值在内部结点中存在，也要相应的替换内部结点
        if (childIndex==0 && m_Root->getType()!=LEAF && parentNode!=m_DataHead)
        {
            changeKey(m_Root, key, parentNode->getKeyValue(0));
        }
    }
    else // 内结点
    {
        CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex); //包含key的子树根节点
        if (pChildNode->getKeyNum()==MINNUM_KEY)                       // 包含关键字达到下限值，进行相关操作
        {
            CNode *pLeft = childIndex>0 ? ((CInternalNode*)parentNode)->getChild(childIndex-1) : NULL;                       //左兄弟节点
            CNode *pRight = childIndex<parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex+1) : NULL;//右兄弟节点
            // 先考虑从兄弟结点中借
            if (pLeft && pLeft->getKeyNum()>MINNUM_KEY)// 左兄弟结点可借
            {
                pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
            }
            else if (pRight && pRight->getKeyNum()>MINNUM_KEY)//右兄弟结点可借
            {
                pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
            }
                //左右兄弟节点都不可借，考虑合并
            else if (pLeft)                    //与左兄弟合并
            {
                pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
                pChildNode = pLeft;
            }
            else if (pRight)                   //与右兄弟合并
            {
                pChildNode->mergeChild(parentNode, pRight, childIndex);
            }
        }
        recursive_remove(pChildNode, key);
    }
}

void CBPlusTree::changeKey(CNode *pNode, KeyType oldKey, KeyType newKey)
{
    if (pNode!=NULL && pNode->getType()!=LEAF)
    {
        int keyIndex = pNode->getKeyIndex(oldKey);
        if (keyIndex<pNode->getKeyNum() && oldKey==pNode->getKeyValue(keyIndex))  // 找到
        {
            pNode->setKeyValue(keyIndex, newKey);
        }
        else   // 继续找
        {
            changeKey(((CInternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
        }
    }
}

bool CBPlusTree::update(KeyType oldKey, KeyType newKey)
{
    if (search(newKey)) // 检查更新后的键是否已经存在
    {
        return false;
    }
    else
    {
        int dataValue;
        remove(oldKey, dataValue);
        if (dataValue==INVALID_INDEX)
        {
            return false;
        }
        else
        {
            return insert(newKey, dataValue);
        }
    }
}

void CBPlusTree::remove(KeyType key, DataType& dataValue)
{
    if (!search(key))  //不存在
    {
        dataValue = INVALID_INDEX;
        return;
    }
    if (m_Root->getKeyNum()==1)//特殊情况处理
    {
        if (m_Root->getType()==LEAF)
        {
            dataValue = ((CLeafNode*)m_Root)->getData(0);
            clear();
            return;
        }
        else
        {
            CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
            CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
            if (pChild1->getKeyNum()==MINNUM_KEY && pChild2->getKeyNum()==MINNUM_KEY)
            {
                pChild1->mergeChild(m_Root, pChild2, 0);
                delete m_Root;
                m_Root = pChild1;
            }
        }
    }
    recursive_remove(m_Root, key, dataValue);
}

void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue)
{
    int keyIndex = parentNode->getKeyIndex(key);
    int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (parentNode->getType()==LEAF)// 找到目标叶子节点
    {
        if (key==m_MaxKey&&keyIndex>0)
        {
            m_MaxKey = parentNode->getKeyValue(keyIndex-1);
        }
        dataValue = ((CLeafNode*)parentNode)->getData(keyIndex);
        parentNode->removeKey(keyIndex, childIndex);  // 直接删除
        // 如果键值在内部结点中存在，也要相应的替换内部结点
        if (childIndex==0 && m_Root->getType()!=LEAF && parentNode!=m_DataHead)
        {
            changeKey(m_Root, key, parentNode->getKeyValue(0));
        }
    }
    else // 内结点
    {
        CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex); //包含key的子树根节点
        if (pChildNode->getKeyNum()==MINNUM_KEY)                       // 包含关键字达到下限值，进行相关操作
        {
            CNode *pLeft = childIndex>0 ? ((CInternalNode*)parentNode)->getChild(childIndex-1) : NULL;                       //左兄弟节点
            CNode *pRight = childIndex<parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex+1) : NULL;//右兄弟节点
            // 先考虑从兄弟结点中借
            if (pLeft && pLeft->getKeyNum()>MINNUM_KEY)// 左兄弟结点可借
            {
                pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
            }
            else if (pRight && pRight->getKeyNum()>MINNUM_KEY)//右兄弟结点可借
            {
                pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
            }
                //左右兄弟节点都不可借，考虑合并
            else if (pLeft)                    //与左兄弟合并
            {
                pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
                pChildNode = pLeft;
            }
            else if (pRight)                   //与右兄弟合并
            {
                pChildNode->mergeChild(parentNode, pRight, childIndex);
            }
        }
        recursive_remove(pChildNode, key, dataValue);
    }
}

vector<DataType> CBPlusTree::select(KeyType compareKey, int compareOpeartor)
{
    vector<DataType> results;
    if (m_Root!=NULL)
    {
        if (compareKey>m_MaxKey)   // 比较键值大于B+树中最大的键值
        {
            if (compareOpeartor==LE || compareOpeartor==LT)
            {
                for(CLeafNode* itr = m_DataHead; itr!=NULL; itr= itr->getRightSibling())
                {
                    for (int i=0; i<itr->getKeyNum(); ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                }
            }
        }
        else if (compareKey<m_DataHead->getKeyValue(0))  // 比较键值小于B+树中最小的键值
        {
            if (compareOpeartor==BE || compareOpeartor==BT)
            {
                for(CLeafNode* itr = m_DataHead; itr!=NULL; itr= itr->getRightSibling())
                {
                    for (int i=0; i<itr->getKeyNum(); ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                }
            }
        }
        else  // 比较键值在B+树中
        {
            SelectResult result;
            search(compareKey, result);
            switch(compareOpeartor)
            {
                case LT:
                case LE:
                {
                    CLeafNode* itr = m_DataHead;
                    int i;
                    while (itr!=result.targetNode)
                    {
                        for (i=0; i<itr->getKeyNum(); ++i)
                        {
                            results.push_back(itr->getData(i));
                        }
                        itr = itr->getRightSibling();
                    }
                    for (i=0; i<result.keyIndex; ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                    if (itr->getKeyValue(i)<compareKey ||
                        (compareOpeartor==LE && compareKey==itr->getKeyValue(i)))
                    {
                        results.push_back(itr->getData(i));
                    }
                }
                    break;
                case EQ:
                {
                    if (result.targetNode->getKeyValue(result.keyIndex)==compareKey)
                    {
                        results.push_back(result.targetNode->getData(result.keyIndex));
                    }
                }
                    break;
                case BE:
                case BT:
                {
                    CLeafNode* itr = result.targetNode;
                    if (compareKey<itr->getKeyValue(result.keyIndex) ||
                        (compareOpeartor==BE && compareKey==itr->getKeyValue(result.keyIndex))
                            )
                    {
                        results.push_back(itr->getData(result.keyIndex));
                    }
                    int i;
                    for (i=result.keyIndex+1; i<itr->getKeyNum(); ++i)
                    {
                        results.push_back(itr->getData(i));
                    }
                    itr = itr->getRightSibling();
                    while (itr!=NULL)
                    {
                        for (i=0; i<itr->getKeyNum(); ++i)
                        {
                            results.push_back(itr->getData(i));
                        }
                        itr = itr->getRightSibling();
                    }
                }
                    break;
                default:  // 范围查询
                    break;
            }
        }
    }
    sort<vector<DataType>::iterator>(results.begin(), results.end());
    return results;
}

vector<DataType> CBPlusTree::select(KeyType smallKey, KeyType largeKey)
{
    vector<DataType> results;
    if (smallKey<=largeKey)
    {
        SelectResult start, end;
        search(smallKey, start);
        search(largeKey, end);
        CLeafNode* itr = start.targetNode;
        int i = start.keyIndex;
        if (itr->getKeyValue(i)<smallKey)
        {
            ++i;
        }
        if (end.targetNode->getKeyValue(end.keyIndex)>largeKey)
        {
            --end.keyIndex;
        }
        while (itr!=end.targetNode)
        {
            for (; i<itr->getKeyNum(); ++i)
            {
                results.push_back(itr->getData(i));
            }
            itr = itr->getRightSibling();
            i = 0;
        }
        for (; i<=end.keyIndex; ++i)
        {
            results.push_back(itr->getData(i));
        }
    }
    sort<vector<DataType>::iterator>(results.begin(), results.end());
    return results;
}

void CBPlusTree::search(KeyType key, SelectResult& result)
{
    recursive_search(m_Root, key, result);
}

void CBPlusTree::recursive_search(CNode* pNode, KeyType key, SelectResult& result)
{
    int keyIndex = pNode->getKeyIndex(key);
    int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
    if (pNode->getType()==LEAF)
    {
        result.keyIndex = keyIndex;
        result.targetNode = (CLeafNode*)pNode;
        return;
    }
    else
    {
        return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key, result);
    }
}