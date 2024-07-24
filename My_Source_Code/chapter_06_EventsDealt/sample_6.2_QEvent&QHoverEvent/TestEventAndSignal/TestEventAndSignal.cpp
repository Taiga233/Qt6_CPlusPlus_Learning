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
#include "./ui_TestEventAndSignal.h"

TestEventAndSignal::TestEventAndSignal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestEventAndSignal)
{
    ui->setupUi(this);
    
    connect(ui->label_01_hoverable, SIGNAL(doubleClicked()), this, SLOT(doCustomLabel_doubleClick_slt()));
}

TestEventAndSignal::~TestEventAndSignal()
{
    delete ui;
}

void TestEventAndSignal::doCustomLabel_doubleClick_slt()
{
    ui->label_01_hoverable->setText("label_01_hoverable was double-clicked !\nCustom slot has been triggered!");
    ui->label_01_hoverable->adjustSize();
}


void TestEventAndSignal::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    ui->label_01_hoverable->setText("Widget window was double-Clicked !");
    ui->label_01_hoverable->adjustSize();
}
