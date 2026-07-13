#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QStringList>
#include <QDate>

struct Contact {
    QString name;
    QString phone;
    QString company;
    QString position;
    QString email;
    QString address;
    QDate birthday;
    QString notes;

    static const int ColumnCount = 8;

    Contact() = default;
    Contact(const QString &name, const QString &phone, const QString &company, 
            const QString &position, const QString &email, const QString &address, 
            const QDate &birthday, const QString &notes)
            : name(name), phone(phone), company(company), position(position), 
            email(email), address(address), birthday(birthday), notes(notes) {}

    static QStringList headerLabels() {
        return {"姓名", "电话", "公司", "职务", "邮箱", "地址", "生日", "备注" };
    }
};

#endif // CONTACT_H