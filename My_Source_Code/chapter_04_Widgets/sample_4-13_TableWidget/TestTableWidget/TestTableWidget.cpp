#include "TestTableWidget.h"
#include "ui_TestTableWidget.h"

#include <QRandomGenerator>
#include <QDebug>

void TestTableWidget::buildCustomWidget()
{
    LB_cellIndex = new QLabel("当前单元格坐标", this);
    LB_cellIndex->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    LB_cellType = new QLabel("当前单元格类型", this);
    LB_cellType->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    LB_statusBarID = new QLabel("学生ID：", this);
    LB_statusBarID->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->statusbar->addWidget(LB_cellIndex);
    ui->statusbar->addWidget(LB_cellType);
    ui->statusbar->addWidget(LB_statusBarID);
}

void TestTableWidget::createItemsForRow(int rowNo, QString name, QString sex, QDate birth, QString nation, bool isParty, int score)
{
    QTableWidgetItem *item;
    Qt::Alignment alignmentFlags = Qt::AlignHCenter | Qt::AlignVCenter;
    Qt::ItemFlags itemFlags = Qt::NoItemFlags;

    //E_name_CT
    {
        uint studentID = 160302001;
        item = new QTableWidgetItem(name, TestTableWidget::E_name_CT);
        //item->setTextAlignment(Qt::AlignCenter);
        item->setTextAlignment(alignmentFlags);
        studentID += rowNo;
        item->setData(Qt::UserRole, QVariant(studentID)); //设置学号为用户数据
        ui->TW_people->setItem(rowNo, TestTableWidget::E_01name_FCN, item);
    }

    //E_sex_CT
    {
        QIcon icon;
        if (sex == "男") {
            icon.addFile("://icons/boy.ico");
        } else {
            icon.addFile("://icons/girl.ico");
        }
        item = new QTableWidgetItem(sex, TestTableWidget::E_sex_CT);
        item->setIcon(icon);
        item->setTextAlignment(alignmentFlags);
        itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        item->setFlags(itemFlags); //性别单元格不允许编辑
        ui->TW_people->setItem(rowNo, E_02sex_FCN, item);
    }

    //E_birth_CT
    {
        QString strBirth = birth.toString("yyyy-MM-dd");
        item = new QTableWidgetItem(strBirth, TestTableWidget::E_birth_CT);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->TW_people->setItem(rowNo, E_03birth_FCN, item);
    }

    //E_nation_CT
    {
        item = new QTableWidgetItem(nation, TestTableWidget::E_nation_CT);
        item->setTextAlignment(alignmentFlags);
        ui->TW_people->setItem(rowNo, E_04nation_FCN, item);
    }

    //E_score_CT
    {
        item = new QTableWidgetItem(QString::number(score), TestTableWidget::E_score_CT);
        item->setTextAlignment(alignmentFlags);
        ui->TW_people->setItem(rowNo, E_05score_FCN, item);
    }

    //E_party_CT
    {
        item = new QTableWidgetItem("党员", TestTableWidget::E_party_CT);
        item->setTextAlignment(alignmentFlags);
        itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        item->setFlags(itemFlags); //单元格不允许编辑，但是可以更改复选状态
        item->setCheckState((isParty ? Qt::Checked : Qt::Unchecked));
        item->setBackground(QBrush(Qt::yellow)); //设置背景颜色
        ui->TW_people->setItem(rowNo, E_06party_FCN, item);
    }
}

TestTableWidget::TestTableWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestTableWidget)
{
    ui->setupUi(this);
    buildCustomWidget();
}

TestTableWidget::~TestTableWidget()
{
    delete ui;
}


void TestTableWidget::on_PB_01setHorizontalHeader_clicked()
{
    QStringList headerText;
    headerText << "姓名" << "性别" << "出生日期" << "民族" << "分数" << "是否党员";
//    ui->TW_people->setHorizontalHeaderLabels(headerText); //只设置水平表头的标题文字
    ui->TW_people->setColumnCount(headerText.size()); //设置表格列数
    for (auto i = 0; i < ui->TW_people->columnCount(); i++) {
        QTableWidgetItem *header = new QTableWidgetItem(headerText.at(i), TestTableWidget::E_header_CT);
        QFont font = header->font();
        font.setBold(true);
        font.setFamily("Consolas");
        font.setPointSize(14);
        header->setForeground(QBrush(Qt::red)); //设置文字颜色
        header->setFont(font);
        ui->TW_people->setHorizontalHeaderItem(i, header); //设置表头单元格的item
    }
}


void TestTableWidget::on_PB_02setRowCount_clicked()
{
    ui->TW_people->setRowCount(ui->SB_rowCount->value());
    ui->TW_people->setAlternatingRowColors(ui->CB_02intervalRowBackground->isChecked());
}


