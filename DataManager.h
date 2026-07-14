#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QString>
#include "ContactModel.h"
#include "ContactGroupManager.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(ContactModel *model, ContactGroupManager *manager,
                         const QString &filePath = "contacts.json",
                         QObject *parent = nullptr);

    bool load();
    bool save();

private:
    QJsonObject serialize();
    void deserialize(const QJsonObject &root);

    void serializeContact(const Contact &c, QJsonObject &obj) const;
    Contact deserializeContact(const QJsonObject &obj) const;

    ContactModel *contactModel;
    ContactGroupManager *groupManager;
    QString filePath;
};

#endif // DATAMANAGER_H