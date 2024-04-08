#ifndef TESTABSTRACTBUTTON_H
#define TESTABSTRACTBUTTON_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TestAbstractButton; }
QT_END_NAMESPACE

class TestAbstractButton : public QWidget
{
    Q_OBJECT

public:
    TestAbstractButton(QWidget *parent = nullptr);
    ~TestAbstractButton();

private slots:
    void on_pushButton_01AlignmentLeft_clicked();

    void on_pushButton_02AlignmentMiddle_clicked();

    void on_pushButton_03AlignmentRight_clicked();

    void on_pushButton_04FontBold_clicked(bool checked);

    void on_pushButton_05FontItalic_clicked(bool checked);

    void on_pushButton_06FontUnderline_clicked(bool checked);

    void on_checkBox_01ReadOnly_clicked(bool checked);

    void on_checkBox_02Enabled_clicked(bool checked);

    void on_checkBox_03ClearButton_clicked(bool checked);

    void on_radioButton_01Black_clicked();

    void on_radioButton_02Red_clicked();

    void on_radioButton_03Blue_clicked();

private:
    Ui::TestAbstractButton *ui;
};
#endif // TESTABSTRACTBUTTON_H
