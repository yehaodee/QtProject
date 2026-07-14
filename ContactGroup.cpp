#include "ContactGroup.h"

ContactGroup::ContactGroup(const QString &name, QObject *parent)
    : QObject(parent), groupName(name) {}

ContactGroup::~ContactGroup() {}

QString ContactGroup::getGroupName() const {
    return groupName;
}

void ContactGroup::setGroupName(const QString &name) {
    groupName = name;
}

int ContactGroup::getContactCount() const {
    return contactIds.size();
}

QList<QString> ContactGroup::getContactIds() const {
    return contactIds;
}

void ContactGroup::addContactById(const QString &contactId) {
    if (!containsContactById(contactId)) {
        contactIds.append(contactId);
    }
}

void ContactGroup::removeContactById(const QString &contactId) {
    contactIds.removeOne(contactId);
}

bool ContactGroup::containsContactById(const QString &contactId) const {
    return contactIds.contains(contactId);
}

void ContactGroup::clearContacts() {
    contactIds.clear();
}
