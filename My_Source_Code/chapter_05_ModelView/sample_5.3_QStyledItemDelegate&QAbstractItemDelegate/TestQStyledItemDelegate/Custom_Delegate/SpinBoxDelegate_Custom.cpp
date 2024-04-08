#include "SpinBoxDelegate_Custom.h"

#include <QSpinBox>

SpinBoxDelegate_Custom::SpinBoxDelegate_Custom(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *SpinBoxDelegate_Custom::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QSpinBox *QSB_editor_t = new QSpinBox(parent);
    QSB_editor_t->setFrame(false); //不需要frame
    QSB_editor_t->setMinimum(0);
    QSB_editor_t->setMaximum(10000);
    return QSB_editor_t;
}

void SpinBoxDelegate_Custom::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int i_value_t = index.data(Qt::EditRole).toInt();
    //    i_value_t = index.model()->data(index, Qt::EditRole).toInt();
    qDebug() << "01:" << index.data(Qt::EditRole);
    qDebug() << "02:" << index.model()->data(index, Qt::EditRole);
    QSpinBox *QSBp_spinBox_t = static_cast<QSpinBox *>(editor);
    QSBp_spinBox_t->setValue(i_value_t);
}

void SpinBoxDelegate_Custom::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    //将代理编辑器大小设置为建议大小
    editor->setGeometry(option.rect);
}

void SpinBoxDelegate_Custom::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *QSBp_spinBox_t = static_cast<QSpinBox *>(editor);
    QString QS_editRoleData_t;
    QS_editRoleData_t.setNum(QSBp_spinBox_t->value());
    model->setData(index, QS_editRoleData_t, Qt::EditRole);
}
