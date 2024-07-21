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

#ifndef TESTWIDGETTYPICALEVENT_H
#define TESTWIDGETTYPICALEVENT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TestWidgetTypicalEvent;
}
QT_END_NAMESPACE

class TestWidgetTypicalEvent : public QWidget
{
    Q_OBJECT

public:
    TestWidgetTypicalEvent(QWidget *parent = nullptr);
    ~TestWidgetTypicalEvent();

protected:
    void paintEvent(QPaintEvent *event); //绘制事件
    void closeEvent(QCloseEvent *event); //窗口、组件被关闭事件
    // void keyReleaseEvent(QKeyEvent *event); //键盘按键释放事件
    // void keyPressEvent(QKeyEvent *event); //键盘按键按下事件
    // void showEvent(QShowEvent *event); //窗口在屏幕上显示出来或组件变得可见时的事件处理函数
    // void hideEvent(QHideEvent *event); //窗口在屏幕上隐藏（例如窗口最小化），或组件变得不可见
    // void mousePressEvent(QMouseEvent *event); //鼠标按键按下，可以是左键或右键

private:
    void testQDebugQLocaleQString();

private:
    Ui::TestWidgetTypicalEvent *ui;

//Qt Creator自动生成的重写虚函数：
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
};
#endif // TESTWIDGETTYPICALEVENT_H
