#include "RecentContactManager.h"
#include "ContactModel.h"
#include <QJsonArray>

const QString RecentContactManager::RecentGroupName = "最近联系";

RecentContactManager::RecentContactManager(ContactModel *model, QObject *parent)
    : QObject(parent), contactModel(model), lruCache(50)
{
}

void RecentContactManager::setContactModel(ContactModel *model)
{
    contactModel = model;
}

ContactModel* RecentContactManager::getContactModel() const
{
    return contactModel;
}

void RecentContactManager::recordContact(const QString &contactId)
{
    if (contactId.isEmpty()) return;
    lruCache.put(contactId, contactId);
    emit recentContactsChanged();
}

void RecentContactManager::removeContact(const QString &contactId)
{
    lruCache.remove(contactId);
    emit recentContactsChanged();
}

void RecentContactManager::clear()
{
    lruCache.clear();
    emit recentContactsChanged();
}

QList<QString> RecentContactManager::getRecentContactIds() const
{
    return lruCache.getKeys();
}

int RecentContactManager::getRecentCount() const
{
    return lruCache.size();
}

void RecentContactManager::loadFromJson(const QJsonArray &array)
{
    lruCache.clear();
    for (const QJsonValue &val : array) {
        QString id = val.toString();
        if (!id.isEmpty()) {
            lruCache.put(id, id);
        }
    }
}

QJsonArray RecentContactManager::saveToJson() const
{
    QJsonArray array;
    for (const QString &id : lruCache.getKeys()) {
        array.append(id);
    }
    return array;
}