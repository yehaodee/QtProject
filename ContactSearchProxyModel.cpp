#include "ContactSearchProxyModel.h"
#include "ContactModel.h"
#include "ContactGroupManager.h"
#include "RecentContactManager.h"
#include "KMP.h"

ContactSearchProxyModel::ContactSearchProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent), groupManager(nullptr), recentManager(nullptr), excludeGroup(false) {}

void ContactSearchProxyModel::setSourceModel(QAbstractItemModel *model) {
    QSortFilterProxyModel::setSourceModel(model);
    
    if (model) {
        connect(model, &QAbstractItemModel::rowsInserted,
                this, &ContactSearchProxyModel::onRowsInserted);
        connect(model, &QAbstractItemModel::rowsRemoved,
                this, &ContactSearchProxyModel::onRowsRemoved);
        connect(model, &QAbstractItemModel::dataChanged,
                this, &ContactSearchProxyModel::onDataChanged);
        connect(model, &QAbstractItemModel::modelReset,
                this, &ContactSearchProxyModel::refreshFilter);
    }
}

void ContactSearchProxyModel::setGroupManager(ContactGroupManager *manager) {
    groupManager = manager;
}

void ContactSearchProxyModel::setRecentContactManager(RecentContactManager *manager) {
    recentManager = manager;
}

void ContactSearchProxyModel::setCurrentGroup(const QString &groupName) {
    currentGroup = groupName;
    excludeGroup = false;
    invalidateFilter();
    if (groupName == "最近联系") {
        sort(0, Qt::AscendingOrder);
    }
}

void ContactSearchProxyModel::setExcludeGroup(const QString &groupName) {
    currentGroup = groupName;
    excludeGroup = true;
    invalidateFilter();
}

QString ContactSearchProxyModel::getCurrentGroup() const {
    return currentGroup;
}

void ContactSearchProxyModel::refreshFilter() {
    invalidateFilter();
}

void ContactSearchProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    invalidateFilter();
}

void ContactSearchProxyModel::onRowsRemoved(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    invalidateFilter();
}

void ContactSearchProxyModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    invalidateFilter();
}

void ContactSearchProxyModel::onRecentContactsChanged() {
    if (currentGroup == "最近联系") {
        invalidateFilter();
    }
}

void ContactSearchProxyModel::setSearchKeyword(const QString &keyword) {
    currentKeyword = keyword;
    invalidateFilter();
}

bool ContactSearchProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
    Q_UNUSED(source_parent);

    ContactModel *source = qobject_cast<ContactModel*>(sourceModel());
    if (!source) return false;

    Contact c = source->getContact(source_row);

    if (!currentGroup.isEmpty() && currentGroup != "全部") {
        if (currentGroup == "最近联系") {
            bool isRecent = recentManager && recentManager->getRecentContactIds().contains(c.id);
            if (!isRecent) {
                return false;
            }
        } else {
            bool isInGroup = groupManager && groupManager->isContactInGroup(c.id, currentGroup);
            if (excludeGroup) {
                if (isInGroup) {
                    return false;
                }
            } else {
                if (!isInGroup) {
                    return false;
                }
            }
        }
    }

    if (currentKeyword.isEmpty()) {
        return true;
    }

    QString keyword = currentKeyword.toLower();
    bool nameMatch = KMP::contains(c.name.toLower(), keyword);
    bool phoneMatch = KMP::contains(c.phone.toLower(), keyword);
    return nameMatch || phoneMatch;
}

bool ContactSearchProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (currentGroup == "最近联系" && recentManager) {
        ContactModel *source = qobject_cast<ContactModel*>(sourceModel());
        if (!source) return QSortFilterProxyModel::lessThan(left, right);

        Contact leftContact = source->getContact(left.row());
        Contact rightContact = source->getContact(right.row());

        QList<QString> recentIds = recentManager->getRecentContactIds();
        int leftIndex = recentIds.indexOf(leftContact.id);
        int rightIndex = recentIds.indexOf(rightContact.id);

        if (leftIndex >= 0 && rightIndex >= 0) {
            return leftIndex < rightIndex;
        }
    }
    return QSortFilterProxyModel::lessThan(left, right);
}