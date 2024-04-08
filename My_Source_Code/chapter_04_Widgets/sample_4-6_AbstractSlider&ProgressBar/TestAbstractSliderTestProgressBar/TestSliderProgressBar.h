#ifndef TESTSLIDERPROGRESSBAR_H
#define TESTSLIDERPROGRESSBAR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TestSliderProgressBar; }
QT_END_NAMESPACE

class TestSliderProgressBar : public QWidget
{
    Q_OBJECT

public:
    TestSliderProgressBar(QWidget *parent = nullptr);
    ~TestSliderProgressBar();

private slots:
    void on_checkBox_01TextVisible_clicked(bool checked);

    void on_checkBox_02InvertedAppearance_clicked(bool checked);

    void on_radioButton_01Percent_clicked();

    void on_radioButton_02Value_clicked();

    void on_checkBox_03TristateCheckBox_stateChanged(int arg1);

private:
    Ui::TestSliderProgressBar *ui;
};
#endif // TESTSLIDERPROGRESSBAR_H
