//
// Created by 19254 on 2023/9/3.
//

#ifndef HELLOWORD_DYNAMIC_FW_CLIST_HPP
#define HELLOWORD_DYNAMIC_FW_CLIST_HPP

#include "FreeRTOS.h"

namespace HYSDK {

template<typename T>
class CList {
public:
    struct Node {
        T val;  //存储数据
        Node *pNext;  //指向下一个节点的指针
        Node(T v) {//构造函数初始化
            val = v;
            pNext = nullptr;
        }
    };

    Node* m_pHead;
    Node* m_pEnd;
    unsigned char m_nLen;
public:
    CList() {
        m_pHead = m_pHead = nullptr;
        m_nLen = 0;
    }
    ~CList() {
        Node* pNode = nullptr;
        while (m_pHead) { //如果链表不为空，循环
            pNode = m_pHead;  //标记头

            m_pHead = m_pHead->pNext; //头向后移动
            vPortFree(pNode); //删除标记的
        }
        m_pHead = nullptr;
        m_pEnd = nullptr;
        m_nLen = 0;
    }

    void PushBack(T v) {
        auto pNode = (Node*)pvPortMalloc(sizeof(Node));
        *pNode = Node(v);
        //Node* pNode = new Node(v);
        if (m_pHead) { //非空链表
            m_pEnd->pNext = pNode;
            m_pEnd = pNode;
        }
        else {//空链表
            m_pHead = pNode;
        }
        m_pEnd = pNode;
        m_nLen++;
    }

    void PopFront() {
        if (m_pHead) {
            Node* pNode = m_pHead;  //标记头，也是将来要删除的
            if (m_pHead == m_pEnd) {  //1个节点
                m_pHead = m_pEnd = nullptr;
            }
            else {  //多个节点
                m_pHead = m_pHead->pNext;//向后移动
            }
            vPortFree(pNode); //删除标记的
            pNode = nullptr;
            m_nLen--;
        }
    }

    int GetLength() {
        return m_nLen;
    }

    auto GetHeadPtr() {
        return m_pHead;
    }

};
}


#endif //HELLOWORD_DYNAMIC_FW_CLIST_HPP
