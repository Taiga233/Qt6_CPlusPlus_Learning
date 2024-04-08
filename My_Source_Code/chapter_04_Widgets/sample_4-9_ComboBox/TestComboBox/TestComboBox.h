#ifndef TESTCOMBOBOX_H
#define TESTCOMBOBOX_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TestComboBox; }
QT_END_NAMESPACE

class TestComboBox : public QWidget
{
    Q_OBJECT

public:
    TestComboBox(QWidget *parent = nullptr);
    ~TestComboBox();

private slots:
    void on_pushButton_01InitList_clicked();

    void on_pushButton_02InitCity_clicked();

    void on_comboBox_02Data_currentIndexChanged(int index);

    void on_checkBox_clicked(bool checked);

private:
    Ui::TestComboBox *ui;
};
#endif // TESTCOMBOBOX_H
