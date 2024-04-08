#ifndef TESTTIMERELAPSEDTIMER_H
#define TESTTIMERELAPSEDTIMER_H

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class TestTimerElapsedTimer; }
QT_END_NAMESPACE

class TestTimerElapsedTimer : public QWidget
{
    Q_OBJECT

public:
    TestTimerElapsedTimer(QWidget *parent = nullptr);
    ~TestTimerElapsedTimer();

private slots:
    void do_timer_timeout();
    void do_single_shot();

    void on_pushButton_01Start_clicked();

    void on_pushButton_02Stop_clicked();

    void on_pushButton_03Create_clicked();

private:
    Ui::TestTimerElapsedTimer *ui;

    QTimer *m_timer;
    QElapsedTimer m_counter;
};
#endif // TESTTIMERELAPSEDTIMER_H
