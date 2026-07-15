#ifndef CONTACTSEARCHPROXYMODEL_H
#define CONTACTSEARCHPROXYMODEL_H

#include <QSortFilterProxyModel>

class ContactGroupManager;
class RecentContactManager;

class ContactSearchProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit ContactSearchProxyModel(QObject *parent = nullptr);

    void setGroupManager(ContactGroupManager *manager);
    void setRecentContactManager(RecentContactManager *manager);
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
    void onRecentContactsChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    QString currentKeyword;
    QString currentGroup;
    bool excludeGroup;
    ContactGroupManager *groupManager;
    RecentContactManager *recentManager;
};

#endif // CONTACTSEARCHPROXYMODEL_H