#include <QDebug>

#include "TestSliderProgressBar.h"
#include "ui_TestSliderProgressBar.h"

TestSliderProgressBar::TestSliderProgressBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestSliderProgressBar)
{
    ui->setupUi(this);
}

TestSliderProgressBar::~TestSliderProgressBar()
{
    delete ui;
}


void TestSliderProgressBar::on_checkBox_01TextVisible_clicked(bool checked)
{
    ui->progressBar->setTextVisible(checked);
}


void TestSliderProgressBar::on_checkBox_02InvertedAppearance_clicked(bool checked)
{
    ui->progressBar->setInvertedAppearance(checked);
}


void TestSliderProgressBar::on_radioButton_01Percent_clicked()
{
    ui->progressBar->setFormat("%p%");
}


void TestSliderProgressBar::on_radioButton_02Value_clicked()
{
    ui->progressBar->setFormat("%v");
}


void TestSliderProgressBar::on_checkBox_03TristateCheckBox_stateChanged(int arg1)
{
    qDebug() << "int arg1 = " << arg1;
    switch (arg1) {
    case 0:
        ui->progressBar->setFormat("%p%"); //进度条百分比模式
        break;
    case 1:
        ui->progressBar->setFormat("%v"); //进度条显示当前值
        break;
    case 2:
        ui->progressBar->setFormat("%m"); //显示总步数
        break;
    default:
        ui->progressBar->setFormat("%p%");
        break;
    }
}

