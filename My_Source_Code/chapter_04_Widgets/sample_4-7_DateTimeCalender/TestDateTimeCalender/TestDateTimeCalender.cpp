#include "TestDateTimeCalender.h"
#include "qdatetime.h"
#include "ui_TestDateTimeCalender.h"

TestDateTimeCalender::TestDateTimeCalender(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestDateTimeCalender)
{
    ui->setupUi(this);
}

TestDateTimeCalender::~TestDateTimeCalender()
{
    delete ui;
}


void TestDateTimeCalender::on_pushButton_01Read_clicked()
{
    QDateTime curDT = QDateTime::currentDateTime();
    ui->timeEdit->setTime(curDT.time());
    ui->dateEdit->setDate(curDT.date());
    ui->dateTimeEdit->setDateTime(curDT);
    ui->lineEdit_01Time->setText(curDT.time().toString("hh:mm:ss.zzz"));
    ui->lineEdit_02Date->setText(curDT.date().toString("yyyy-MM-dd"));
    ui->lineEdit_03DateTime->setText(curDT.toString("dd-MM-yyyy hh:mm:ss.zzz"));
    ui->label_01Display->setText("01Read: " + curDT.toString());
}


void TestDateTimeCalender::on_pushButton_02Time_clicked()
{
    QTime time01(15, 39, 5);
    QString str = time01.toString("HH:mm:ss.zzz");
    qDebug("Original time = %s", str.toLocal8Bit().data());
    QTime time02 = time01.addSecs(150);
    qDebug("time01 add 150s time = %s", time02.toString("HH:mm:ss.zzz").toLocal8Bit().data());
    QTime time03 = time02.addMSecs(1234);
    qDebug("time02 add 1234ms time = %s", time03.toString("HH:mm:ss.zzz").toLocal8Bit().data());

    QTime time04 = QTime::currentTime();
    str = time04.toString("HH:mm:ss.zzz");
    qDebug("current time:%s; Hour:%d; Minute:%d; Second:%d; MSecond:%d", str.toLocal8Bit().data(), time04.hour(), time04.minute(), time04.second(), time04.msec());
    ui->label_01Display->setText("02Time: " + time04.toString());
}


void TestDateTimeCalender::on_pushButton_03SetTime_clicked()
{
    QString str = ui->lineEdit_01Time->text();
    str = str.trimmed(); //去掉多余空格
    if (!str.isEmpty()) {
        qDebug() << "str = " << str;
        QTime time = QTime::fromString(str, "hh:mm:ss.zzz");
        qDebug() << "time = " << time;
        if (time.isValid()) {
            ui->timeEdit->setTime(time);
            ui->label_01Display->setText("03SetTime: " + time.toString());
        }
    }
}


void TestDateTimeCalender::on_pushButton_04Date_clicked()
{
    QDate date01(2022, 10, 1);
    qDebug("date01 = %s", date01.toString("yyyy-MM-dd").toLocal8Bit().data());
    QDate date02;
    date02.setDate(2012, 5, 12);
    qDebug("date02 = %s", date02.toString("yyyy-MM-dd").toLocal8Bit().data());
    qDebug("Days between date01 and date02 = %lld", date02.daysTo(date01));

    QDate date03 = QDate::currentDate();
    qDebug("Current date:%s; Year:%d; Month:%d; Day:%d; Day of Week:%d; Day of year:%d; Days in month:%d; Days in year:%d", date03.toString("yyyy-MM-dd").toLocal8Bit().data(), date03.year(), date03.month(), date03.day(), date03.dayOfWeek(), date03.dayOfYear(), date03.daysInMonth(), date03.daysInYear());
    qDebug() << "Current year is LeapYear?" << QDate::isLeapYear(date03.year());
    ui->label_01Display->setText("04Date: " + date03.toString());
}


void TestDateTimeCalender::on_pushButton_05SetDate_clicked()
{
    QString str = ui->lineEdit_02Date->text();
    str = str.trimmed(); //去掉多余空格
    if (!str.isEmpty()) {
        qDebug() << "str = " << str;
        QDate date = QDate::fromString(str, "yyyy-MM-dd");
        qDebug() << "date = " << date;
        if (date.isValid()) {
            ui->dateEdit->setDate(date);
            ui->label_01Display->setText("05SetDate: " + date.toString());
        }
    }
}


void TestDateTimeCalender::on_pushButton_06DateTime_clicked()
{
    QDateTime DT1 = QDateTime::currentDateTime();
    QDate date = DT1.addDays(100).date();
    QTime time = DT1.addSecs(1234).addMSecs(567890).time();
    qDebug() << "Current DateTime:" << DT1 << " toString():" << DT1.toString("yyyy-MM-dd hh:mm:ss.zzz");
    qDebug("DT1.addDays(100).date() = %s, DT1.addSecs(1234).addMSecs(567890).time() = %s", date.toString("yy-M-d").toLocal8Bit().data(), time.toString("h:m:s.z").toLocal8Bit().data());
    qDebug() << "Date Gap:" << date.daysTo(DT1.date()) << " Time Gap(sec):" << time.secsTo(DT1.time()) << " Time Gap(msec)" << time.msecsTo(DT1.time());

    qint64 ms_epoch = DT1.toMSecsSinceEpoch();
    qDebug("DT1.toMSecsSinceEpoch() = %lld", ms_epoch);
    ms_epoch += 100000;
    QDateTime DT2;
    DT2.setMSecsSinceEpoch(ms_epoch);
    qDebug("DT1 + 100000ms = %s", DT2.toString("yyyy-MM-dd hh:mm:ss.zzz").toLocal8Bit().data());
    QDateTime DT3 = DT1.toUTC();
    qDebug() << "Current DateTime to UTC:" << DT3;
    ui->label_01Display->setText("06DateTime: " + DT1.toString());
}


void TestDateTimeCalender::on_pushButton_07SetDT_clicked()
{
    QString str = ui->lineEdit_03DateTime->text();
    str = str.trimmed(); //去掉多余空格
    if (!str.isEmpty()) {
        qDebug() << "str = " << str;
        QDateTime datetime = QDateTime::fromString(str, "dd.MM.yyyy hh:mm:ss.zzz");
        qDebug() << "datetime = " << datetime;
        if (datetime.isValid()) {
            ui->dateTimeEdit->setDateTime(datetime);
            ui->label_01Display->setText("07SetDT: " + datetime.toString());
        }
    }
}


void TestDateTimeCalender::on_calendarWidget_selectionChanged()
{
    qDebug() << "on_calendarWidget_selectionChanged():" << ui->calendarWidget->selectedDate().toString();
}

