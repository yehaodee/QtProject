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
    void setExcludeGroup(const QString &groupName);
    QString getCurrentGroup() const;
    void refreshFilter();

    void setSourceModel(QAbstractItemModel *model) override;

public slots:
    void setSearchKeyword(const QString &keyword);
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Trie nameTrie;
    Trie phoneTrie;
    QString currentKeyword;
    QString currentGroup;
    bool excludeGroup;
    ContactGroupManager *groupManager;
};

#endif // CONTACTSEARCHPROXYMODEL_H
