#include "TestListWidgetToolButton.h"
#include "ui_TestListWidgetToolButton.h"

#include <QRandomGenerator>
#include <QDebug>

//全局函数加入static限制后，只能在文件所在的编译单位内使用，不能在其它编译单位内使用。
//It's OK in C++11
static QListWidgetItem createAListWidgetItem(const bool &editable, const QString &text)
{
    QListWidgetItem t_Item(*new QIcon("://icons/check2.ico"), text);
    //    t_Item.setCheckState(Qt::Checked);
    t_Item.setCheckState((Qt::CheckState)(QRandomGenerator::global()->generate() % 3)); //设置复选状态
    t_Item.setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | (editable ? Qt::ItemIsEditable : Qt::NoItemFlags)); //设置可编辑状态
    return t_Item; //QListWidgetItem类没有父类
}

//如果不是放入对象树中，就不要用下面的函数了，否则需在外部delete掉指针
static QListWidgetItem *createAListWidgetItem(const bool &editable, const QString &text, const QString &iconPath)
{
    QListWidgetItem *t_Item = new QListWidgetItem(*new QIcon(iconPath), text);
    t_Item->setCheckState((Qt::CheckState)(QRandomGenerator::global()->generate() % 3)); //设置复选状态
    t_Item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | (editable ? Qt::ItemIsEditable : Qt::NoItemFlags));
    return t_Item;
}

TestListWidgetToolButton::TestListWidgetToolButton(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestListWidgetToolButton)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter); //设置中心组件
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu); //定制右键快捷菜单

    QFont font = this->font();
    font.setPointSize(font.pointSize() - 5);
    ui->actList_Init->setFont(font);
    ui->actList_Clear->setFont(font);
    ui->actList_Insert->setFont(font);
    ui->actList_Append->setFont(font);
    ui->actList_Delete->setFont(font);
    ui->actSelect_PopMenu->setFont(font);
    ui->actSys_Quit->setFont(font);

    setActionForToolButtion();
    createSelectionMenu();
}

TestListWidgetToolButton::~TestListWidgetToolButton()
{
    delete ui;
}

void TestListWidgetToolButton::setActionForToolButtion()
{
    //为QToolButtion设置对应的QAction，工具按钮的QToolButton::triggered(QAction *action)信号自动关联Action的QAction::triggered()信号，action->trigger()。
    ui->TB_02SelectAll->setDefaultAction(ui->actSelect_All);
    ui->TB_03SelectNone->setDefaultAction(ui->actSelect_None);
    ui->TB_04SelectInverse->setDefaultAction(ui->actSelect_Inverse);
    ui->TB_05Initialize->setDefaultAction(ui->actList_Init);
    ui->TB_06ClearList->setDefaultAction(ui->actList_Clear);
    ui->TB_07Insert->setDefaultAction(ui->actList_Insert);
    ui->TB_08Append->setDefaultAction(ui->actList_Append);
    ui->TB_09Delete->setDefaultAction(ui->actList_Delete);

    //重设一下字体大小
    ui->TB_05Initialize->setFont(this->font());
    ui->TB_06ClearList->setFont(this->font());
    ui->TB_07Insert->setFont(this->font());
    ui->TB_08Append->setFont(this->font());
    ui->TB_09Delete->setFont(this->font());
}

void TestListWidgetToolButton::createSelectionMenu()
{
    //创建下拉菜单，并将其设置为工具按钮的下拉菜单。
    popupMenu = new QMenu(this);
    popupMenu->addAction(ui->actSelect_All);
    popupMenu->addAction(ui->actSelect_None);
    popupMenu->addAction(ui->actSelect_Inverse);

    //设置菜单弹出模式，点击下拉小箭头按钮才会显示下拉菜单，点击工具按钮会执行关联的Aciton，而不会显示下拉菜单
    ui->TB_01SelectItem->setPopupMode(QToolButton::MenuButtonPopup);
    ui->TB_01SelectItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->TB_01SelectItem->setDefaultAction(ui->actSelect_PopMenu); //关联Action
    ui->TB_01SelectItem->setMenu(popupMenu); //设置下拉菜单
    ui->TB_01SelectItem->setFont(this->font()); //重设一下字体

    TB_00ItemSelection = new QToolButton(this);
    //设置立即显示下拉菜单，即使工具按钮关联了一个Action也不会执行这个Action
    TB_00ItemSelection->setPopupMode(QToolButton::InstantPopup);
    TB_00ItemSelection->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TB_00ItemSelection->setDefaultAction(ui->actSelect_PopMenu);
    TB_00ItemSelection->setMenu(popupMenu);

    ui->toolBar->addWidget(TB_00ItemSelection);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actSys_Quit);
}


