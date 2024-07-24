/*! **********************************************************
 *  @File        : main.cpp
 *  @Brief       : xxxxx
 *  @Details     : xxxxx
 *  @Author      : YiRongPeng
 *  @Email       : taiga_tora@foxmail.com
 *  @Date        : 2024-07-23 11:34:11
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
#include "CustomLabel.h"

#include <QPalette>
#include <QEvent>
#include <QStyleHints>
#include <QGuiApplication>

//构造函数需要将该对象加入Qt对象树
CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent)
{
    this->setAttribute(Qt::WA_Hover, true); //!!!必须设置这个属性才能产生hover事件
}

//重新实现event()
bool CustomLabel::event(QEvent *event)
{
    //某些类型的事件没有对应的事件处理函数，例如，对于QEvent::HoverEnvet和QEvent::HoverLeave类型的事件，QWidget类中就没有对应的事件处理函数。
    //所以重写函数event()，判断事件类型，针对这两个事件类型进行相应的处理
    if (event->type() == QEvent::HoverEnter) {
        //鼠标光标移入
        QPalette palette_QP_t = this->palette();
        palette_QP_t.setColor(QPalette::WindowText, Qt::red);
        this->setPalette(palette_QP_t);
    } else if (event->type() == QEvent::HoverLeave) {
        //鼠标光标移出
        QPalette palette_QP_t = this->palette();
        palette_QP_t.setColor(QPalette::WindowText, (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) ? Qt::lightGray : Qt::darkGray);
        this->setPalette(palette_QP_t);
    } else { }
    //运行父类的event()处理其他类型事件
    return QLabel::event(event);
}

//重新实现鼠标双击事件的默认处理函数
void CustomLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //mouseDoubleClickEvent()是鼠标双击事件（QEvent::MouseButtonDblClick）的默认处理函数，在此发射自定义信号。
    //这样就把鼠标双击事件转换为发射一个信号，如果要对自定义Label的鼠标双击事件处理则只需要为其doubleClicked()信号编写槽函数即可。
    emit doubleClicked(); //鼠标双击发送该自定义信号
}
