#include "TestMainWindow_Action.h"
#include "ui_TestMainWindow_Action.h"

TestMainWindow_Action::TestMainWindow_Action(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestMainWindow_Action)
{
    ui->setupUi(this);
    buildCustomUI();
    buildSignalSlots();
    //设置工具按钮栏只显示图标
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    //设置plainTextEdit填充满工作区
    this->setCentralWidget(ui->plainTextEdit);
}

TestMainWindow_Action::~TestMainWindow_Action()
{
    delete ui;
}

void TestMainWindow_Action::buildCustomUI()
{
    //传递了this指针，也就是用窗口作为所创建组件的父容器，将其加入到窗口的对象树中，这样当窗口被删除时，其所有子组件都会被自动删除，就不需要delete删除指针了，同时也不需要考虑内存泄露的问题
    labStauts_file = new QLabel(this);
    labStauts_file->setMinimumWidth(150);
    labStauts_file->setText(QString("文件名："));
    ui->statusbar->addWidget(labStauts_file);

    labStauts_Info = new QLabel(this);
    labStauts_Info->setText(QString("选择字体的名称："));
    ui->statusbar->addPermanentWidget(labStauts_Info);

    pgbStatus_show = new QProgressBar(this);
    pgbStatus_show->setMaximumWidth(200);
    pgbStatus_show->setMinimum(5);
    pgbStatus_show->setMaximum(50);
    pgbStatus_show->setFormat("%p%");
    qDebug() << "pointSize:" << ui->plainTextEdit->font().pointSize();
    pgbStatus_show->setValue(ui->plainTextEdit->font().pointSize());
    ui->statusbar->addWidget(pgbStatus_show);

    sbTool_font = new QSpinBox(this);
    sbTool_font->setMinimum(5);
    sbTool_font->setMaximum(50);
    sbTool_font->setValue(ui->plainTextEdit->font().pointSize());
    sbTool_font->setMinimumWidth(50);
    ui->toolBar->addWidget(sbTool_font);

    fcbTool_fontName = new QFontComboBox(this);
    fcbTool_fontName->setMinimumWidth(150);
    ui->toolBar->addWidget(fcbTool_fontName);
    ui->toolBar->addSeparator();
    //手动添加Action到工具栏最后
    ui->toolBar->addAction(ui->actTool_Close);

    agAction_language = new QActionGroup(this);
    agAction_language->addAction(ui->actLanguage_CN);
    agAction_language->addAction(ui->actLanguage_En);
    agAction_language->setExclusive(true); //设置互斥选择
    ui->actLanguage_CN->setChecked(true);
}

void TestMainWindow_Action::buildSignalSlots()
{
    connect(sbTool_font, SIGNAL(valueChanged(int)), this, SLOT(do_fontSize_change(int)));
    connect(fcbTool_fontName, &QFontComboBox::currentFontChanged, this, &TestMainWindow_Action::do_fontSelected);
}

void TestMainWindow_Action::do_fontSize_change(int fontSize)
{
    qDebug() << "fontSize:" << fontSize;
    QFont font = ui->plainTextEdit->font();
    font.setPointSize(fontSize);
    ui->plainTextEdit->setFont(font);
    /*
     * 不知为何下面这种方式没法用
    QTextCharFormat format = ui->plainTextEdit->currentCharFormat();
    format.setFontPointSize(fontSize);
    ui->plainTextEdit->mergeCurrentCharFormat(format);
    */
    pgbStatus_show->setValue(fontSize);
}

void TestMainWindow_Action::do_fontSelected(const QFont &font)
{
    labStauts_Info->setText(QString("字体名称：%1").arg(font.family()));
    /*
     * 下面这种方法设置字体不成功
    QTextCharFormat format = ui->plainTextEdit->currentCharFormat();
    format.setFont(font);
    ui->plainTextEdit->mergeCurrentCharFormat(format);
    */
    ui->plainTextEdit->setFont(font);
}


void TestMainWindow_Action::on_actFont_Bold_triggered(bool checked)
{
    #if 1
    QTextCharFormat format = ui->plainTextEdit->currentCharFormat();
    if (checked) {
        format.setFontWeight(QFont::Bold);
    } else {
        format.setFontWeight(QFont::Normal);
    }
    #else //两种方式都可以设置字体
    QFont font = ui->plainTextEdit->font();
    font.setBold(checked);
    ui->plainTextEdit->setFont(font);
    #endif
}


void TestMainWindow_Action::on_actFont_Italic_triggered(bool checked)
{
    QTextCharFormat format = ui->plainTextEdit->currentCharFormat();
    format.setFontItalic(checked);
    ui->plainTextEdit->mergeCurrentCharFormat(format);
}


void TestMainWindow_Action::on_actFont_Underline_triggered(bool checked)
{
    QTextCharFormat format = ui->plainTextEdit->currentCharFormat();
    format.setFontUnderline(checked);
    ui->plainTextEdit->mergeCurrentCharFormat(format);
}


void TestMainWindow_Action::on_actSys_Toggle_triggered(bool checked)
{
    //是否显示工具栏按钮的文字
    if (checked) {
        ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}


void TestMainWindow_Action::on_actFile_Save_triggered()
{
    //表示已经保存过了改变修改状态
    ui->plainTextEdit->document()->setModified(false);
    labStauts_file->setText(QString("文件已保存"));
}


void TestMainWindow_Action::on_plainTextEdit_copyAvailable(bool b)
{
    Q_UNUSED(b);
    //当每一次有文字被选择或者被取消选择的时候发送信号，判断是否能粘贴，然后设置粘贴action的使能状态
    ui->actEdit_Paste->setEnabled(ui->plainTextEdit->canPaste());
}


void TestMainWindow_Action::on_plainTextEdit_selectionChanged()
{
    QTextCharFormat format = ui->plainTextEdit->currentCharFormat();
    ui->actFont_Bold->setChecked(format.font().bold());
    ui->actFont_Italic->setChecked(format.fontItalic());
    ui->actFont_Underline->setChecked(format.fontUnderline());
}


void TestMainWindow_Action::on_actFile_New_triggered()
{
    ui->plainTextEdit->clear();
    //表示已经保存过了,改变修改状态
    ui->plainTextEdit->document()->setModified(false);
    labStauts_file->setText(QString("新建了一个文件"));
}


void TestMainWindow_Action::on_actFile_Open_triggered()
{
    labStauts_file->setText(QString("打开了一个文件"));
}

