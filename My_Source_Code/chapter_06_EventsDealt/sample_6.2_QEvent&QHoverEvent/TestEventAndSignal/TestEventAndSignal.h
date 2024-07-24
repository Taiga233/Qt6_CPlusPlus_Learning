/*! **********************************************************
 *  @File        :
 *  @Brief       : xxxxx
 *  @Details     : xxxxx
 *  @Author      : YiRongPeng
 *  @Email       : taiga_tora@foxmail.com
 *  @Date        : 2024-07-23 11:23:41
 *  @Version     : v1.0
 *  @System      : Windows/Linux/Unix
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
#ifndef TESTEVENTANDSIGNAL_H
#define TESTEVENTANDSIGNAL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TestEventAndSignal;
}
QT_END_NAMESPACE

class TestEventAndSignal : public QWidget
{
    Q_OBJECT

public:
    TestEventAndSignal(QWidget *parent = nullptr);
    ~TestEventAndSignal();

private slots:
    void doCustomLabel_doubleClick_slt(); // connect with label_01_doubleClick doublClick()

private:
    Ui::TestEventAndSignal *ui;

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE; // called when mouse key is double clicked
};
#endif // TESTEVENTANDSIGNAL_H
