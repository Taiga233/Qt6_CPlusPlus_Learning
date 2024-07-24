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
#include "TestEventAndSignal.h"

#include <QApplication>
//darkOrLightMode
#include <QStyleHints>
#include <QDebug>
#include <QStyleHints>
#include <QStyleFactory>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << a.styleHints()->colorScheme();
    // reset Appliction style
    // in windows11, this will be followed with system mode,eg. dark mode
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    // QApplication::setStyle(QStyleFactory::create("Windows"));
    // reset default palette
    a.setPalette(QApplication::style()->standardPalette());

    TestEventAndSignal w;
    w.show();
    return a.exec();
}
