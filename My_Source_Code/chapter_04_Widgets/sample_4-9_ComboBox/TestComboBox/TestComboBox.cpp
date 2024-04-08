#include "TestComboBox.h"
#include "ui_TestComboBox.h"

#include <QRandomGenerator>

TestComboBox::TestComboBox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestComboBox)
{
    ui->setupUi(this);
}

TestComboBox::~TestComboBox()
{
    delete ui;
}


void TestComboBox::on_pushButton_01InitList_clicked()
{
    QIcon icon;
    quint32 item_count = 0;
    while(!item_count) item_count = QRandomGenerator::global()->generate() % 20;
    qDebug() << "item_count = " << item_count;
    icon.addFile("://icons/aim.ico");
    ui->comboBox_01Simple->clear();
    for (uint i = 0; i < item_count; i++) {
        if (QRandomGenerator::system()->generate() & 1)
            ui->comboBox_01Simple->addItem(icon, QString("Item%1 with icon").arg(i));
        else
            ui->comboBox_01Simple->addItem(QString("Item%1 without icon").arg(i));
    }
}


void TestComboBox::on_pushButton_02InitCity_clicked()
{
    //QMap自动按照key排序
    QMap<QString, int> city_zone;
    city_zone.insert("Beijing", 10);
    city_zone.insert("Shanghai", 21);
    city_zone.insert("Tianjing", 22);
    city_zone.insert("Dalian", 411);
    city_zone.insert("Jinzhou", 416);
    city_zone.insert("Xuzhou", 516);
    city_zone.insert("Fuzhou", 591);
    city_zone.insert("Qingdao", 532);
    ui->comboBox_02Data->clear();
    //直接用foreach来遍历Map中的key列表
    foreach (const QString &str, city_zone.keys()) {
        ui->comboBox_02Data->addItem(str, city_zone.value(str));
    }
}


void TestComboBox::on_comboBox_02Data_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (!~index) return;
    qDebug() << "current index:" << index;
    QString city = ui->comboBox_02Data->currentText();
    QString zone = ui->comboBox_02Data->currentData().toString();
    ui->plainTextEdit->appendPlainText(city + " zone-number = " + zone);
}


void TestComboBox::on_checkBox_clicked(bool checked)
{
    ui->comboBox_01Simple->setEditable(checked);
}

