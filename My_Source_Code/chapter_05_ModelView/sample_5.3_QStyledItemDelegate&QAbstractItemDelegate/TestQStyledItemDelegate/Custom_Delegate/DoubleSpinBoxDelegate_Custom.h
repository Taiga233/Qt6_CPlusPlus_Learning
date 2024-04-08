#ifndef DOUBLESPINBOXDELEGATE_CUSTOM_H
#define DOUBLESPINBOXDELEGATE_CUSTOM_H

#include <QStyledItemDelegate>

class DoubleSpinBoxDelegate_Custom : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DoubleSpinBoxDelegate_Custom(QObject *parent = nullptr);
    //It is possible for a custom delegate to provide editors without the use of an editor item factory.
    //This functions of the custom delegate must be reimplemented:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // DOUBLESPINBOXDELEGATE_CUSTOM_H
