#include "ContactSearchProxyModel.h"
#include "ContactModel.h"
#include "ContactGroupManager.h"

ContactSearchProxyModel::ContactSearchProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent), groupManager(nullptr) {}

void ContactSearchProxyModel::rebuildIndex() {
    nameTrie.clear();
    phoneTrie.clear();

    ContactModel *source = qobject_cast<ContactModel*>(sourceModel());
    if (!source) return;

    for (int i = 0; i < source->rowCount(); ++i) {
        Contact c = source->getContact(i);
        if (!c.name.isEmpty()) {
            nameTrie.insert(c.name, i);
        }
        if (!c.phone.isEmpty()) {
            phoneTrie.insert(c.phone, i);
        }
    }
}

void ContactSearchProxyModel::addContactIndex(int row) {
    ContactModel *source = qobject_cast<ContactModel*>(sourceModel());
    if (!source) return;

    Contact c = source->getContact(row);
    if (!c.name.isEmpty()) {
        nameTrie.insert(c.name, row);
    }
    if (!c.phone.isEmpty()) {
        phoneTrie.insert(c.phone, row);
    }
}

void ContactSearchProxyModel::removeContactIndex(int row) {
    ContactModel *source = qobject_cast<ContactModel*>(sourceModel());
    if (!source) return;

    Contact c = source->getContact(row);
    if (!c.name.isEmpty()) {
        nameTrie.remove(c.name, row);
    }
    if (!c.phone.isEmpty()) {
        phoneTrie.remove(c.phone, row);
    }
}

void ContactSearchProxyModel::updateContactIndex(int row) {
    removeContactIndex(row);
    addContactIndex(row);
}

void ContactSearchProxyModel::setGroupManager(ContactGroupManager *manager) {
    groupManager = manager;
}

void ContactSearchProxyModel::setCurrentGroup(const QString &groupName) {
    currentGroup = groupName;
    invalidateFilter();
}

void ContactSearchProxyModel::refreshFilter() {
    invalidateFilter();
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
        if (!groupManager || !groupManager->isContactInGroup(c.id, currentGroup)) {
            return false;
        }
    }

    if (currentKeyword.isEmpty()) {
        return true;
    }

    QList<int> nameMatches = nameTrie.search(currentKeyword);
    QList<int> phoneMatches = phoneTrie.search(currentKeyword);

    return nameMatches.contains(source_row) || phoneMatches.contains(source_row);
}
