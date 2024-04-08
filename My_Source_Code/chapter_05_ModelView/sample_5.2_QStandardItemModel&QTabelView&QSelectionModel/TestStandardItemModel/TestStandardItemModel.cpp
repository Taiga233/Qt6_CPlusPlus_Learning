/*! @File        : TestStandardItemModel.cpp
 *  @Brief       : TestQStandardItemModelAndQItemSelectionModelAndQTableView
 *  @Details     : Test QStandardItemModel QItemSelectionModel QTableView QStandardItem
 *  @Author      : Taiga
 *  @Email       : taigaahri@gmail.com
 *  @Date        : 2023-11-13 00:42:48
 *  @Version     : v1.0
 *
 **********************************************************
 *
 *  @par modified log :
 *  @Attention        :
 *
 *********************************************************/

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QtDebug>

#include "TestStandardItemModel.h"
#include "ui_TestStandardItemModel.h"

#define ACTION_ALIGN_LEFT 1
#define ACTION_ALIGN_CENTER 2
#define ACTION_ALIGN_RIGHT 3

void TestStandardItemModel::initModelData(QStringList &aFileContent)
{
    QSIMp_dataModel_M->setRowCount(aFileContent.size() - 1); //设置模型数据行数
    QString header_t = aFileContent.at(0); //第一行是表头
    //以一个或多个空格、制表符等分隔符隔开的字符串，分割为一个stringList
    static QRegularExpression regularExpression_t("\\s+");
    QStringList headerList_t = header_t.split(regularExpression_t, Qt::SkipEmptyParts);
    qDebug() << "header size:" << headerList_t.size();
    qDebug() << "headerList:" << headerList_t;
    QSIMp_dataModel_M->setHorizontalHeaderLabels(headerList_t);

    //设置表格数据
    QStandardItem *item_t;
    for (auto i = 1; i < aFileContent.size(); i++) {
        QString lineText_t = aFileContent.at(i); //获取一行的数据
        //以一个或多个空格、制表符等分隔符隔开的字符串，分割为一个StringList
        QStringList lineList_t = lineText_t.split(regularExpression_t, Qt::SkipEmptyParts);
        qDebug("第%d行分割的个数：%lld", i, lineList_t.size());
        //不包含最后一列
        for (auto j = 0; j < FIXED_COLUMN_COUNT - 1; j++) {
            item_t = new QStandardItem(lineList_t.at(j));
            QSIMp_dataModel_M->setItem((i - 1), j, item_t);
        }

        //最后一列单独处理
        item_t = new QStandardItem(*(headerList_t.rbegin())); //最后一列文字
        item_t->setCheckable(true); //设置是否可复选
        item_t->setBackground(QBrush(Qt::yellow));
        if (lineList_t.at(FIXED_COLUMN_COUNT - 1).compare("0")) {
            item_t->setCheckState(Qt::Checked);
        } else {
            item_t->setCheckState(Qt::Unchecked);
        }
        QSIMp_dataModel_M->setItem((i - 1), (FIXED_COLUMN_COUNT - 1), item_t);
    }
}

void TestStandardItemModel::initAll()
{
    QSIMp_dataModel_M = new QStandardItemModel(1, FIXED_COLUMN_COUNT, this); //创建数据模型
    QISMp_selecetionModel_M = new QItemSelectionModel(QSIMp_dataModel_M, this); //创建选择模型，并且设置数据模型
    connect(QISMp_selecetionModel_M, &QItemSelectionModel::currentChanged, this, &TestStandardItemModel::do_currentChanged); //关联选择当前单元格变化槽函数
    //为QTableView设置数据模型和选择模型
    ui->TV_01standardItemModel->setModel(QSIMp_dataModel_M);
    ui->TV_01standardItemModel->setSelectionModel(QISMp_selecetionModel_M);
    //selectionMode属性表示在视图组件上选择项的操作模式
    ui->TV_01standardItemModel->setSelectionMode(QAbstractItemView::ExtendedSelection); //扩展选择，如按住Ctrl则可以选择多个不连续的单元格
    //selectionBehavior属性表示点击鼠标时选择操作的行为
    ui->TV_01standardItemModel->setSelectionBehavior(QAbstractItemView::SelectItems); //选择单个项

    //创建状态栏组件
    QLBp_01barCurrentFile_M = new QLabel("当前文件：", this);
    QLBp_01barCurrentFile_M->setMinimumWidth(200);
    QLBp_02barCellPosition_M = new QLabel("当前单元格坐标：", this);
    QLBp_02barCellPosition_M->setMinimumWidth(180);
    QLBp_02barCellPosition_M->setAlignment(Qt::AlignHCenter);
    QLBp_03barCellText_M = new QLabel("当前单元格内容：", this);
    QLBp_03barCellText_M->setMinimumWidth(180);

    ui->statusbar->addWidget(QLBp_01barCurrentFile_M);
    ui->statusbar->addWidget(QLBp_02barCellPosition_M);
    ui->statusbar->addWidget(QLBp_03barCellText_M);

    //关联项的文字对齐方式响应函数
    this->connect(ui->actText_alignCenter, SIGNAL(triggered(bool)), SLOT(do_itemsAlignmentTriggered(bool)));
    connect(ui->actText_alignLeft, &QAction::triggered, this, qOverload<bool>(&TestStandardItemModel::do_itemsAlignmentTriggered));
    connect(ui->actText_alignRight, SIGNAL(triggered(bool)), this, SLOT(do_itemsAlignmentTriggered(bool)));
    ui->actText_alignLeft->setData(QVariant(ACTION_ALIGN_LEFT));
    ui->actText_alignCenter->setData(QVariant(ACTION_ALIGN_CENTER));
    ui->actText_alignRight->setData(QVariant(ACTION_ALIGN_RIGHT));
}

