/*! @File        : TestStandardItemModel.h
 *  @ClassName   : TestStandardItemModel
 *  @OS Env      : Linux/Unix&Windows
 *  @Brief       : briefs
 *  @Details     : details
 *  @Author      : Taiga
 *  @Email       : taigaahri@gmail.com
 *  @Date        : 2023-11-13 02:22:21
 *  @Version     : v1.0
 *
 **********************************************************
 *
 *  @par modified log :
 *  @Attention        :
 *
 *********************************************************/

#ifndef TESTSTANDARDITEMMODEL_H
#define TESTSTANDARDITEMMODEL_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardItemModel>
#include <QItemSelectionModel>

QT_BEGIN_NAMESPACE
namespace Ui { class TestStandardItemModel; }
QT_END_NAMESPACE

#define FIXED_COLUMN_COUNT 6 //文件固定为6列
class TestStandardItemModel : public QMainWindow
{
    Q_OBJECT

private:
    //用于状态栏的信息显示
    QLabel *QLBp_01barCurrentFile_M; //当前文件
    QLabel *QLBp_02barCellPosition_M; //当前单元格的行列号
    QLabel *QLBp_03barCellText_M; //当前单元格内容
    QStandardItemModel *QSIMp_dataModel_M; //数据模型
    QItemSelectionModel *QISMp_selecetionModel_M; //选择模型
    void initModelData(QStringList &aFileContent); //从Stringlist初始化数据模型内容
    void initAll(); //整体初始化

public:
    TestStandardItemModel(QWidget *parent = nullptr);
    ~TestStandardItemModel();

private:
    Ui::TestStandardItemModel *ui;

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
#endif // TESTSTANDARDITEMMODEL_H
