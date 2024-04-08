#ifndef COMBOBOXDELEGATE_CUSTOM_H
#define COMBOBOXDELEGATE_CUSTOM_H

#include <QStyledItemDelegate>

class ComboBoxDelegate_Custom : public QStyledItemDelegate
{
    Q_OBJECT

    QStringList QSL_itemList_m; //选项列表
    bool b_editable_m; //是否可编辑
public:
    explicit ComboBoxDelegate_Custom(QObject *parent = nullptr);

    //初始化设置表格内容，是否可编辑
    void setItems(const QStringList &items, const bool &editable);

    //This functions of the custom delegate must be reimplemented:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // COMBOBOXDELEGATE_CUSTOM_H
