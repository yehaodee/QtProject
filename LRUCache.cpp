#include "LRUCache.h"
#include <QList>

template <typename T>
LRUCache<T>::LRUCache(int capacity)
    : maxCapacity(capacity), currentSize(0), head(nullptr), tail(nullptr)
{
}

template <typename T>
LRUCache<T>::~LRUCache()
{
    clear();
}

template <typename T>
void LRUCache<T>::setCapacity(int capacity)
{
    maxCapacity = capacity;
    while (currentSize > maxCapacity) {
        remove(tail->key);
    }
}

template <typename T>
int LRUCache<T>::capacity() const
{
    return maxCapacity;
}

template <typename T>
int LRUCache<T>::size() const
{
    return currentSize;
}

template <typename T>
bool LRUCache<T>::isEmpty() const
{
    return currentSize == 0;
}

template <typename T>
void LRUCache<T>::detachNode(Node *node)
{
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
}

template <typename T>
void LRUCache<T>::attachToHead(Node *node)
{
    node->prev = nullptr;
    node->next = head;

    if (head) {
        head->prev = node;
    } else {
        tail = node;
    }

    head = node;
}

template <typename T>
void LRUCache<T>::deleteNode(Node *node)
{
    delete node;
}

template <typename T>
void LRUCache<T>::put(const QString &key, const T &value)
{
    if (hashTable.contains(key)) {
        Node *node = hashTable[key];
        node->value = value;
        detachNode(node);
        attachToHead(node);
        return;
    }

    Node *newNode = new Node(key, value);
    hashTable[key] = newNode;
    attachToHead(newNode);
    currentSize++;

    if (currentSize > maxCapacity) {
        remove(tail->key);
    }
}

template <typename T>
bool LRUCache<T>::contains(const QString &key) const
{
    return hashTable.contains(key);
}

template <typename T>
T LRUCache<T>::get(const QString &key)
{
    if (!hashTable.contains(key)) {
        return T();
    }

    Node *node = hashTable[key];
    detachNode(node);
    attachToHead(node);
    return node->value;
}

template <typename T>
T LRUCache<T>::remove(const QString &key)
{
    if (!hashTable.contains(key)) {
        return T();
    }

    Node *node = hashTable[key];
    T value = node->value;

    detachNode(node);
    hashTable.remove(key);
    deleteNode(node);
    currentSize--;

    return value;
}

template <typename T>
void LRUCache<T>::clear()
{
    Node *current = head;
    while (current) {
        Node *next = current->next;
        delete current;
        current = next;
    }
    hashTable.clear();
    head = nullptr;
    tail = nullptr;
    currentSize = 0;
}

template <typename T>
QList<QString> LRUCache<T>::getKeys() const
{
    QList<QString> keys;
    Node *current = head;
    while (current) {
        keys.append(current->key);
        current = current->next;
    }
    return keys;
}

template <typename T>
QList<T> LRUCache<T>::getValues() const
{
    QList<T> values;
    Node *current = head;
    while (current) {
        values.append(current->value);
        current = current->next;
    }
    return values;
}

template class LRUCache<QString>;