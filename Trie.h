#ifndef TRIE_H
#define TRIE_H

#include <QMap>
#include <QList>
#include <QString>

class TrieNode {
public:
    QMap<QChar, TrieNode*> children;
    QList<int> rowIndices;
    bool isEnd;

    TrieNode() : isEnd(false) {}
    ~TrieNode() {
        qDeleteAll(children);
    }
};

class Trie {
public:
    Trie();
    ~Trie();

    void insert(const QString &word, int rowIndex);
    void remove(const QString &word, int rowIndex);
    QList<int> search(const QString &prefix) const;
    void clear();

private:
    TrieNode *root;
};

#endif // TRIE_H
