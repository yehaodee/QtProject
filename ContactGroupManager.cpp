#include "ContactGroupManager.h"
#include "ContactModel.h"

ContactGroupManager::ContactGroupManager(QObject *parent)
    : QObject(parent), contactModel(nullptr) {}

ContactGroupManager::~ContactGroupManager() {
    qDeleteAll(groups);
    groups.clear();
}

void ContactGroupManager::setContactModel(ContactModel *model) {
    contactModel = model;
}

ContactModel* ContactGroupManager::getContactModel() const {
    return contactModel;
}

bool ContactGroupManager::createGroup(const QString &groupName) {
    if (groupExists(groupName)) {
        return false;
    }
    ContactGroup *group = new ContactGroup(groupName, this);
    groups.insert(groupName, group);
    return true;
}

bool ContactGroupManager::deleteGroup(const QString &groupName) {
    if (!groupExists(groupName)) {
        return false;
    }
    ContactGroup *group = groups.take(groupName);
    delete group;
    return true;
}

bool ContactGroupManager::groupExists(const QString &groupName) const {
    return groups.contains(groupName);
}

QList<QString> ContactGroupManager::getAllGroupNames() const {
    return groups.keys();
}

QList<ContactGroup*> ContactGroupManager::getAllGroups() const {
    return groups.values();
}

ContactGroup* ContactGroupManager::getGroup(const QString &groupName) const {
    return groups.value(groupName, nullptr);
}

bool ContactGroupManager::addContactToGroup(const QString &contactId, const QString &groupName) {
    ContactGroup *group = getGroup(groupName);
    if (!group) {
        return false;
    }
    group->addContactById(contactId);
    return true;
}

bool ContactGroupManager::removeContactFromGroup(const QString &contactId, const QString &groupName) {
    ContactGroup *group = getGroup(groupName);
    if (!group) {
        return false;
    }
    group->removeContactById(contactId);
    return true;
}

bool ContactGroupManager::isContactInGroup(const QString &contactId, const QString &groupName) const {
    ContactGroup *group = getGroup(groupName);
    if (!group) {
        return false;
    }
    return group->containsContactById(contactId);
}

QList<QString> ContactGroupManager::getGroupsForContact(const QString &contactId) const {
    QList<QString> result;
    for (ContactGroup *group : groups.values()) {
        if (group->containsContactById(contactId)) {
            result.append(group->getGroupName());
        }
    }
    return result;
}

QList<Contact> ContactGroupManager::getContactsInGroup(const QString &groupName) const {
    ContactGroup *group = getGroup(groupName);
    if (!group || !contactModel) {
        return QList<Contact>();
    }

    QList<Contact> result;
    for (const QString &id : group->getContactIds()) {
        for (int i = 0; i < contactModel->rowCount(); ++i) {
            Contact c = contactModel->getContact(i);
            if (c.id == id) {
                result.append(c);
                break;
            }
        }
    }
    return result;
}

void ContactGroupManager::addContactToAllGroups(const QString &contactId) {
    for (ContactGroup *group : groups.values()) {
        group->addContactById(contactId);
    }
}

void ContactGroupManager::removeContactFromAllGroups(const QString &contactId) {
    for (ContactGroup *group : groups.values()) {
        group->removeContactById(contactId);
    }
}

void ContactGroupManager::clearAllGroups() {
    qDeleteAll(groups);
    groups.clear();
}
