#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <exception>
using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::vector;
using std::exception;
using std::out_of_range;

template<typename T>
class LinkedList {
public:
    struct Node {
        T data;
        Node* next;
        Node* prev;
    };

    Node* head;
    Node* tail;

    //Constructors/Destructors
    LinkedList();   //Default Constructor
    void Set(const LinkedList<T>& list);    //For use in Copy Constructor and Copy Assignment Operator to prevent code duplication
    LinkedList(const LinkedList<T>& list);  //Copy Constructor
    ~LinkedList();  //Destructor

    //Operators
    LinkedList<T>& operator=(const LinkedList<T>& rhs); //Copy assignment operator
    T& operator[](unsigned int index);
    const T& operator[](unsigned int index) const;
    bool operator==(const LinkedList<T>& rhs) const;

    //Behaviors
    void PrintForward() const;
    void PrintReverse() const;
    void PrintForwardRecursive(const Node* node) const;
    void PrintReverseRecursive(const Node* node) const;

    //Accessors
    unsigned int NodeCount() const;
    void FindAll(vector<Node*>& outData, const T& value) const;
    Node* Head();
    const Node* Head() const;
    Node* Tail();
    const Node* Tail() const;
    Node* GetNode(unsigned int index);
    const Node* GetNode(unsigned int index) const;
    Node* Find(const T& data);
    const Node* Find(const T& data) const;

    //Mutators
    void AddHead(const T& data);
    void AddTail(const T& data);
    void AddNodesHead(const T* data, unsigned int count);
    void AddNodesTail(const T* data, unsigned int count);
    void InsertAfter(Node* node, const T& data);
    void InsertBefore(Node* node, const T& data);
    void InsertAt(const T& data, unsigned int index);
    bool RemoveHead();
    bool RemoveTail();
    void RemoveNode(Node* node); //this is an extra function I wrote so I don't have to duplicate too much code
    unsigned int Remove(const T& data);
    bool RemoveAt(unsigned int index);
    void Clear();
};

//Constructors/Destructors
template <typename T>
LinkedList<T>::LinkedList() { //Default Constructor
    head = nullptr;
    tail = nullptr;
}

template <typename T>
void LinkedList<T>::Set(const LinkedList<T>& list) {
    head = nullptr;
    tail = nullptr;
    for (Node* temp = list.head; temp; temp = temp->next) {
        this->AddTail(temp->data);
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) { //Copy Constructor
    Set(list);
}

template <typename T>
LinkedList<T>::~LinkedList() { //Destructor
    Node* cur = head;
    Node* next;
    while (cur != nullptr) {
        next = cur->next;
        delete cur;
        cur = next;
    }
}

//Operators
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
    this->~LinkedList(); //ZyDE won't let you, so just copy code
    Set(rhs);
    return *this;
}

template <typename T>
T& LinkedList<T>::operator[](unsigned int index) {
    return GetNode(index)->data;
}

template <typename T>
const T& LinkedList<T>::operator[](unsigned int index) const {
    return GetNode(index)->data;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& rhs) const {
    if (this->NodeCount() != rhs.NodeCount()) return false;
    Node* tempRHS = rhs.head;
    for (Node* tempLHS = head; tempLHS; tempLHS = tempLHS->next) {
        if (tempLHS->data != tempRHS->data) return false;
        tempRHS = tempRHS->next;
    }
    return true;
}

//Behaviors
template <typename T>
void LinkedList<T>::PrintForward() const {
    for (Node* temp = head; temp; temp = temp->next) {
        cout << temp->data << endl;
    }
}

template <typename T>
void LinkedList<T>::PrintReverse() const {
    for (Node* temp = tail; temp; temp = temp->prev) {
        cout << temp->data << endl;
    }
}

template <typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const {
    cout << node->data << endl;
    if (node->next) this->PrintForwardRecursive(node->next);
}

template <typename T>
void LinkedList<T>::PrintReverseRecursive(const Node* node) const {
    cout << node->data << endl;
    if (node->prev) this->PrintReverseRecursive(node->prev);
}

//Accessors
template <typename T>
unsigned int LinkedList<T>::NodeCount() const {
    int res = 0;
    for (Node* temp = head; temp; temp = temp->next) {
        res++;
    }
    return res;
}

