#include "TestTimerElapsedTimer.h"
#include "ui_TestTimerElapsedTimer.h"

#include <QTime>

TestTimerElapsedTimer::TestTimerElapsedTimer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestTimerElapsedTimer)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    m_timer->stop();
    m_timer->setTimerType(Qt::CoarseTimer);
    ui->radioButton_04Coarse->setChecked(true);
    this->connect(m_timer, SIGNAL(timeout()), SLOT(do_timer_timeout()));
}

TestTimerElapsedTimer::~TestTimerElapsedTimer()
{
    delete ui;
}

void TestTimerElapsedTimer::do_timer_timeout()
{
    qDebug() << "do_timer_timeout()";
    QApplication::beep(); //使系统蜂鸣器响一声
    QTime curTime = QTime::currentTime();
    ui->lcdNumber_01Hour->display(curTime.hour());
    ui->lcdNumber_02Minute->display(curTime.minute());
    ui->lcdNumber_03Second->display(curTime.second());
    if (m_timer->isSingleShot()) {
        on_pushButton_02Stop_clicked(); //如果是单个定时器就给他关了
    }
}

void TestTimerElapsedTimer::do_single_shot()
{
    QApplication::beep();
    qint64 elapsed_nsec = m_counter.nsecsElapsed();
    int nsec = elapsed_nsec % 1000;
    int msec = (elapsed_nsec / 1000000) % 1000;
    int sec = elapsed_nsec / 1000000000;
    QString str = QString("%1 seconds %2 milliseconds %3 nanoseconds").arg(sec).arg(msec, 3, 10, QChar('0')).arg(nsec, 3, 10, QChar('0'));
    str += QString(", total:%1nanoseconds").arg(elapsed_nsec);
    ui->label_03Elapsed->setText(str);
    ui->pushButton_03Create->setEnabled(true);
}


void TestTimerElapsedTimer::on_pushButton_01Start_clicked()
{
    m_timer->setInterval(ui->spinBox->value()); //设置定时器的周期
    if (ui->radioButton_01Succession->isChecked() || ui->radioButton_02Single->isChecked()) {
        m_timer->setSingleShot(!ui->radioButton_01Succession->isChecked()); //设置连续定时or单次定时
    }
    //设置定时器的精度
    m_timer->setTimerType(ui->radioButton_03Precise->isChecked() ? (Qt::PreciseTimer) : ui->radioButton_05VeryCoarse->isChecked() ? (Qt::VeryCoarseTimer) : (Qt::CoarseTimer));
    m_timer->start();
    m_counter.start();
    ui->pushButton_01Start->setEnabled(false);
    ui->pushButton_02Stop->setEnabled(true);
    ui->pushButton_03Create->setEnabled(false);
}


void TestTimerElapsedTimer::on_pushButton_02Stop_clicked()
{
    m_timer->stop();
    qint64 elapsed_msec = m_counter.elapsed();
    int ms = elapsed_msec % 1000;
    int sec = elapsed_msec / 1000;
    QString str = QString("%1 seconds %2 milliseconds").arg(sec).arg(ms, 3, 10, QChar('0'));
    ui->label_03Elapsed->setText(str);
    ui->pushButton_01Start->setEnabled(true);
    ui->pushButton_02Stop->setEnabled(false);
    ui->pushButton_03Create->setEnabled(true);
}


void TestTimerElapsedTimer::on_pushButton_03Create_clicked()
{
    int interval = ui->spinBox->value();
    ui->pushButton_03Create->setEnabled(false);
    QTimer::singleShot(interval, Qt::PreciseTimer, this, &TestTimerElapsedTimer::do_single_shot);
    m_counter.start();
}

