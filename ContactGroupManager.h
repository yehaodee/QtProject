#ifndef CONTACTGROUPMANAGER_H
#define CONTACTGROUPMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "ContactGroup.h"
#include "Contact.h"

class ContactModel;

class ContactGroupManager : public QObject
{
    Q_OBJECT
public:
    explicit ContactGroupManager(QObject *parent = nullptr);
    ~ContactGroupManager();

    void setContactModel(ContactModel *model);
    ContactModel* getContactModel() const;

    bool createGroup(const QString &groupName);
    bool deleteGroup(const QString &groupName);
    bool groupExists(const QString &groupName) const;

    QList<QString> getAllGroupNames() const;
    QList<ContactGroup*> getAllGroups() const;
    ContactGroup* getGroup(const QString &groupName) const;

    bool addContactToGroup(const QString &contactId, const QString &groupName);
    bool removeContactFromGroup(const QString &contactId, const QString &groupName);
    bool isContactInGroup(const QString &contactId, const QString &groupName) const;

    QList<QString> getGroupsForContact(const QString &contactId) const;
    QList<Contact> getContactsInGroup(const QString &groupName) const;

    void addContactToAllGroups(const QString &contactId);
    void removeContactFromAllGroups(const QString &contactId);

    void clearAllGroups();

private:
    QMap<QString, ContactGroup*> groups;
    ContactModel *contactModel;
};

#endif // CONTACTGROUPMANAGER_H
