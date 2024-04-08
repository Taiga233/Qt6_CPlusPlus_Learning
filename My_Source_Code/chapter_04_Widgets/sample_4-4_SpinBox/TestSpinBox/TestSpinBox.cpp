#include <QDebug>

#include "TestSpinBox.h"
#include "ui_TestSpinBox.h"

TestSpinBox::TestSpinBox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestSpinBox)
{
    ui->setupUi(this);
}

TestSpinBox::~TestSpinBox()
{
    delete ui;
}



void TestSpinBox::on_pushButton_01Calculate_clicked()
{
    int count = ui->spinBox_01Count->value();
    float price = ui->doubleSpinBox_01Price->value();
    float total = count * price;
    ui->doubleSpinBox_02Total->setValue(total);
}


void TestSpinBox::on_spinBox_02Dec_valueChanged(int arg1)
{
    //用qDebug()显示仅数值的十进制字符串
    qDebug("十进制字符串:%s", ui->spinBox_02Dec->cleanText().toLocal8Bit().data());
    ui->spinBox_03Bin->setValue(arg1);
    ui->spinBox_04Hex->setValue(arg1);
}


void TestSpinBox::on_spinBox_03Bin_valueChanged(int arg1)
{
    //用qDebug()显示仅数值的二进制字符串
    qDebug("二进制字符串:%s", ui->spinBox_03Bin->cleanText().toLocal8Bit().data());
    ui->spinBox_02Dec->setValue(arg1);
    ui->spinBox_04Hex->setValue(arg1);
}


void TestSpinBox::on_spinBox_04Hex_valueChanged(int arg1)
{
    //用qDebug()显示仅数值的十六进制字符串
    qDebug("十六进制字符串:%s", ui->spinBox_04Hex->cleanText().toLocal8Bit().data());
    ui->spinBox_02Dec->setValue(arg1);
    ui->spinBox_03Bin->setValue(arg1);
}

