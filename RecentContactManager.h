#ifndef RECENTCONTACTMANAGER_H
#define RECENTCONTACTMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include "LRUCache.h"

class ContactModel;

class RecentContactManager : public QObject
{
    Q_OBJECT
public:
    explicit RecentContactManager(ContactModel *model, QObject *parent = nullptr);

    void setContactModel(ContactModel *model);
    ContactModel* getContactModel() const;

    void recordContact(const QString &contactId);
    void removeContact(const QString &contactId);
    void clear();

    QList<QString> getRecentContactIds() const;
    int getRecentCount() const;

    void loadFromJson(const QJsonArray &array);
    QJsonArray saveToJson() const;

signals:
    void recentContactsChanged();

private:
    ContactModel *contactModel;
    LRUCache<QString> lruCache;
    static const QString RecentGroupName;
};

#endif // RECENTCONTACTMANAGER_H