template <typename T>
void LinkedList<T>::FindAll(vector<Node*>& outData, const T& value) const {
    for (Node* temp = head; temp; temp = temp->next) {
        if (temp->data == value) outData.push_back(temp);
    }
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head() {
    return head;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const {
    return head;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail() {
    return tail;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const {
    return tail;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) {
    if (index > NodeCount() - 1 || index < 0) throw out_of_range("Index is out of range.");
    unsigned int curIndex = 0;
    for (Node* temp = head; temp; temp = temp->next) {
        if (curIndex == index) return temp;
        curIndex++;
    }
    Node* temp = head;
    for (unsigned int i = 0; i < index; i++) {
        temp = temp->next;
    }
    return temp;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) const {
    if (index > NodeCount() - 1 || index < 0) throw out_of_range("Index is out of range.");
    unsigned int curIndex = 0;
    for (Node* temp = head; temp; temp = temp->next) {
        if (curIndex == index) return temp;
        curIndex++;
    }
    Node* temp = head;
    for (unsigned int i = 0; i < index; i++) {
        temp = temp->next;
    }
    return temp;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) {
    for (Node* temp = head; temp; temp = temp->next) {
        if (temp->data == data) return temp;
    }
    return nullptr;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) const {
    for (Node* temp = head; temp; temp = temp->next) {
        if (temp->data == data) return temp;
    }
    return nullptr;
}

//Mutators
template <typename T>
void LinkedList<T>::AddHead(const T& data) {
    if (head == nullptr) { //if/else logic necessary because you can't set the .next of nullptr, note segfault you got earlier
        head = new Node;
        head->data = data;
        head->next = nullptr;
        head->prev = nullptr;
        tail = head; //This is because there is only one item in list
    }

    else {
        Node* newHead = new Node;
        newHead->data = data;
        newHead->next = head;
        newHead->prev = nullptr;
        head->prev = newHead;
        head = newHead;
    }
}

template <typename T>
void LinkedList<T>::AddTail(const T& data) {
    if (tail == nullptr) { //if/else logic necessary because you can't set the .next of nullptr, note segfault you got earlier
        tail = new Node;
        tail->data = data;
        tail->next = nullptr;
        tail->prev = nullptr;
        head = tail; //This is because there is only one item in list
    }

    else {
        Node* newTail = new Node;
        newTail->data = data;
        newTail->next = nullptr;
        newTail->prev = tail;
        tail->next = newTail;
        tail = newTail;
    }
}

//I could do the AddNodes fns more efficiently, but this is much more readable
template <typename T>
void LinkedList<T>::AddNodesHead(const T* data, unsigned int count) {
    for (unsigned int i = 1; i <= count; i++) { //I would write the for loop for i to decrement, but since it is unsigned it underflows, causing it to never be less than 0. i is unsigned because count is unsigned, and there is a warning for comparing int to unsigned int
        this->AddHead(data[count - i]);
    }
}

template <typename T>
void LinkedList<T>::AddNodesTail(const T* data, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        this->AddTail(data[i]);
    }
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = node->next; //making the new node's next point to the node the param node used to point to, which the new one is now before
    newNode->prev = node;
    node->next->prev = newNode; //Setting the node after the param node's prev point to new node
    node->next = newNode;
}

template <typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = node;
    newNode->prev = node->prev;
    node->prev->next = newNode;
    node->prev = newNode;
}

template <typename T>
void LinkedList<T>::InsertAt(const T& data, unsigned int index) {
    if (index > NodeCount() || index < 0) throw out_of_range("Index is out of range.");
    else if (index == 0) AddHead(data);
    else if (index == NodeCount() - 1 || index == NodeCount()) AddTail(data);
    else {
        Node* newNode = new Node;
        newNode->data = data;
        newNode->next = GetNode(index);
        newNode->prev = newNode->next->prev; //what was before node originally at index, is now before newNode
        //setting surrounding nodes to point to newNode
        newNode->next->prev = newNode;
        newNode->prev->next = newNode;
    }
}

template <typename T>
bool LinkedList<T>::RemoveHead() {
    if (this->NodeCount() > 1) {
        Node* rNode = head->next;
        delete head;
        rNode->prev = nullptr;  //have to make prev a nullptr if nothing else
        head = rNode;    //have to make new head if removing head
        return true;
    }

    if (this->NodeCount() == 1) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return true;
    }

    return false;
}

template <typename T>
bool LinkedList<T>::RemoveTail() {
    if (this->NodeCount() > 1) {
        Node* lNode = tail->prev;
        delete tail;
        lNode->next = nullptr;
        tail = lNode;
        return true;
    }

    if (this->NodeCount() == 1) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return true;
    }
    return false;
}

template <typename T>
void LinkedList<T>::RemoveNode(Node* node) {
    if (node == head) {
        this->RemoveHead();
    }
    if (node == tail) {
        this->RemoveTail();
    }
    else {
        Node* lNode = node->prev;
        Node* rNode = node->next;
        delete node;
        rNode->prev = lNode;
        lNode->next = rNode;
    }
}

// probably could be simplified for less code repetition, but gotta keep that <i>readability</i>
template <typename T>
unsigned int LinkedList<T>::Remove(const T& data) {
    unsigned int count = 0;
    Node* temp = head;
    while (temp) {
        if (temp->data == data) {
            if (temp == head || temp == tail) {
                temp = temp->next;
                this->RemoveNode(temp->prev);
            }
            else {
                Node* lNode = temp->prev;
                Node* rNode = temp->next;
                delete temp;
                rNode->prev = lNode;
                lNode->next = rNode;
                temp = rNode;
            }
            count++;
        }
        else temp = temp->next;
    }
    return count;
}

template <typename T>
bool LinkedList<T>::RemoveAt(unsigned int index) {
    if (index >= this->NodeCount() || index < 0) return false;
    RemoveNode(this->GetNode(index));
    return true;
}

template <typename T>
void LinkedList<T>::Clear() {
    Node* cur = head;
    Node* next;
    while (cur != nullptr) {
        next = cur->next;
        delete cur;
        cur = next;
    }
    head = nullptr;
    tail = nullptr;
}
