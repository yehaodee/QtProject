#include "ContactDetailPanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFontMetrics>

ContactDetailPanel::ContactDetailPanel(QWidget *parent) : QWidget(parent) {
    setupUI();
    setFixedWidth(320);
}

void ContactDetailPanel::setupUI() {
    QGroupBox *groupBox = new QGroupBox("联系人详情", this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    
    nameLabel = new QLabel(groupBox);
    initLabels(nameLabel);

    phoneLabel = new QLabel(groupBox);
    initLabels(phoneLabel);

    companyLabel = new QLabel(groupBox);
    initLabels(companyLabel);

    positionLabel = new QLabel(groupBox);
    initLabels(positionLabel);

    emailLabel = new QLabel(groupBox);
    initLabels(emailLabel);

    addressLabel = new QLabel(groupBox);
    initLabels(addressLabel);

    birthdayLabel = new QLabel(groupBox);
    initLabels(birthdayLabel);

    notesLabel = new QLabel(groupBox);
    notesLabel->setWordWrap(true);
    notesLabel->setMaximumWidth(maxLabelWidth);
    notesLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    layout->addWidget(nameLabel);
    layout->addWidget(phoneLabel);
    layout->addWidget(companyLabel);
    layout->addWidget(positionLabel);
    layout->addWidget(emailLabel);
    layout->addWidget(addressLabel);
    layout->addWidget(birthdayLabel);
    layout->addWidget(notesLabel);
    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    groupBox->setLayout(layout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);
}

void ContactDetailPanel::initLabels(QLabel *label) {
    label->setWordWrap(true);
    label->setMaximumWidth(maxLabelWidth); 
    // 普通字段固定高度，为备注字段腾出位置
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

/**
 * @brief ContactDetailPanel::wrapText 为标签设置自动换行后的文本
 * @param label 要设置文本的标签
 * @param longText 要设置的长文本，需要换行显示
 * @brief 为标签设置换行后的文本
 * 
 * 这个函数会根据标签的最大宽度，将长文本换行，确保文本在标签内显示。
 */
void ContactDetailPanel::wrapText(QLabel *label, QString longText) {
    QFontMetrics fm(label->font());
    QString wrappedText;
    int currentWidth = 0;

    for (int i = 0; i < longText.length(); ++i) {
        QChar ch = longText[i];
        int charWidth = fm.horizontalAdvance(ch);

        // 如果加上下一个字符会超宽，插入换行符
        if (currentWidth + charWidth > maxLabelWidth && currentWidth > 0) {
            wrappedText.append('\n');
            currentWidth = 0;
        }
        wrappedText.append(ch);
        currentWidth += charWidth;
    }
    label->setText(wrappedText);
}

void ContactDetailPanel::updateContact(const Contact &contact) {
    wrapText(nameLabel, "姓名：" + (contact.name.isEmpty() ? "-" : contact.name));
    wrapText(phoneLabel, "电话：" + (contact.phone.isEmpty() ? "-" : contact.phone));
    wrapText(companyLabel, "公司：" + (contact.company.isEmpty() ? "-" : contact.company));
    wrapText(positionLabel, "职务：" + (contact.position.isEmpty() ? "-" : contact.position));
    wrapText(emailLabel, "邮箱：" + (contact.email.isEmpty() ? "-" : contact.email));
    wrapText(addressLabel, "地址：" + (contact.address.isEmpty() ? "-" : contact.address));
    wrapText(birthdayLabel, "生日：" + (contact.birthday.isValid() ? contact.birthday.toString("yyyy-MM-dd") : "-"));
    wrapText(notesLabel, "备注：" + (contact.notes.isEmpty() ? "-" : contact.notes));
}
