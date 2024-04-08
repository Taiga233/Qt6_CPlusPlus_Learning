#include "DoubleSpinBoxDelegate_Custom.h"

#include <QDoubleSpinBox>

DoubleSpinBoxDelegate_Custom::DoubleSpinBoxDelegate_Custom(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *DoubleSpinBoxDelegate_Custom::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QDoubleSpinBox *QDSB_editor_t = new QDoubleSpinBox(parent);
    QDSB_editor_t->setFrame(false); //不需要frame
    QDSB_editor_t->setMinimum(0);
    QDSB_editor_t->setMaximum(10000);
    QDSB_editor_t->setDecimals(2); //设置显示两位小数
    return QDSB_editor_t;
}

void DoubleSpinBoxDelegate_Custom::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qreal qr_value_t = index.data(Qt::EditRole).toReal();
//    qr_value_t = index.model()->data(index, Qt::EditRole).toReal();
    QDoubleSpinBox *QDSBp_spinBox_t = static_cast<QDoubleSpinBox *>(editor);
    QDSBp_spinBox_t->setValue(qr_value_t);
}

void DoubleSpinBoxDelegate_Custom::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    //将代理编辑器大小设置为建议大小
    editor->setGeometry(option.rect);
}

void DoubleSpinBoxDelegate_Custom::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *QDSBp_spinBox_t = static_cast<QDoubleSpinBox *>(editor);
    qreal qr_value_t = QDSBp_spinBox_t->value();
    QString QS_editRoleData = QString::asprintf("%.2f", qr_value_t);
//    model->setData(index, QVariant(QS_editRoleData), Qt::EditRole);
    model->setData(index, QS_editRoleData, Qt::EditRole);
}
