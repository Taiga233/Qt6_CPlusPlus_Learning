/*! @File        : TestQStyledItemDelegate.h
 *  @ClassName   : TestQStyledItemDelegate
 *  @OS Env      : Linux/Unix&Windows
 *  @Brief       : briefs
 *  @Details     : details
 *  @Author      : Taiga
 *  @Email       : taigaahri@gmail.com
 *  @Date        : 2023-11-23 18:19s
 *  @Version     : v1.0
 *
 **********************************************************
 *
 *  @par modified log :
 *  @Attention        :
 *
 *********************************************************/

#ifndef TESTQSTYLEDITEMDELEGATE_H
#define TESTQSTYLEDITEMDELEGATE_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "DoubleSpinBoxDelegate_Custom.h"
#include "SpinBoxDelegate_Custom.h"
#include "ComboBoxDelegate_Custom.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestQStyledItemDelegate; }
QT_END_NAMESPACE

#define FIXED_COLUMN_COUNT 6 //文件固定为6列
class TestQStyledItemDelegate : public QMainWindow
{
    Q_OBJECT

public:
    TestQStyledItemDelegate(QWidget *parent = nullptr);
    ~TestQStyledItemDelegate();

private:
    //用于状态栏的信息显示
    QLabel *QLBp_01barCurrentFile_M; //当前文件
    QLabel *QLBp_02barCellPosition_M; //当前单元格的行列号
    QLabel *QLBp_03barCellText_M; //当前单元格内容
    QStandardItemModel *QSIMp_dataModel_M; //数据模型
    QItemSelectionModel *QISMp_selecetionModel_M; //选择模型
    void initModelData(QStringList &aFileContent); //从Stringlist初始化数据模型内容
    void initAll(); //整体初始化
    void setCustomDelegate(); //设置自定义的代理模型

private:
    Ui::TestQStyledItemDelegate *ui;
    //三个自定义代理的对象：
    DoubleSpinBoxDelegate_Custom *C_01float_DSBDp_m; //用于编辑浮点数的自定义代理
    SpinBoxDelegate_Custom *C_02int_SBDp_m; //用于编辑整数的自定义代理对象
    ComboBoxDelegate_Custom *C_03combo_CBDp_m; //用于列表选择的自定义代理

private slots:
    //自定义槽函数，与QItemSelectionModel的currentChanged()信号连接
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void do_itemsAlignmentTriggered(bool checked); //选择的项的文字对齐方式

private slots:
    void on_actFile_open_triggered();
    void on_actTable_appendRow_triggered();
    void on_actTable_insertRow_triggered();
    void on_actTable_deleteRow_triggered();
    void on_actText_fontBold_triggered(bool checked);
    void on_actModel_displayData_triggered();
};
#undef FixedColumnCount
#endif // TESTQSTYLEDITEMDELEGATE_H
