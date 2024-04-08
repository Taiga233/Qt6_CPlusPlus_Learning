#include "TestAbstractButton.h"
#include "ui_TestAbstractButton.h"

TestAbstractButton::TestAbstractButton(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestAbstractButton)
{
    ui->setupUi(this);
}

TestAbstractButton::~TestAbstractButton()
{
    delete ui;
}


void TestAbstractButton::on_pushButton_01AlignmentLeft_clicked()
{
    ui->lineEdit->setAlignment(Qt::AlignLeft);
}


void TestAbstractButton::on_pushButton_02AlignmentMiddle_clicked()
{
    ui->lineEdit->setAlignment(Qt::AlignCenter);
}


void TestAbstractButton::on_pushButton_03AlignmentRight_clicked()
{
    ui->lineEdit->setAlignment(Qt::AlignRight);
}


void TestAbstractButton::on_pushButton_04FontBold_clicked(bool checked)
{
    QFont font = ui->lineEdit->font();
    font.setBold(checked);
    ui->lineEdit->setFont(font);
}


void TestAbstractButton::on_pushButton_05FontItalic_clicked(bool checked)
{
    QFont font = ui->lineEdit->font();
    font.setItalic(checked);
    ui->lineEdit->setFont(font);
}


void TestAbstractButton::on_pushButton_06FontUnderline_clicked(bool checked)
{
    QFont font = ui->lineEdit->font();
    font.setUnderline(checked);
    ui->lineEdit->setFont(font);
}


void TestAbstractButton::on_checkBox_01ReadOnly_clicked(bool checked)
{
    ui->lineEdit->setReadOnly(checked);
}


void TestAbstractButton::on_checkBox_02Enabled_clicked(bool checked)
{
    ui->lineEdit->setEnabled(checked);
}


void TestAbstractButton::on_checkBox_03ClearButton_clicked(bool checked)
{
    ui->lineEdit->setClearButtonEnabled(checked);
}


void TestAbstractButton::on_radioButton_01Black_clicked()
{
    QPalette palette = ui->lineEdit->palette();
    palette.setColor(QPalette::Text, Qt::black);
    ui->lineEdit->setPalette(palette);
}


void TestAbstractButton::on_radioButton_02Red_clicked()
{
    QPalette palette = ui->lineEdit->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->lineEdit->setPalette(palette);
}


void TestAbstractButton::on_radioButton_03Blue_clicked()
{
    QPalette palette = ui->lineEdit->palette();
    palette.setColor(QPalette::Text, Qt::blue);
    ui->lineEdit->setPalette(palette);
}

