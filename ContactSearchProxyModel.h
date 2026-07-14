#ifndef CONTACTSEARCHPROXYMODEL_H
#define CONTACTSEARCHPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "Trie.h"

class ContactGroupManager;

class ContactSearchProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit ContactSearchProxyModel(QObject *parent = nullptr);

    void rebuildIndex();
    void addContactIndex(int row);
    void removeContactIndex(int row);
    void updateContactIndex(int row);

    void setGroupManager(ContactGroupManager *manager);
    void setCurrentGroup(const QString &groupName);
    QString getCurrentGroup() const;
    void refreshFilter();

public slots:
    void setSearchKeyword(const QString &keyword);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Trie nameTrie;
    Trie phoneTrie;
    QString currentKeyword;
    QString currentGroup;
    ContactGroupManager *groupManager;
};

#endif // CONTACTSEARCHPROXYMODEL_H