TestStandardItemModel::TestStandardItemModel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestStandardItemModel)
{
    ui->setupUi(this);

    initAll();
}

TestStandardItemModel::~TestStandardItemModel()
{
    delete ui;
}

void TestStandardItemModel::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid()) return; //非法则返回

    QLBp_02barCellPosition_M->setText(QString("当前单元格坐标：").append(QString::asprintf("(%d,%d)", current.row(), current.column())));
    //通过模型索引（index）获得当前点击的项的QStandardItem对象指针
    QStandardItem *item = QSIMp_dataModel_M->itemFromIndex(current);
    QLBp_03barCellText_M->setText(QString("当前单元格内容：").append(item->text()));
    //设置action状态
    ui->actText_fontBold->setChecked(item->font().bold());
}

void TestStandardItemModel::do_itemsAlignmentTriggered(bool checked)
{
    Q_UNUSED(checked);
    QAction *senderAction_t = qobject_cast<QAction *>(sender()); //获取信号的发射者
    Qt::Alignment action_alignment_t = Qt::AlignVCenter;
    switch (senderAction_t->data().toUInt()) {
    case ACTION_ALIGN_LEFT:
        action_alignment_t |= Qt::AlignLeft;
        break;
    case ACTION_ALIGN_CENTER:
        action_alignment_t |= Qt::AlignCenter;
        break;
    case ACTION_ALIGN_RIGHT:
        action_alignment_t |= Qt::AlignRight;
        break;
    default:
        action_alignment_t = Qt::AlignCenter;
        break;
    }

    if (!QISMp_selecetionModel_M->hasSelection()) return; //没有选择的项则返回
    //获取选择的单元格的模型索引列表，单元格可以多选
    QModelIndexList indexList_t =  QISMp_selecetionModel_M->selectedIndexes();
    for (auto index_t : indexList_t) {
        QSIMp_dataModel_M->itemFromIndex(index_t)->setTextAlignment(action_alignment_t);
    }
}


void TestStandardItemModel::on_actFile_open_triggered()
{   //打开文件, 文件需要使用UTF-8编码，否则显示汉字乱码
    //获取应用程序的路径
    QString currentPath = QCoreApplication::applicationDirPath();
    QString fileName = QFileDialog::getOpenFileName(this, "打开一个文件", currentPath, "数据文件(*.txt);;所有文件(*.*)");
    if (fileName.isEmpty()) return;
    QStringList fileContents;
    QFile file_t(fileName);

    //以只读的方式打开文件
    if (file_t.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //用文本流读取文件
        QTextStream stream_t(&file_t);
        ui->PTE_01displayData->clear();
        while (!stream_t.atEnd()) {
            QString line_t = stream_t.readLine(); //读取文件的一行
            ui->PTE_01displayData->appendPlainText(line_t);
            fileContents.append(line_t);
        }
        file_t.close(); //关闭文件
        QLBp_01barCurrentFile_M->setText(QLBp_01barCurrentFile_M->text().append(fileName)); //打开过的所有文件
        ui->actTable_appendRow->setEnabled(true);
        ui->actTable_insertRow->setEnabled(true);
        ui->actTable_deleteRow->setEnabled(true);
        ui->actFile_saveAS->setEnabled(true);
        initModelData(fileContents);
    }
}


