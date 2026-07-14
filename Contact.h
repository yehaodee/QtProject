#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QStringList>
#include <QDate>

struct Contact {
    QString id;
    QString name;
    QString phone;
    QString company;
    QString position;
    QString email;
    QString address;
    QDate birthday;
    QString notes;

    static const int ColumnCount = 5;

    Contact() = default;
    Contact(const QString &id, const QString &name, const QString &phone, 
            const QString &company, const QString &position, const QString &email, 
            const QString &address, const QDate &birthday, const QString &notes)
            : id(id), name(name), phone(phone), company(company), position(position), 
            email(email), address(address), birthday(birthday), notes(notes) {}

    Contact(const QString &name, const QString &phone, const QString &company, 
            const QString &position, const QString &email, const QString &address, 
            const QDate &birthday, const QString &notes)
            : name(name), phone(phone), company(company), position(position), 
            email(email), address(address), birthday(birthday), notes(notes) {}

    static QStringList headerLabels() {
        return {"姓名", "电话", "公司", "职务", "邮箱" };
    }

    static QStringList detailLabels() {
        return {"姓名", "电话", "公司", "职务", "邮箱", "地址", "生日", "备注" };
    }

    bool operator==(const Contact &other) const {
        return id == other.id;
    }
};

#endif // CONTACT_H