void TestListWidgetToolButton::on_actList_Init_triggered()
{
    qDebug() << "on_actList_Init_triggered()";
    ui->listWidget->clear(); //信号与槽设计界面不能自动关联，clear可能会最后执行导致清空了

    QListWidgetItem *t_Item;
    QIcon t_Icon;
    t_Icon.addFile("://icons/check2.ico"); //设置图标
    bool editable = ui->CB_01Editable->isChecked(); //是否可编辑
    int randCount = (int)(QRandomGenerator::system()->generate() % 20);
    qDebug() << "item count:" << randCount;

    for (auto i = 0; i < randCount; i++) {
        t_Item = new QListWidgetItem();
        t_Item->setIcon(t_Icon);
        t_Item->setText(QString("Item %1").arg(i));
        t_Item->setCheckState((QRandomGenerator::securelySeeded().generate() & 1) ? Qt::Checked : (QRandomGenerator::global()->generate() & 1) ? Qt::Unchecked : Qt::PartiallyChecked);
        if (editable) {
            t_Item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        } else {
            t_Item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        }
        ui->listWidget->addItem(t_Item);
//        ui->listWidget->addItem(&t_Item); ////注意这里不能添加同一个指针地址作为item，详细看addItem()的帮助文档
    }
}


void TestListWidgetToolButton::on_actList_Clear_triggered()
{
    qDebug() << "on_actList_Clear_triggered()";
//    ui->listWidget->clear();
}

//在当前行上面插入一个Item
void TestListWidgetToolButton::on_actList_Insert_triggered()
{
    qDebug() << "on_actList_Insert_triggered()";
//    QIcon t_Icon("://icons/check2.ico"); //定义图标变量并直接赋值
//    QListWidgetItem item = createAListWidgetItem(ui->CB_01Editable->isChecked(), QString("Inserted Item"));
    ui->listWidget->insertItem(ui->listWidget->currentRow(), new QListWidgetItem(createAListWidgetItem(ui->CB_01Editable->isChecked(), QString("Inserted Item")))); //这样虽然可以，但会造成内存泄漏吗？不会，因为加入到了QListWidget中，对象由QListWidget管理。
////    ui->listWidget->insertItem(ui->listWidget->currentRow(), &createAListWidgetItem(ui->CB_01Editable->isChecked(), QString("Inserted Item"))); //这样为什么不行？临时类的地址？右值指针？ //用一个const引用来接收一个右值引用然后完成构造，所以上一行没问题。
}

//在最后一行后追加一个项
void TestListWidgetToolButton::on_actList_Append_triggered()
{
    qDebug() << "on_actList_Append_triggered()";
    ui->listWidget->addItem(createAListWidgetItem(ui->CB_01Editable->isChecked(), "Appended Item", "://icons/check2.ico"));
}

//删除当前项
void TestListWidgetToolButton::on_actList_Delete_triggered()
{
    qDebug() << "on_actList_Delete_triggered()";
    QListWidgetItem *deleteItem = ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete deleteItem; ////这里需要手动删除对象！
}


void TestListWidgetToolButton::on_actSelect_All_triggered()
{
    qDebug() << "on_actSelect_All_triggered()";
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        !ui->listWidget->item(i)->isSelected() && (ui->listWidget->item(i)->setCheckState(Qt::Checked), true);
    }
//    for (auto item : qAsConst(*(ui->listWidget)))
}


void TestListWidgetToolButton::on_actSelect_None_triggered()
{
    qDebug() << "on_actSelect_None_triggered()";
    for (int i = 0; i < ui->listWidget->count(); i++) {
        (ui->listWidget->item(i)->checkState() != Qt::Unchecked) && (ui->listWidget->item(i)->setCheckState(Qt::Unchecked), true);
    }
}


void TestListWidgetToolButton::on_actSelect_Inverse_triggered()
{
    qDebug() << "on_actSelect_Inverse_triggered()";
    QListWidgetItem *item;
    int cnt = ui->listWidget->count();
    for (int i = 0; i < cnt; i++) {
        item = ui->listWidget->item(i);
        switch (item->checkState()) {
        case Qt::Unchecked:
            item->setCheckState(Qt::Checked);
            break;
        case Qt::Checked:
            item->setCheckState(Qt::Unchecked);
            break;
        default:
            item->setCheckState(Qt::PartiallyChecked);
            break;
        }
    }
}


void TestListWidgetToolButton::on_CB_02Sortable_clicked(bool checked)
{
    qDebug() << "on_CB_02Sortable_clicked()";
    //设置列表是否可排序，sortingEnabled默认值为false。注意，即使这个属性值为false，也可以使用函数sortItems()进行列表的排序。
    //如果sortingEnabled值为true，通过运行addItem()和insertItem()新增的项将添加到列表的最前面，所以，如果要确保添加和插入的项操作正常，属性sortingEnabled的值应该设置为false。
    ui->listWidget->setSortingEnabled(checked);
}


