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

/*! @File        : TestQFileSystemModel.h
 *  @ClassName   : className
 *  @OS Env      : Linux/Unix&Windows
 *  @Brief       : briefs
 *  @Details     : details
 *  @Author      : Taiga
 *  @Email       : taigaahri@gmail.com
 *  @Date        : 2023-12-12 19:51:23
 *  @Version     : v1.0
 *
 **********************************************************
 *
 *  @par modified log :
 *  @Attention        :
 *
 *********************************************************/

#ifndef TESTQFILESYSTEMMODEL_H
#define TESTQFILESYSTEMMODEL_H

#include <QMainWindow>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class TestQFileSystemModel; }
QT_END_NAMESPACE

class TestQFileSystemModel : public QMainWindow
{
    Q_OBJECT

    void initAll();

public:
    TestQFileSystemModel(QWidget *parent = nullptr);
    ~TestQFileSystemModel();

private slots:
    void on_TV_01mainTreeView_clicked(const QModelIndex &index);

    void on_act01File_setRootPath_triggered();

    void on_RB_01showAll_clicked();

    void on_RB_02showDirOnly_clicked();

    void on_CB_01fileFilter_clicked(bool checked);

    void on_TB_01quickApply_clicked();

private:
    Ui::TestQFileSystemModel *ui;
    QFileSystemModel *QFSMp_model_m; //数据模型变量
};
#endif // TESTQFILESYSTEMMODEL_H
