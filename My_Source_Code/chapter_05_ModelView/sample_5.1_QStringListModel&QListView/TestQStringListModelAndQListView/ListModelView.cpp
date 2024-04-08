#include "ListModelView.h"
#include "ui_ListModelView.h"

#include <QDebug>

ListModelView::ListModelView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ListModelView)
{
    ui->setupUi(this);
    QSL_initList_M.clear();
    QSL_initList_M << "北京" << "上海" << "广州" << "深圳" << "杭州" << "成都" << "南京" << "苏州" << "武汉";
    QSLM_p_listModel_M = new QStringListModel(this); //创建数据模型
    QSLM_p_listModel_M->setStringList(QSL_initList_M); //初始化数据
    ui->LV_01->setModel(QSLM_p_listModel_M); //设置数据模型
    ui->CB_01listEditable->setChecked(true);
    ui->LV_01->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

ListModelView::~ListModelView()
{
    delete ui;
}


void ListModelView::on_PB_01recovery_clicked()
{
    QSLM_p_listModel_M->setStringList(QSL_initList_M);
}


void ListModelView::on_PB_02clearList_clicked()
{
    QSLM_p_listModel_M->removeRows(0, QSLM_p_listModel_M->rowCount()); //清除数据模型的所有项
}


void ListModelView::on_PB_03append_clicked()
{
    int indexPosition = QSLM_p_listModel_M->rowCount();
    QSLM_p_listModel_M->insertRow(indexPosition); //在末尾插入一个项
    QModelIndex index = QSLM_p_listModel_M->index(indexPosition); //获取刚插入项的模型索引
    QSLM_p_listModel_M->setData(index, "appendedCityItem", Qt::DisplayRole); //用模型索引定位项，需要指定数据的角色
    ui->LV_01->setCurrentIndex(index); //设置模型索引为index的项 为当前项
}


void ListModelView::on_PB_04insert_clicked()
{
    QModelIndex index = ui->LV_01->currentIndex();
    if (!index.isValid()) return;
    QSLM_p_listModel_M->insertRow(index.row());
    QSLM_p_listModel_M->setData(index, "insertedCityItem", Qt::EditRole); //在这里 EditRole和DisplayRole 好像没啥区别
    ui->LV_01->setCurrentIndex(index); //这样设置的当前项为哪一个呢？项是根据行和列定位的，所以在当前项之前插入的项变成了当前项
}


void ListModelView::on_PB_05delete_clicked()
{
    QSLM_p_listModel_M->removeRow(ui->LV_01->currentIndex().row());
}


void ListModelView::on_PB_06moveUp_clicked()
{
    int currentRow = ui->LV_01->currentIndex().row();
    if (!currentRow) return;
    QSLM_p_listModel_M->moveRow(QModelIndex(), currentRow, QModelIndex(), currentRow - 1);
}


void ListModelView::on_PB_07moveDown_clicked()
{
    int currentRow = ui->LV_01->currentIndex().row();
    qDebug() << "currentRow:" << currentRow;
    if (currentRow == QSLM_p_listModel_M->rowCount() - 1) return;
    QSLM_p_listModel_M->moveRow(QModelIndex(), currentRow, QModelIndex(), ui->LV_01->currentIndex().row() + 2); //注意这里是+2，解释：moveRow() 通过调用 beginMoveRows 向视图发出将要移动行的信号，将源行从源父项中删除，并在目标父项中插入该行。最后，函数通过调用 endMoveRows 通知视图移动已完成，返回操作结果。
    qDebug() << "currentRow:" << ui->LV_01->currentIndex().row();
}


void ListModelView::on_PB_08sort_clicked(bool checked)
{
    QSLM_p_listModel_M->sort(0, (checked ? Qt::AscendingOrder : Qt::DescendingOrder));
}


void ListModelView::on_CB_01listEditable_clicked(bool checked)
{
    if (checked) {
        ui->LV_01->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    } else {
        ui->LV_01->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


void ListModelView::on_LV_01_clicked(const QModelIndex &index)
{
    QString str1 = QString::asprintf("index: row = %d, column = %d", index.row(), index.column());
    QString str2 = QSLM_p_listModel_M->data(index, Qt::DisplayRole).toString();
    ui->statusbar->showMessage(str1 + str2);
}


void ListModelView::on_PB_10showModelStringlist_clicked()
{
    QStringList stringList = QSLM_p_listModel_M->stringList();
    ui->PTE_01->setPlainText("String List Data:");
    for (auto str : stringList) {
        ui->PTE_01->appendPlainText(str);
    }
}

