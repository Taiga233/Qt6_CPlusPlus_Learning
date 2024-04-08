#include "ComboBoxDelegate_Custom.h"

#include <QComboBox>

ComboBoxDelegate_Custom::ComboBoxDelegate_Custom(QObject *parent)
    : QStyledItemDelegate{parent}
{
    this->QSL_itemList_m.clear();
    this->b_editable_m = false;
}

void ComboBoxDelegate_Custom::setItems(const QStringList &items, const bool &editable)
{
    this->QSL_itemList_m = items;
    this->b_editable_m = editable;
}

QWidget *ComboBoxDelegate_Custom::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *QCBp_editor_t = new QComboBox(parent);
    QCBp_editor_t->setEditable(b_editable_m);
    QCBp_editor_t->addItems(QSL_itemList_m);
    return QCBp_editor_t;
}

void ComboBoxDelegate_Custom::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString QS_editRoleData_t = index.data(Qt::EditRole).toString();
    QComboBox *QCBp_editor_t = static_cast<QComboBox *>(editor);
    auto i = 0;
    for (i = 0; i < QCBp_editor_t->count(); i++) {
        if (QS_editRoleData_t == QCBp_editor_t->itemText(i)) {
            QCBp_editor_t->setCurrentIndex(i);
            break;
        }
    }
    if (i == QCBp_editor_t->count()) {
        QCBp_editor_t->addItem(QS_editRoleData_t);
        QCBp_editor_t->setCurrentText(QS_editRoleData_t);
    }
}

void ComboBoxDelegate_Custom::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    //将代理编辑器大小设置为建议大小
    editor->setGeometry(option.rect);
}

void ComboBoxDelegate_Custom::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QString QS_editRoleData = static_cast<QComboBox *>(editor)->currentText();
    model->setData(index, QS_editRoleData, Qt::EditRole);
}
