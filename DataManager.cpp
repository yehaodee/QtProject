#include "DataManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QDebug>

DataManager::DataManager(ContactModel *model, ContactGroupManager *manager,
                         RecentContactManager *recentManager,
                         const QString &filePath, QObject *parent)
    : QObject(parent), contactModel(model), groupManager(manager),
      recentManager(recentManager), filePath(filePath)
{
    saveTimer.setSingleShot(true);
    saveTimer.setInterval(500);
    connect(&saveTimer, &QTimer::timeout, this, &DataManager::performSave);

    if (contactModel) {
        connect(contactModel, &QAbstractItemModel::rowsInserted,
                this, &DataManager::scheduleSave);
        connect(contactModel, &QAbstractItemModel::rowsRemoved,
                this, &DataManager::scheduleSave);
        connect(contactModel, &QAbstractItemModel::dataChanged,
                this, &DataManager::scheduleSave);
    }

    if (groupManager) {
        connect(groupManager, &ContactGroupManager::groupAdded,
                this, &DataManager::scheduleSave);
        connect(groupManager, &ContactGroupManager::groupRemoved,
                this, &DataManager::scheduleSave);
        connect(groupManager, &ContactGroupManager::membershipChanged,
                this, &DataManager::scheduleSave);
    }

    if (recentManager) {
        connect(recentManager, &RecentContactManager::recentContactsChanged,
                this, &DataManager::scheduleSave);
    }
}

void DataManager::scheduleSave()
{
    saveTimer.start();
}

void DataManager::performSave()
{
    save();
}

/**
 * @brief DataManager::save 保存数据到json文件
 * @return true 如果保存成功，否则返回 false
 */
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

/**
 * @brief DataManager::load 从json文件加载数据
 * @return true 如果加载成功，否则返回 false
 */
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

/**
 * @brief DataManager::serialize 序列化数据为json对象
 * @return QJsonObject 包含联系人和分组的json对象
 */
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

    // 序列化最近联系人
    if (recentManager) {
        root["recentContacts"] = recentManager->saveToJson();
    }

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

    // 反序列化最近联系人
    if (recentManager) {
        recentManager->loadFromJson(root["recentContacts"].toArray());
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