void TestListWidgetToolButton::on_TB_10Ascending_clicked()
{
    qDebug() << "on_TB_10Ascending_clicked()";
    ui->listWidget->sortItems();
}


void TestListWidgetToolButton::on_TB_11Descending_clicked()
{
    qDebug() << "on_TB_11Descending_clicked()";
    ui->listWidget->sortItems(Qt::DescendingOrder);
}


void TestListWidgetToolButton::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString show;
    if (current != Q_NULLPTR) { //需要检测指针是否为空！
        //判断前一项指针是否为空
        if (previous == nullptr) {
            show = "current:" + current->text();
        } else {
            show = "current:" + current->text() + " previous:" + previous->text();
        }
        ui->lineEdit->setText(show);
    }
    ui->plainTextEdit->appendPlainText(QString("SIGNAL currentItemChanged() has been emited."));
}


void TestListWidgetToolButton::on_listWidget_currentRowChanged(int currentRow)
{
    ui->plainTextEdit->appendPlainText(QString("SIGNAL currentRowChanged() has been emited, current row:%1").arg(currentRow));
}


void TestListWidgetToolButton::on_listWidget_currentTextChanged(const QString &currentText)
{
    ui->plainTextEdit->appendPlainText(QString("SIGNAL currentTextChanged() has been emited, current text:%1").arg(currentText));
}


void TestListWidgetToolButton::on_listWidget_itemSelectionChanged()
{
    ui->plainTextEdit->appendPlainText(QString("SIGNAL itemSelectionChanged() has been emited."));
}


void TestListWidgetToolButton::on_listWidget_itemChanged(QListWidgetItem *item)
{
    //如果不需要点击项来改变复选状态checkState，可以按下空格键，此时只会发射itemChanged()信号。
    ui->plainTextEdit->appendPlainText(QString("SIGNAL itemChanged() has been emited, current text:%1, current checkstate: %2").arg(item->text()).arg(item->checkState()));
}


void TestListWidgetToolButton::on_listWidget_itemActivated(QListWidgetItem *item)
{
    //当光标停留在某个项上，按Enter键时发射此信号
    ui->plainTextEdit->appendPlainText(QString("SIGNAL itemActivated() has been emited, current text:%1").arg(item->text()));
}


void TestListWidgetToolButton::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //当鼠标双击某个项时发射此信号
    ui->plainTextEdit->appendPlainText(QString("SIGNAL itemDoubleClicked() has been emited, current text:%1").arg(item->text()));
}


void TestListWidgetToolButton::on_listWidget_itemEntered(QListWidgetItem *item)
{
    //如果QListWidget的mouseTracking属性被设置为true，当鼠标移动到某个项上时，组件就会发射itemEntered()信号。
    //mouseTracking属性的默认值为false
    ui->plainTextEdit->appendPlainText(QString("SIGNAL itemEntered() has been emited, current text:%1").arg(item->text()));
}


void TestListWidgetToolButton::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //点击
    ui->plainTextEdit->appendPlainText(QString("SIGNAL itemClicked() has been emited, current text:%1").arg(item->text()));
}


void TestListWidgetToolButton::on_listWidget_itemPressed(QListWidgetItem *item)
{
    //鼠标左键或右键按下
    //点击一个项时，不管是否发生了当前项的切换，都会发射itemPressed()和itemClicked()信号。
    //在一个项上按下鼠标右键时只会发射itemPressed()信号，而不会发射itemClicked()信号。
    ui->plainTextEdit->appendPlainText(QString("SIGNAL itemPressed() has been emited, current text:%1").arg(item->text()));
}


void TestListWidgetToolButton::on_CB_03MouseTracking_clicked(bool checked)
{
    ui->listWidget->setMouseTracking(checked);
}


void TestListWidgetToolButton::on_TB_13AppendSpace_clicked()
{
    ui->plainTextEdit->appendPlainText(QString(""));
}


void TestListWidgetToolButton::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    //点击右键发射此信号，前提是QListWidget的contextMenuPolicy属性设置为Qt::CustomContextMenu
    Q_UNUSED(pos);
    qDebug() << "On SIGNAL customContextMenuRequested()" << "position:" << pos << "QCursor::pos():" << QCursor::pos();
    QMenu *menuList = new QMenu(this); //局部对象指针
    menuList->addAction(ui->actList_Init);
    menuList->addAction(ui->actList_Clear);
    menuList->addAction(ui->actList_Insert);
    menuList->addAction(ui->actList_Append);
    menuList->addAction(ui->actList_Delete);
    menuList->addSeparator();
    menuList->addAction(ui->actSelect_All);
    menuList->addAction(ui->actSelect_None);
    menuList->addAction(ui->actSelect_Inverse);

    menuList->exec(QCursor::pos()); //在鼠标光标位置显示快捷菜单
    delete menuList; //菜单显示完后，需要删除对象
}

