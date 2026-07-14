#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "Contact.h"

class ContactDialog : public QDialog {
    Q_OBJECT
public:
    explicit ContactDialog(QWidget *parent = nullptr);
    explicit ContactDialog(const Contact &contact, QWidget *parent = nullptr);

    Contact getContact() const;

private:
    void initUI();

    QString originalId;
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *companyEdit;
    QLineEdit *positionEdit;
    QLineEdit *emailEdit;
    QLineEdit *addressEdit;
    QDateEdit *birthdayEdit;
    QLineEdit *notesEdit;
};

#endif // CONTACTDIALOG_H
