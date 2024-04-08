#ifndef TESTLISTWIDGETTOOLBUTTON_H
#define TESTLISTWIDGETTOOLBUTTON_H

#include <QMainWindow>
#include <QMenu>
#include <QToolButton>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class TestListWidgetToolButton;}
QT_END_NAMESPACE

#define PRIVATE_FUNS private
#define PRIVATE_WIDGETS private
class TestListWidgetToolButton : public QMainWindow
{
    Q_OBJECT

public:
    TestListWidgetToolButton(QWidget *parent = nullptr);
    ~TestListWidgetToolButton();

PRIVATE_FUNS:
    void setActionForToolButtion();
    void createSelectionMenu();

PRIVATE_WIDGETS:
    QMenu *popupMenu; //下拉菜单
    QToolButton *TB_00ItemSelection; //工具栏中的项选择下拉菜单工具按钮

private slots:
    void on_actList_Init_triggered();

    void on_actList_Clear_triggered();

    void on_actList_Insert_triggered();

    void on_actList_Append_triggered();

    void on_actList_Delete_triggered();

    void on_actSelect_All_triggered();

    void on_actSelect_None_triggered();

    void on_actSelect_Inverse_triggered();

    void on_CB_02Sortable_clicked(bool checked);

    void on_TB_10Ascending_clicked();

    void on_TB_11Descending_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_listWidget_itemSelectionChanged();

    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_itemEntered(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemPressed(QListWidgetItem *item);

    void on_CB_03MouseTracking_clicked(bool checked);

    void on_TB_13AppendSpace_clicked();

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::TestListWidgetToolButton *ui;
};
#undef PRIVATE_FUNS
#undef PRIVATE_WIDGETS
#endif // TESTLISTWIDGETTOOLBUTTON_H
