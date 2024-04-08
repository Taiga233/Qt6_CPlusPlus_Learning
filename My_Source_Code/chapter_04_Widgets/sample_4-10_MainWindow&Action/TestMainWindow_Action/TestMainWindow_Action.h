#ifndef TESTMAINWINDOW_ACTION_H
#define TESTMAINWINDOW_ACTION_H

#define PRIVATE_DATA private
#define PRIVATE_FUNC private

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QSpinBox>
#include <QFontComboBox>
#include <QFont>
#include <QActionGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class TestMainWindow_Action; }
QT_END_NAMESPACE

class TestMainWindow_Action : public QMainWindow
{
    Q_OBJECT

public:
    TestMainWindow_Action(QWidget *parent = nullptr);
    ~TestMainWindow_Action();

PRIVATE_DATA:
    QLabel *labStauts_file; //状态栏里使用
    QLabel *labStauts_Info; //状态栏里使用
    QProgressBar *pgbStatus_show; //状态栏上的进度条
    QSpinBox *sbTool_font; //字体大小
    QFontComboBox *fcbTool_fontName; //字体名称
    QActionGroup *agAction_language; //Action分组，用于“中文”和“英文”的互斥选择

PRIVATE_FUNC:
    void buildCustomUI(); //程序设计的UI初始化
    void buildSignalSlots(); //手动关联信号与槽

private slots: //  自定义组件的槽函数
    //改变字体大小的SpinBox的响应
    void do_fontSize_change(int fontSize);
    //选择字体FontComboBox的响应函数
    void do_fontSelected(const QFont &font);

private slots:
    void on_actFont_Bold_triggered(bool checked);

    void on_actFont_Italic_triggered(bool checked);

    void on_actFont_Underline_triggered(bool checked);

    void on_actSys_Toggle_triggered(bool checked);

    void on_actFile_Save_triggered();

    void on_plainTextEdit_copyAvailable(bool b);

    void on_plainTextEdit_selectionChanged();

    void on_actFile_New_triggered();

    void on_actFile_Open_triggered();

private:
    Ui::TestMainWindow_Action *ui;
};

#undef PRIVATE_DATA
#undef PRIVATE_FUNC
#endif // TESTMAINWINDOW_ACTION_H