void TestStandardItemModel::on_actTable_appendRow_triggered()
{
    QList<QStandardItem *> itemList_t;
    QStandardItem *item_t;
    for (auto i = 0; i < FIXED_COLUMN_COUNT - 1; i++) {
        //最后一列不包括在内
        item_t = new QStandardItem(QString::asprintf("appendItem %d", i));
        itemList_t << item_t; //添加到列表
    }
    //获取最后一列的表头文字，通过数据模型的headerData接口
    QString lastColumnText_t = QSIMp_dataModel_M->headerData(QSIMp_dataModel_M->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
    item_t = new QStandardItem(lastColumnText_t);
    item_t->setCheckable(true);
    item_t->setBackground(QBrush(Qt::green));
    itemList_t << item_t; //添加最后一列

    QSIMp_dataModel_M->appendRow(itemList_t); //在模型的最后添加一行
    QModelIndex currentIndex_t = QSIMp_dataModel_M->index(QSIMp_dataModel_M->rowCount() - 1, 0);
    QISMp_selecetionModel_M->clearSelection(); //清除所有选择会触发selectionChanged信号
    QISMp_selecetionModel_M->setCurrentIndex(currentIndex_t, QItemSelectionModel::Select);
}


void TestStandardItemModel::on_actTable_insertRow_triggered()
{
    QList<QStandardItem *> itemList_t;
    QStandardItem *item_t;
    for (auto i = 0; i < FIXED_COLUMN_COUNT - 1; i++) {
        //最后一列不包括在内
        item_t = new QStandardItem(QString::asprintf("insertItem %d", i));
        itemList_t << item_t; //添加到列表
    }
    //获取最后一列的表头文字，通过数据模型的headerData接口
    QString lastColumnText_t = QSIMp_dataModel_M->headerData(QSIMp_dataModel_M->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
    item_t = new QStandardItem(lastColumnText_t);
    item_t->setCheckable(true);
    item_t->setBackground(QBrush(Qt::blue));
    itemList_t << item_t; //添加最后一列

    QModelIndex currentIndex_t = QISMp_selecetionModel_M->currentIndex();
    //在当前行的前面插入一行
    QSIMp_dataModel_M->insertRow(currentIndex_t.row(), itemList_t);
    QISMp_selecetionModel_M->clearSelection(); //清除所有选择会触发selectionChanged信号
    QISMp_selecetionModel_M->setCurrentIndex(currentIndex_t, QItemSelectionModel::Select);
}


void TestStandardItemModel::on_actTable_deleteRow_triggered()
{
    //获取当前单元格（只能通过选择模型）的模型索引
    QModelIndex currentIndex_t = QISMp_selecetionModel_M->currentIndex();
    if (!currentIndex_t.isValid()) return;
    QSIMp_dataModel_M->removeRow(currentIndex_t.row()); //删除最后一行
    if (currentIndex_t.row() != QSIMp_dataModel_M->rowCount()) {
        //这里表示是删除的行不是最后一行，删除完成后rowCount()会自动更新的
        qDebug() << "deleted row isn't the last row";
        QISMp_selecetionModel_M->clearSelection();
        QISMp_selecetionModel_M->setCurrentIndex(currentIndex_t, QItemSelectionModel::Select); //重新设置当前选择项
    }
}


void TestStandardItemModel::on_actText_fontBold_triggered(bool checked)
{
    if (!QISMp_selecetionModel_M->hasSelection()) return; //没有选择的项则返回
    QItemSelection selection_t = QISMp_selecetionModel_M->selection();
    qDebug() << "selection_t.indexes().count():" << selection_t.indexes().count();
    for (auto index_t : selection_t.indexes()) {
        QStandardItem *item_t = QSIMp_dataModel_M->itemFromIndex(index_t);
        QFont font_t = item_t->font();
        font_t.setBold(checked);
        item_t->setFont(font_t);
    }
}



#undef ACTION_ALIGN_LEFT
#undef ACTION_ALIGN_CENTER
#undef ACTION_ALIGN_RIGHT

void TestStandardItemModel::on_actModel_displayData_triggered()
{
    ui->PTE_01displayData->clear();
    QStandardItem *item_t;
    QString showStr_t;
    for (auto i = 0; i < QSIMp_dataModel_M->columnCount(); i++) {
        item_t = QSIMp_dataModel_M->horizontalHeaderItem(i);
        showStr_t += (item_t->text() + "\t");
    }
    ui->PTE_01displayData->appendPlainText("表头：");
    ui->PTE_01displayData->appendPlainText(showStr_t);

    ui->PTE_01displayData->appendPlainText("内容：");
    for (auto i = 0; i < QSIMp_dataModel_M->rowCount(); i++) {
        showStr_t.clear();
        for (auto j = 0; j < QSIMp_dataModel_M->columnCount() - 1; j++) {
            item_t = QSIMp_dataModel_M->item(i, j);
            showStr_t += (item_t->text() + QString::asprintf("\t"));
        }
        item_t = QSIMp_dataModel_M->item(i, (FIXED_COLUMN_COUNT - 1));
        showStr_t += item_t->checkState() == Qt::Checked ? "1" : "0";
        ui->PTE_01displayData->appendPlainText(showStr_t);
    }
}

