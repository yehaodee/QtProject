#include "Trie.h"

Trie::Trie() : root(new TrieNode()) {}

Trie::~Trie() {
    delete root;
}

void Trie::insert(const QString &word, int rowIndex) {
    TrieNode *node = root;
    for (QChar c : word.toLower()) {
        if (!node->children.contains(c)) {
            node->children[c] = new TrieNode();
        }
        node = node->children[c];
        if (!node->rowIndices.contains(rowIndex)) {
            node->rowIndices.append(rowIndex);
        }
    }
    node->isEnd = true;
}

void Trie::remove(const QString &word, int rowIndex) {
    TrieNode *node = root;
    for (QChar c : word.toLower()) {
        if (!node->children.contains(c)) {
            return;
        }
        node = node->children[c];
        node->rowIndices.removeOne(rowIndex);
    }
}

QList<int> Trie::search(const QString &prefix) const {
    if (prefix.isEmpty()) {
        return QList<int>();
    }
    TrieNode *node = root;
    for (QChar c : prefix.toLower()) {
        if (!node->children.contains(c)) {
            return QList<int>();
        }
        node = node->children[c];
    }
    return node->rowIndices;
}

void Trie::clear() {
    delete root;
    root = new TrieNode();
}