void TestTableWidget::on_PB_03initalizeTable_clicked()
{
    QDate birth(2001, 4, 13);
    ui->TW_people->clearContents(); //只清空工作区，不清空表头
    ui->PB_02setRowCount->click();
//    emit ui->PB_02setRowCount->clicked();
//    on_PB_02setRowCount_clicked(); //设置表格行数

    for (auto i = 0; i < ui->TW_people->rowCount(); i++) {
        QString name("学生%1");
        name = name.arg(i + 1);
        QString sex = ((i % 2) == 0) ? "男" : "女";
        bool isParty = (i & 1);
        int score = QRandomGenerator::global()->bounded(60, 101); //include 60, exclude 101,随机数范围为[60,101)
        createItemsForRow(i, name, sex, birth, "汉族", isParty, score);
        birth = birth.addDays(score);
    }
}


void TestTableWidget::on_PB_04insertRow_clicked()
{
    int currentRowNumber = ui->TW_people->currentRow();
    qDebug() << "current row" << currentRowNumber;
    if (!~currentRowNumber) return; //如果没选择当前行则返回
    ui->TW_people->insertRow(currentRowNumber); //在当前行下插入一行，但是不会自动为单元格创建item
    createItemsForRow(currentRowNumber, "新学生", "男", QDate::fromString("2023.2.3", "yyyy.M.d"), "满族", true, 80);
}


void TestTableWidget::on_PB_05appendRow_clicked()
{
    auto appendRowNumber = ui->TW_people->rowCount();
    //需要保存下来，否则insertRow后ui->TW_people->rowCount()会发生变化
    ui->TW_people->insertRow(appendRowNumber); //在表格末尾添加一行
    createItemsForRow(appendRowNumber, "新生", "女", QDate::fromString("2005.2.3", "yyyy.M.d"), "苗族", false, 100);
}


void TestTableWidget::on_PB_06deleteCurrentRow_clicked()
{
    if (!~ui->TW_people->currentRow()) return;
    ui->TW_people->removeRow(ui->TW_people->currentRow());
}


void TestTableWidget::on_PB_07autoRowHeight_clicked()
{
    qDebug() << "on_PB_07autoRowHeight_clicked()";
//    ui->TW_people->resizeRowsToContents();
}


void TestTableWidget::on_PB_08autoColumnWidth_clicked()
{
    qDebug() << "on_PB_08autoColumnWidth_clicked()";
//    ui->TW_people->resizeColumnsToContents();
}


void TestTableWidget::on_PB_09readContentsToTextEdit_clicked()
{
    ui->PTE_infomation->clear();
    if (!ui->TW_people->rowCount() || !ui->TW_people->columnCount()) return;
    QString str;
    for (int i = 0; i < ui->TW_people->rowCount(); ++i) {
        str = QString::asprintf("第%d行:", (i + 1));
        for (int j = 0; j < ui->TW_people->columnCount() - 1; ++j) {
            str += ui->TW_people->item(i, j)->text() + "  ";
        }
        //最后一列单独处理
        if (ui->TW_people->item(i, E_06party_FCN)->checkState() == Qt::Checked) {
            str += "党员";
        } else {
            str += "群众";
        }
        ui->PTE_infomation->appendPlainText(str);
    }
}


void TestTableWidget::on_TW_people_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    qDebug() << "on_TW_people_currentCellChanged";
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    //    QTableWidgetItem *currentItem = ui->TW_people->currentItem();
    QTableWidgetItem *item = ui->TW_people->item(currentRow, currentColumn);
    if (item == Q_NULLPTR) return;
    LB_cellIndex->setText(QString::asprintf("当前单元格坐标：(%d,%d)", currentRow, currentColumn));
    LB_cellType->setText(QString::asprintf("当前单元格格式：%d", item->type()));

    item = ui->TW_people->item(currentRow, TestTableWidget::E_01name_FCN); //获取当前行姓名列的item
    auto id = item->data(Qt::UserRole).toUInt();
    LB_statusBarID->setText(QString::asprintf("学生ID：%u", id));
}


void TestTableWidget::on_CB_01tableEditable_clicked(bool checked)
{
    if (checked) {
        //双击或获取焦点后点击 可进入编辑状态
        ui->TW_people->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    } else {
        ui->TW_people->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


void TestTableWidget::on_CB_02intervalRowBackground_clicked(bool checked)
{
    ui->TW_people->setAlternatingRowColors(checked); //行的底色用不同颜色交替显示
}


void TestTableWidget::on_CB_03horizontalVisible_clicked(bool checked)
{
    ui->TW_people->horizontalHeader()->setVisible(checked);
}


void TestTableWidget::on_CB_04verticalVisible_clicked(bool checked)
{
    ui->TW_people->verticalHeader()->setVisible(checked);
}


void TestTableWidget::on_RB_01selectRow_clicked()
{
    ui->TW_people->setSelectionBehavior(QAbstractItemView::SelectRows);
}


void TestTableWidget::on_RB_02selectCell_clicked()
{
    ui->TW_people->setSelectionBehavior(QAbstractItemView::SelectItems);
}

