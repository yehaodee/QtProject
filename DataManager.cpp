#include "DataManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QDebug>

DataManager::DataManager(ContactModel *model, ContactGroupManager *manager,
                         const QString &filePath, QObject *parent)
    : QObject(parent), contactModel(model), groupManager(manager), filePath(filePath)
{
}

bool DataManager::save()
{
    QJsonObject root = serialize();

    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool DataManager::load()
{
    QFile file(filePath);
    if (!file.exists()) {
        return true;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << error.errorString();
        return false;
    }

    deserialize(doc.object());
    return true;
}

QJsonObject DataManager::serialize()
{
    QJsonObject root;

    root["version"] = 1;

    QJsonArray contactsArray;
    for (const Contact &c : contactModel->getAllContacts()) {
        QJsonObject obj;
        serializeContact(c, obj);
        contactsArray.append(obj);
    }
    root["contacts"] = contactsArray;

    // 序列化分组
    QJsonObject groupsObj;
    for (const QString &name : groupManager->getAllGroupNames()) {
        ContactGroup *group = groupManager->getGroup(name);
        if (group) {
            QJsonArray idArray;
            for (const QString &id : group->getContactIds()) {
                idArray.append(id);
            }
            groupsObj[name] = idArray;
        }
    }
    root["groups"] = groupsObj;

    return root;
}

void DataManager::deserialize(const QJsonObject &root)
{
    // 反序列化联系人
    QJsonArray contactsArray = root["contacts"].toArray();
    QList<Contact> contacts;
    for (const QJsonValue &val : contactsArray) {
        contacts.append(deserializeContact(val.toObject()));
    }
    contactModel->setAllContacts(contacts);

    // 反序列化分组
    groupManager->clearAllGroups();
    QJsonObject groupsObj = root["groups"].toObject();
    for (const QString &name : groupsObj.keys()) {
        groupManager->createGroup(name);
        QJsonArray idArray = groupsObj[name].toArray();
        for (const QJsonValue &idVal : idArray) {
            groupManager->addContactToGroup(idVal.toString(), name);
        }
    }
}

void DataManager::serializeContact(const Contact &c, QJsonObject &obj) const
{
    obj["id"] = c.id;
    obj["name"] = c.name;
    obj["phone"] = c.phone;
    obj["company"] = c.company;
    obj["position"] = c.position;
    obj["email"] = c.email;
    obj["address"] = c.address;
    obj["birthday"] = c.birthday.toString(Qt::ISODate);
    obj["notes"] = c.notes;
}

Contact DataManager::deserializeContact(const QJsonObject &obj) const
{
    Contact c;
    c.id = obj["id"].toString();
    c.name = obj["name"].toString();
    c.phone = obj["phone"].toString();
    c.company = obj["company"].toString();
    c.position = obj["position"].toString();
    c.email = obj["email"].toString();
    c.address = obj["address"].toString();
    c.birthday = QDate::fromString(obj["birthday"].toString(), Qt::ISODate);
    c.notes = obj["notes"].toString();
    return c;
}