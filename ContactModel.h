#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QUuid>
#include "Contact.h"

/**
 * @brief 联系人Model类
 * 用于表示联系人数据的模型类，继承自QAbstractTableModel。
 * 用于构建Model/View架构。
 */
class ContactModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ContactModel(QObject *parent = nullptr);

    // 必须实现的QAbstractTableModel接口方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // 对外公开的操作联系人的方法
    void addContact(const Contact &contact);
    void removeContact(int row);
    void updateContact(int row, const Contact &contact);
    Contact getContact(int row) const;
    QList<Contact> getAllContacts() const;
    void setAllContacts(const QList<Contact> &newContacts);

private:
    QList<Contact> contacts;
};

#endif // CONTACTMODEL_H