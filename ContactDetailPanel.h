#ifndef CONTACTDETAILPANEL_H
#define CONTACTDETAILPANEL_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include "Contact.h"

class ContactDetailPanel : public QWidget {
    Q_OBJECT
public:
    explicit ContactDetailPanel(QWidget *parent = nullptr);

    void updateContact(const Contact &contact);

private:
    void setupUI();
    void initLabels(QLabel *label);
    void wrapText(QLabel *label, QString longText);

    const int maxLabelWidth = 300;

    QLabel *nameLabel;
    QLabel *phoneLabel;
    QLabel *companyLabel;
    QLabel *positionLabel;
    QLabel *emailLabel;
    QLabel *addressLabel;
    QLabel *birthdayLabel;
    QLabel *notesLabel;
};

#endif // CONTACTDETAILPANEL_H
