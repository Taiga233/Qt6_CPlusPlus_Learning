#ifndef LISTMODELVIEW_H
#define LISTMODELVIEW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class ListModelView; }
QT_END_NAMESPACE

#ifndef DATA
#define DATA
class ListModelView : public QMainWindow
{
    Q_OBJECT

public:
    ListModelView(QWidget *parent = nullptr);
    ~ListModelView();

private slots:
    void on_PB_01recovery_clicked();

    void on_PB_02clearList_clicked();

    void on_PB_03append_clicked();

    void on_PB_04insert_clicked();

    void on_PB_05delete_clicked();

    void on_PB_06moveUp_clicked();

    void on_PB_07moveDown_clicked();

    void on_PB_08sort_clicked(bool checked);

    void on_CB_01listEditable_clicked(bool checked);

    void on_LV_01_clicked(const QModelIndex &index);

    void on_PB_10showModelStringlist_clicked();

private:
    Ui::ListModelView *ui;

private DATA:
    QStringList QSL_initList_M; //保存初始字符串列表的内容
    QStringListModel *QSLM_p_listModel_M; //数据模型
};
#endif
#undef DATA
#endif // LISTMODELVIEW_H
