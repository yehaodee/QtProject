#ifndef LRUCache_H
#define LRUCache_H

#include <QHash>
#include <QString>

template <typename T>
class LRUCache
{
private:
    struct Node {
        QString key;
        T value;
        Node *prev;
        Node *next;
        Node(const QString &k, const T &v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

public:
    explicit LRUCache(int capacity = 50);
    ~LRUCache();

    void setCapacity(int capacity);
    int capacity() const;
    int size() const;
    bool isEmpty() const;

    void put(const QString &key, const T &value);
    bool contains(const QString &key) const;
    T get(const QString &key);
    T remove(const QString &key);
    void clear();

    QList<QString> getKeys() const;
    QList<T> getValues() const;

private:
    void detachNode(Node *node);
    void attachToHead(Node *node);
    void deleteNode(Node *node);

    QHash<QString, Node*> hashTable;
    Node *head;
    Node *tail;
    int maxCapacity;
    int currentSize;
};

#endif // LRUCache_H