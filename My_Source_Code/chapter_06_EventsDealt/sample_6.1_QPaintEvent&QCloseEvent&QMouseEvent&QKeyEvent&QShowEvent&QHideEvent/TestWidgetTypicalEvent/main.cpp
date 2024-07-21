/*! ***********************************************************
 *  @File        :
 *  @Brief       : xxxxx
 *  @Details     : xxxxx
 *  @Author      : YiRongPeng
 *  @Email       : taiga_tora@foxmail.com
 *  @Date        : 2024-07-20 18:57:50
 *  @Version     : v1.0
 *  @Environment : C:Program Files (x86)Common FilesOracleJavajava8path;C:Program Files (x86)Common FilesOracleJavajavapath;C:Windowssystem32;C:Windows;C:WindowsSystem32Wbem;C:WindowsSystem32WindowsPowerShellv1.0;C:WindowsSystem32OpenSSH;C:01_Software09_GitGitcmd;C:UsersTaiga_DevelopmentPCAppDataLocalMicrosoftWindowsApps;C:01_Software04_BandiZip02_app;C:01_Software08_VSCodeMicrosoft VS Codebin;C:UsersTaiga_DevelopmentPCAppDataLocalPandoc;C:01_Software11_QtForAndroidEnvandroid-sdk-windows	oolsbin;C:01_Software11_QtForAndroidEnvandroid-sdk-windowsplatform-tools;
 *
 **************************************************************
 *
 *  @par modified log :
 *  @Attention        :
 *
 * This file is part of the QtWidgets module of the Qt Toolkit.
 * Copyright (C) 2024 The Qt Company Ltd.
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

#include "TestWidgetTypicalEvent.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWidgetTypicalEvent w;
    w.show();
    return a.exec();
}
