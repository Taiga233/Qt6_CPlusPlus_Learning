#ifndef SPINBOXDELEGATE_CUSTOM_H
#define SPINBOXDELEGATE_CUSTOM_H

#include <QStyledItemDelegate>

class SpinBoxDelegate_Custom : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SpinBoxDelegate_Custom(QObject *parent = nullptr);
    //This functions of the custom delegate must be reimplemented:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // SPINBOXDELEGATE_CUSTOM_H
