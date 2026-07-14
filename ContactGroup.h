#ifndef CONTACTGROUP_H
#define CONTACTGROUP_H

#include <QObject>
#include <QString>
#include <QList>

class Contact;

class ContactGroup : public QObject
{
    Q_OBJECT
public:
    explicit ContactGroup(const QString &name = "", QObject *parent = nullptr);
    ~ContactGroup();

    QString getGroupName() const;
    void setGroupName(const QString &name);

    int getContactCount() const;
    QList<QString> getContactIds() const;

    void addContactById(const QString &contactId);
    void removeContactById(const QString &contactId);
    bool containsContactById(const QString &contactId) const;

    void clearContacts();

private:
    QString groupName;
    QList<QString> contactIds;
};

#endif // CONTACTGROUP_H
