#include "Contact.h"
#include "ContactModel.h"

ContactModel::ContactModel(QObject *parent) : QAbstractTableModel(parent) 
{
}

int ContactModel::rowCount(const QModelIndex &parent) const 
{
    if (parent.isValid())
        return 0;
    return contacts.size();
}

int ContactModel::columnCount(const QModelIndex &parent) const 
{
    if (parent.isValid())
        return 0;
    return Contact::ColumnCount;
}

/**
 * @brief ContactModel::data 获取单元格数据
 * @param index 单元格索引
 * @param role 角色
 * @return QVariant 单元格数据
 */
QVariant ContactModel::data(const QModelIndex &index, int role) const 
{
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    if (index.column() >= Contact::ColumnCount)
        return QVariant();
    if (index.column() == 0)
        return contacts[index.row()].name;
    if (index.column() == 1)
        return contacts[index.row()].phone;
    if (index.column() == 2)
        return contacts[index.row()].company;
    if (index.column() == 3)
        return contacts[index.row()].position;
    if (index.column() == 4)
        return contacts[index.row()].email;
    return QVariant();
}

/**
 * @brief ContactModel::headerData 获取表头数据
 * @param section 表头部分
 * @param orientation 方向
 * @param role 角色
 * @return QVariant 表头数据
 */
QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return Contact::headerLabels().value(section, "未知");
    }
    return QVariant();
}

/**
 * @brief ContactModel::setData 设置单元格数据
 * @param index 单元格索引
 * @param value 新值
 * @param role 角色
 * @return bool 是否成功设置数据
 */
bool ContactModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
    if (!index.isValid())
        return false;
    if (role != Qt::EditRole)
        return false;
    if (index.column() >= Contact::ColumnCount)
        return false;
    if (index.column() == 0)
        contacts[index.row()].name = value.toString();
    if (index.column() == 1)
        contacts[index.row()].phone = value.toString();
    if (index.column() == 2)
        contacts[index.row()].company = value.toString();
    if (index.column() == 3)
        contacts[index.row()].position = value.toString();
    if (index.column() == 4)
        contacts[index.row()].email = value.toString();

    emit dataChanged(index, index);
    return true;
}

/**
 * @brief ContactModel::flags 获取单元格标志
 * @param index 单元格索引
 * @return Qt::ItemFlags 单元格标志
 */
Qt::ItemFlags ContactModel::flags(const QModelIndex &index) const 
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    // 用户不能直接编辑，后续使用对话框来编辑
    return /* Qt::ItemIsEditable | */Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

/**
 * @brief ContactModel::insertRows 在row位置插入count个空行
 * @param row 行索引
 * @param count 行数
 * @param parent 父索引（此处必为无效索引）
 * @return bool 是否成功插入行
 */
bool ContactModel::insertRows(int row, int count, const QModelIndex &parent) 
{
    if (parent.isValid())
        return false;
    if  (row < 0 || row > rowCount(parent))
        return false;
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++) {
        contacts.insert(row + i, Contact());
    }
    endInsertRows();
    return true;
}

/**
 * @brief ContactModel::removeRows 删除row行开始的count行
 * @param row 行索引
 * @param count 行数
 * @param parent 父索引（此处必为无效索引）
 * @return bool 是否成功删除行
 */
bool ContactModel::removeRows(int row, int count, const QModelIndex &parent) 
{
    if (parent.isValid())
        return false;
    if (row < 0 || row + count > rowCount(parent))
        return false;
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++) {
        contacts.removeAt(row);
    }
    endRemoveRows();
    return true;
}

/**
 * @brief ContactModel::addContact 添加联系人
 * @param contact 联系人数据
 */
void ContactModel::addContact(const Contact &contact) 
{
    int row = rowCount();
    insertRows(row, 1, QModelIndex());
    Contact c = contact;
    if (c.id.isEmpty()) {
        c.id = QUuid::createUuid().toString();
    }
    contacts[row] = c;
    emit dataChanged(index(row, 0), index(row, Contact::ColumnCount - 1));
}


/**
 * @brief ContactModel::removeContact 删除联系人
 * @param row 联系人行索引
 */
void ContactModel::removeContact(int row) 
{
    removeRows(row, 1, QModelIndex());
    emit dataChanged(index(row, 0), index(row, Contact::ColumnCount - 1));
}   

/**
 * @brief ContactModel::updateContact 更新联系人
 * @param row 联系人行索引
 * @param contact 联系人数据
 */
void ContactModel::updateContact(int row, const Contact &contact) 
{
    if (row < 0 || row >= rowCount())
        return;
    contacts[row] = contact;
    emit dataChanged(index(row, 0), index(row, Contact::ColumnCount - 1));
}

Contact ContactModel::getContact(int row) const 
{
    if (row < 0 || row >= rowCount())
        return Contact();
    return contacts[row];
}