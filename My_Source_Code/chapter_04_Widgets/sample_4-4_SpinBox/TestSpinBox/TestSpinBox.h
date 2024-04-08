#ifndef TESTSPINBOX_H
#define TESTSPINBOX_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TestSpinBox; }
QT_END_NAMESPACE

class TestSpinBox : public QWidget
{
    Q_OBJECT

public:
    TestSpinBox(QWidget *parent = nullptr);
    ~TestSpinBox();

private slots:
    void on_pushButton_01Calculate_clicked();

    void on_spinBox_02Dec_valueChanged(int arg1);

    void on_spinBox_03Bin_valueChanged(int arg1);

    void on_spinBox_04Hex_valueChanged(int arg1);

private:
    Ui::TestSpinBox *ui;
};
#endif // TESTSPINBOX_H
