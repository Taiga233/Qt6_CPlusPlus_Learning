#ifndef TESTTABLEWIDGET_H
#define TESTTABLEWIDGET_H

#include <QMainWindow>
#include <QLabel>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class TestTableWidget; }
QT_END_NAMESPACE

#define Private_Data private
#define Private_Func private
class TestTableWidget : public QMainWindow
{
    Q_OBJECT

Private_Data:
    //自定义单元格Type的类型，在创建单元格的Item时使用
    enum CellType{E_name_CT = 1001, E_sex_CT, E_birth_CT, E_nation_CT, E_score_CT, E_party_CT, E_header_CT}; //各单元格的类型
    enum FieldColumnNumber{E_01name_FCN = 0, E_02sex_FCN, E_03birth_FCN, E_04nation_FCN, E_05score_FCN, E_06party_FCN}; //各字段在表格中的列号
    QLabel *LB_cellIndex; //状态栏上用于显示单元格的行号、列号
    QLabel *LB_cellType; //状态栏上用于显示单元格的type
    QLabel *LB_statusBarID; //状态栏上用于显示单元格的data

Private_Func:
    //创建自定义的控件
    void buildCustomWidget();
    //为某一行创建items
    void createItemsForRow(int rowNo, QString name, QString sex, QDate birth, QString nation, bool isParty, int score);

public:
    TestTableWidget(QWidget *parent = nullptr);
    ~TestTableWidget();

private slots:
    void on_PB_01setHorizontalHeader_clicked();

    void on_PB_02setRowCount_clicked();

    void on_PB_03initalizeTable_clicked();

    void on_PB_04insertRow_clicked();

    void on_PB_05appendRow_clicked();

    void on_PB_06deleteCurrentRow_clicked();

    void on_PB_07autoRowHeight_clicked();

    void on_PB_08autoColumnWidth_clicked();

    void on_PB_09readContentsToTextEdit_clicked();

    void on_TW_people_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_CB_01tableEditable_clicked(bool checked);

    void on_CB_02intervalRowBackground_clicked(bool checked);

    void on_CB_03horizontalVisible_clicked(bool checked);

    void on_CB_04verticalVisible_clicked(bool checked);

    void on_RB_01selectRow_clicked();

    void on_RB_02selectCell_clicked();

private:
    Ui::TestTableWidget *ui;
};
#undef Private_Data
#undef Private_Func
#endif // TESTTABLEWIDGET_H
