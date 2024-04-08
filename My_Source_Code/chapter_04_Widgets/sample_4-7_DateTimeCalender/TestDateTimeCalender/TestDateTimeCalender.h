#ifndef TESTDATETIMECALENDER_H
#define TESTDATETIMECALENDER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TestDateTimeCalender; }
QT_END_NAMESPACE

class TestDateTimeCalender : public QWidget
{
    Q_OBJECT

public:
    TestDateTimeCalender(QWidget *parent = nullptr);
    ~TestDateTimeCalender();

private slots:
    void on_pushButton_01Read_clicked();

    void on_pushButton_02Time_clicked();

    void on_pushButton_03SetTime_clicked();

    void on_pushButton_04Date_clicked();

    void on_pushButton_05SetDate_clicked();

    void on_pushButton_06DateTime_clicked();

    void on_pushButton_07SetDT_clicked();

    void on_calendarWidget_selectionChanged();

private:
    Ui::TestDateTimeCalender *ui;
};
#endif // TESTDATETIMECALENDER_H
