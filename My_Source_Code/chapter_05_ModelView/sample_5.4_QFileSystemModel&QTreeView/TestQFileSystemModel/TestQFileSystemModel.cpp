/*! **********************************************************
 *  @File        :
 *  @Brief       : xxxxx
 *  @Details     : xxxxx
 *  @Author      : Taiga
 *  @Email       : taigaahri@gmail.com
 *  @Date        : 2023-12-12 19:41:45
 *  @Version     : v1.0
 *  @Environment : D:VMwareVM_programbin;C:WINDOWSsystem32;C:WINDOWS;C:WINDOWSSystem32Wbem;C:WINDOWSSystem32WindowsPowerShellv1.0;C:WINDOWSSystem32OpenSSH;L:Xftp_6;L:Xshell_7_Personal;C:Program Filesdotnet;E:BandiZIP;C:Userspyr28AppDataLocalMicrosoftWindowsApps;
 *
 **************************************************************
 *
 *  @par modified log :
 *  @Attention        :
 *
 * This file is part of the QtWidgets module of the Qt Toolkit.
 * Copyright (C) 2023 The Qt Company Ltd.
 * Commercial License Usage
 * Licensees holding valid commercial Qt licenses may use
 * this file in accordance with the commercial license
 * agreement provided with the Software or, alternatively,
 * in accordance with the terms contained in a written
 * agreement between you and The Qt Company. For licensing
 * terms and conditions see
 * https://www.qt.io/terms-conditions.
 * For further information use the contact form at
 * https://www.qt.io/contact-us.
 *************************************************************/
#include "TestQFileSystemModel.h"
#include "ui_TestQFileSystemModel.h"

#include <QFileDialog>

void TestQFileSystemModel::initAll()
{
    //将分割布局的 垂直方向尺寸策略 设置为扩展
    ui->Sh_01horizontal->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //构建模型/视图结构
    QFSMp_model_m = new QFileSystemModel(this);
    QFSMp_model_m->setRootPath(QDir::currentPath()); //设置根目录
    ui->TV_01mainTreeView->setModel(QFSMp_model_m); //设置数据模型
    ui->LV_01mainListView->setModel(QFSMp_model_m);
    ui->TBV_01mainTableView->setModel(QFSMp_model_m);

    //信号槽关联，点击treeView的一个节点时，此节点设置为listView和tableView的根节点
    connect(ui->TV_01mainTreeView, SIGNAL(clicked(QModelIndex)), ui->LV_01mainListView, SLOT(setRootIndex(QModelIndex)));
    connect(ui->TV_01mainTreeView, SIGNAL(clicked(QModelIndex)), ui->TBV_01mainTableView, SLOT(setRootIndex(QModelIndex)));
}

TestQFileSystemModel::TestQFileSystemModel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestQFileSystemModel)
{
    ui->setupUi(this);

    initAll();
}

TestQFileSystemModel::~TestQFileSystemModel()
{
    delete ui;
}


void TestQFileSystemModel::on_TV_01mainTreeView_clicked(const QModelIndex &index)
{
    ui->CB_02nodeIsDirectory->setChecked(QFSMp_model_m->isDir(index)); //设置是否为文件夹
    ui->LB_01fileName->setText(QFSMp_model_m->fileName(index)); //文件名或最后一级文件夹名
    ui->LB_03nodeType->setText(QFSMp_model_m->type(index)); //文件类型的描述性文字
    ui->LB_04fullPath->setText(QFSMp_model_m->filePath(index)); //完整路径或者文件夹名
    qint64 q64_size_t =  QFSMp_model_m->size(index) / 1024; //文件夹的大小是0
    if (q64_size_t < 1024) {
        ui->LB_02fileSize->setText(QString("%1 KB").arg(q64_size_t));
    } else {
        ui->LB_02fileSize->setText(QString::asprintf("%.2f MB", (q64_size_t / 1024.0)));
    }
}



void TestQFileSystemModel::on_act01File_setRootPath_triggered()
{
    QString QS_directory_t = QFileDialog::getExistingDirectory(this, "选择目录", QDir::currentPath());
    if (!QS_directory_t.isEmpty()) {
        QFSMp_model_m->setRootPath(QS_directory_t);
        //为模型重新设置根目录后，需要用QAbstractItemView的公有槽setRootIndex来设置视图结构的根目录，这样视图就会显示此节点下的文件和目录
        ui->TV_01mainTreeView->setRootIndex(QFSMp_model_m->index(QS_directory_t));
        ////注意，这里的QFileSystemModel::index()是重载的index函数，参数是路径名
    }
}


void TestQFileSystemModel::on_RB_01showAll_clicked()
{
    ui->GB_05filter->setEnabled(true);
    QFSMp_model_m->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
}


void TestQFileSystemModel::on_RB_02showDirOnly_clicked()
{
    ui->GB_05filter->setEnabled(false);
    QFSMp_model_m->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot); //不列出文件
}


void TestQFileSystemModel::on_CB_01fileFilter_clicked(bool checked)
{
    //如果设置为true，则未通过文件名过滤器的项设置为 禁用；如果设置为false，则未通过文件名过滤器的项被 隐藏
    QFSMp_model_m->setNameFilterDisables(!checked);
    ui->CBB_01chooseType->setEnabled(checked);
    ui->TB_01quickApply->setEnabled(checked);
}


void TestQFileSystemModel::on_TB_01quickApply_clicked()
{
    QString QS_filters_t = ui->CBB_01chooseType->currentText().trimmed();
    QStringList QSL_filter_t = QS_filters_t.split(";", Qt::SkipEmptyParts);
    QFSMp_model_m->setNameFilters(QSL_filter_t);
}

