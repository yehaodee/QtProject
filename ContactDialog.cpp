#include "ContactDialog.h"

ContactDialog::ContactDialog(QWidget *parent) : QDialog(parent) {
    initUI();
}

ContactDialog::ContactDialog(const Contact &contact, QWidget *parent) : QDialog(parent) {
    initUI();
    nameEdit->setText(contact.name);
    phoneEdit->setText(contact.phone);
    companyEdit->setText(contact.company);
    positionEdit->setText(contact.position);
    emailEdit->setText(contact.email);
    addressEdit->setText(contact.address);
    birthdayEdit->setDate(contact.birthday);
    notesEdit->setText(contact.notes);
}

void ContactDialog::initUI() {
    setWindowTitle("编辑联系人");

    nameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    companyEdit = new QLineEdit(this);
    positionEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    birthdayEdit = new QDateEdit(this);
    birthdayEdit->setDisplayFormat("yyyy-MM-dd");
    birthdayEdit->setCalendarPopup(true);
    notesEdit = new QLineEdit(this);

    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("姓名:", nameEdit);
    formLayout->addRow("电话:", phoneEdit);
    formLayout->addRow("公司:", companyEdit);
    formLayout->addRow("职务:", positionEdit);
    formLayout->addRow("邮箱:", emailEdit);
    formLayout->addRow("地址:", addressEdit);
    formLayout->addRow("生日:", birthdayEdit);
    formLayout->addRow("备注:", notesEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    formLayout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(formLayout);
    resize(400, 350);
}

Contact ContactDialog::getContact() const {
    return Contact(
        nameEdit->text(),
        phoneEdit->text(),
        companyEdit->text(),
        positionEdit->text(),
        emailEdit->text(),
        addressEdit->text(),
        birthdayEdit->date(),
        notesEdit->text()
    );
}
