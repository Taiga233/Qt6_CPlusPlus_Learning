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
#include "./ui_TestWidgetTypicalEvent.h"

#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QPainter>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include <QHideEvent>

//testQDebugQLocaleQString() used:
#include <QDateTime>
// #include <QTextCharFormat> //qt5
// #pragma execution_character_set("utf-8")

TestWidgetTypicalEvent::TestWidgetTypicalEvent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestWidgetTypicalEvent)
{
    ui->setupUi(this);

    //test:
    // testQDebugQLocaleQString();
}

TestWidgetTypicalEvent::~TestWidgetTypicalEvent()
{
    delete ui;
}

//重写绘制事件
void TestWidgetTypicalEvent::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //将资源文件中的图片绘制到窗口的整体区域，绘图时使用了窗口的画笔对象painter
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/images/background.jpg"));
    //下面是表示运行父类的paintEvent()函数，以便父类执行其内建的一些操作。
    //如果父类的事件函数里没有特殊的处理，可以不运行下面的代码。
    // QWidget::paintEvent(event);
}

//重写关闭窗口事件处理函数
//当窗口被关闭时，例如点击右上角的关闭按钮或者调用QWidget的close()函数时，系统会产生一个QEvent::Close类型的事件，事件处理函数closeEvent()会被自动运行
void TestWidgetTypicalEvent::closeEvent(QCloseEvent *event)
{
    QString title_QS_t("消息提示");
    QString message_QS_t("您确认关闭窗口吗？");
    QMessageBox::StandardButton result_e_t = QMessageBox::question(this, title_QS_t, message_QS_t, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (result_e_t == QMessageBox::Yes) {
        // 关闭窗口；接受事件，窗口可以被关闭
        event->accept();
    } else if (result_e_t == QMessageBox::No) {
        // 忽略关闭窗口；忽略事件，窗口不能被关闭
        event->ignore();
    } else {
        // 取消关闭窗口
        event->ignore();
    }
}

void TestWidgetTypicalEvent::testQDebugQLocaleQString()
{
    // 设置编码格式为 UTF-8
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //qt5
    QString test("你好");
    QString test2 = QString::fromUtf8("测试uuu");
    QString test3 = QString::fromLocal8Bit("æµ‹è¯•uuu");
    qDebug() << "你好你好：" << test << test2 << tr("s中文s") << test3;
    qDebug() << test3.toLatin1().size();
    qDebug() <<QString::fromLocal8Bit(test.toUtf8());
    qDebug() << test.unicode();

    QLocale locale1;
    qDebug() << locale1.country(); //deprecated in 6.6, use territory() instead
    qDebug() << locale1.language();
    qDebug() << locale1;
    qDebug() << locale1.toString(QDateTime::currentDateTime()) << Qt::flush;

    QLocale locale2(QLocale::Chinese, QLocale::China);
    QLocale::setDefault(locale2);

    qDebug() << "你好你好：" << test << test2 << tr("s中文s") << test3;
}

//在窗口上点击鼠标按键时，会触发运行事件处理函数mousePressEvent()
void TestWidgetTypicalEvent::mousePressEvent(QMouseEvent *event)
{
    //除了pos()函数，其他都是QMouseEvent的父类QSinglePointEvent中定义的
    if (event->button() == Qt::LeftButton) {
        //pos()返回的是鼠标光标 在接受此事件的 组件上的 相对坐标
        QPoint point_QP_t = event->pos(); //相对坐标
        //position()同pos()，返回的是鼠标光标 在接受此事件的 组件上的 相对坐标
        QPointF relativePoint_QPF_t = event->position(); //相对坐标
        //scenePosition()返回的是鼠标光标在接受此事件的 窗口或场景上的 相对坐标
        QPointF winPoint_QPF_t = event->scenePosition(); //相对坐标
        //globalPosition()返回的是鼠标光标在 屏幕或虚拟桌面上的 绝对坐标
        QPointF globalPoint_QPF_t = event->globalPosition(); //绝对坐标

        QString showString_QS_t = QString::asprintf("pos() = (%d, %d)", point_QP_t.x(), point_QP_t.y());
        showString_QS_t += QString::asprintf("\nposition() = (%.2f, %.2f)", relativePoint_QPF_t.x(), relativePoint_QPF_t.y());
        showString_QS_t += QString::asprintf("\nscenePosition() = (%.2f, %.2f)", winPoint_QPF_t.x(), winPoint_QPF_t.y());
        showString_QS_t += QString::asprintf("\nglobalPosition() = (%d, %d)", globalPoint_QPF_t.x(), globalPoint_QPF_t.y());

        ui->label_01_PosInfo->setText(showString_QS_t);
        ui->label_01_PosInfo->adjustSize(); //自动调整组件大小
        ui->label_01_PosInfo->move(point_QP_t); //label移动到鼠标光标处
    }
    QWidget::mousePressEvent(event); //执行父类QWidget默认的其他操作
}

void TestWidgetTypicalEvent::keyPressEvent(QKeyEvent *event)
{
    QPoint mvBtnPos_QP_t = ui->pushB_01_movableBtn->pos();
    switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_W:
            mvBtnPos_QP_t.ry() -= 10;
            qDebug() << "Key_W triggered!";
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            mvBtnPos_QP_t.ry() += 10;
            qDebug() << "Key_S triggered!";
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            mvBtnPos_QP_t.rx() -= 10;
            qDebug() << "Key_A triggered!";
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            mvBtnPos_QP_t.rx() += 10;
            qDebug() << "Key_D triggered!";
            break;
        default:
            break;
    }
    ui->pushB_01_movableBtn->move(mvBtnPos_QP_t);

    event->accept(); //接收事件，不会再传播到父容器组件
}

void TestWidgetTypicalEvent::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
    return ;

    //这里发现！！！
    //使用函数keyPressEvent()时（不使用keyReleaseEvent()），只有按下W、A、S、D键有效，如果使用的是keyReleaseEvent()函数，则按下W、A、S、D键和上、下、左、右键都有效。
    //这说明按下上、下、左、右方向键不会产生QEvent::KeyPress类型的事件，只会在按键释放时产生QEvent::KeyRelease类型的事件。
    QPoint mvBtnPos_QP_t = ui->pushB_01_movableBtn->pos();
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        mvBtnPos_QP_t.ry() -= 20;
        qDebug() << "Key_W & Key_Up triggered!";
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        mvBtnPos_QP_t.ry() += 20;
        qDebug() << "Key_S & Key_Down triggered!";
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        mvBtnPos_QP_t.rx() -= 20;
        qDebug() << "Key_A & Key_Left triggered!";
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        mvBtnPos_QP_t.rx() += 20;
        qDebug() << "Key_D & Key_Right triggered!";
        break;
    default:
        break;
    }
    ui->pushB_01_movableBtn->move(mvBtnPos_QP_t);

    event->accept(); //接收事件，不会再传播到父容器组件
}

void TestWidgetTypicalEvent::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "showEvent has been triggered!";
}

void TestWidgetTypicalEvent::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "hideEvent has been triggered!";
}
