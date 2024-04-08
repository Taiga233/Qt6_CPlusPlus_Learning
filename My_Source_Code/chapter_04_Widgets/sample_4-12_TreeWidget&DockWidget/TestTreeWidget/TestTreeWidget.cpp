#include "TestTreeWidget.h"
#include "ui_TestTreeWidget.h"

#include <QDebug>
#include <QFileDialog>

TestTreeWidget::TestTreeWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestTreeWidget)
{
    ui->setupUi(this);

    buildWidget();
    buildTreeHeader();
    buildTreeItems();

    //将scrollArea设置为主窗口工作区的中心组件后，dockWidget与scrollArea之间会自动出现分割条
    setCentralWidget(ui->scrollArea);
}

TestTreeWidget::~TestTreeWidget()
{
    delete ui;
}

void TestTreeWidget::buildWidget()
{
    LB_nodeText_M = new QLabel("节点标题", this);
    LB_nodeText_M->setMinimumWidth(200);
    ui->statusbar->addWidget(LB_nodeText_M);

    LB_scaling_M = new QLabel("缩放比例：", this);
    ui->statusbar->addPermanentWidget(LB_scaling_M);

    SB_scaleRate_M = new QSpinBox(this);
    SB_scaleRate_M->setRange(0, 1000);
    SB_scaleRate_M->setValue(100);
    SB_scaleRate_M->setSuffix("%");
    SB_scaleRate_M->setReadOnly(true);
    SB_scaleRate_M->setButtonSymbols(QAbstractSpinBox::NoButtons); //不显示右侧调节按钮
    ui->statusbar->addPermanentWidget(SB_scaleRate_M);

    LB_fileName_M = new QLabel("文件名：", this);
    LB_fileName_M->setMaximumWidth(400);
    ui->statusbar->addPermanentWidget(LB_fileName_M);

    QFont font = this->font();
    font.setPointSize(font.pointSize() - 6);
    ui->actDock_Float->setFont(font);
    ui->actDock_Visible->setFont(font);
    ui->actTool_AddFile->setFont(font);
    ui->actTool_AddFolder->setFont(font);
    ui->actTool_DeleteItem->setFont(font);
    ui->actTool_Quit->setFont(font);
    ui->actTool_ScanItems->setFont(font);
    ui->actZoom_FitHeight->setFont(font);
    ui->actZoom_FitWidth->setFont(font);
    ui->actZoom_In->setFont(font);
    ui->actZoom_Out->setFont(font);
    ui->actZoom_RealSize->setFont(font);
}

void TestTreeWidget::buildTreeHeader()
{
    //重新构建treeWidget的表头
    ui->treeWidget->clear();
    QTreeWidgetItem *header = new QTreeWidgetItem(TestTreeWidget::itHeader);
    header->setText(TestTreeWidget::colItem, "目录和文件");
    header->setText(TestTreeWidget::colType, "目录和文件");
    header->setText(TestTreeWidget::colDate, "目录和文件");
    header->setTextAlignment(TestTreeWidget::colItem, Qt::AlignLeft | Qt::AlignVCenter);
    header->setTextAlignment(TestTreeWidget::colType, Qt::AlignHCenter | Qt::AlignVCenter);
    header->setTextAlignment(TestTreeWidget::colDate, Qt::AlignCenter);
    ui->treeWidget->setHeaderItem(header);
}

void TestTreeWidget::buildTreeItems()
{
    //初始化目录树，创建一个顶层节点
    QTreeWidgetItem *topItem = new QTreeWidgetItem(TestTreeWidget::itTopItem);
    topItem->setIcon(TestTreeWidget::colItem, *new QIcon("://icons/15.ico"));
    topItem->setText(TestTreeWidget::colItem, "图片");
    topItem->setText(TestTreeWidget::colType, "Top Item");
    topItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    topItem->setCheckState(TestTreeWidget::colItem, Qt::Checked);
    ui->treeWidget->addTopLevelItem(topItem);
}

void TestTreeWidget::addFolderItem(QTreeWidgetItem *parItem, QString dirName)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parItem, (QStringList() << getFinalFolderName(dirName) << "Group Item"), TestTreeWidget::itGroupItem);
    item->setIcon(TestTreeWidget::colItem, QIcon("://icons/open3.bmp"));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(TestTreeWidget::colItem, Qt::Checked);
    item->setData(TestTreeWidget::colItem, Qt::UserRole, QVariant(dirName)); //设置用户数据，存储完整的目录名称
}

QString TestTreeWidget::getFinalFolderName(const QString &fullPathName) const
{
    QString str = fullPathName.right(fullPathName.length() - (fullPathName.lastIndexOf("/") + 1));
    qDebug() << "Folder Name:" << str;
    return str;
}

void TestTreeWidget::addImageItem(QTreeWidgetItem *parItem, QString aFilename)
{
    QIcon icon("://icons/31.ico");
    QFileInfo fileinfo(aFilename); //QfileInfo用于获取文件信息
    QString nodeText = fileinfo.fileName(); //不带有路径的文件名
    QDateTime dateTime = fileinfo.lastModified(); //文件的最后修改日期时间
    QTreeWidgetItem *item = new QTreeWidgetItem(TestTreeWidget::itImageItem);

    item->setIcon(TestTreeWidget::colItem, icon);
    item->setText(colItem, nodeText);
    item->setText(colType, "Image Item");
    item->setText(colDate, dateTime.toString("yyyy-MM-dd hh:mm"));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(colItem, Qt::Checked);
    item->setData(colItem, Qt::UserRole, QVariant(aFilename));
    parItem->addChild(item);
}

void TestTreeWidget::displayImage(QTreeWidgetItem *item)
{
    QString fileName = item->data(TestTreeWidget::colItem, Qt::UserRole).toString();
    LB_fileName_M->setText(fileName);
    LB_nodeText_M->setText(item->text(TestTreeWidget::colItem));
    PM_currentImage_M.load(fileName); //从文件加载图片
    ui->actZoom_FitHeight->trigger(); //直接运行槽函数trigger()，使其触发triggered()信号，使其运行关联的槽函数
    ui->actZoom_FitHeight->setEnabled(true);
    ui->actZoom_FitWidth->setEnabled(true);
    ui->actZoom_In->setEnabled(true);
    ui->actZoom_Out->setEnabled(true);
    ui->actZoom_RealSize->setEnabled(true);
}

void TestTreeWidget::deleteItem(QTreeWidgetItem *parItem, QTreeWidgetItem *item)
{
    //递归删除
    if (item->childCount() > 0) {
        for (int i = item->childCount() - 1; i >= 0; i--) {
            deleteItem(item, item->child(i)); //递归调用自己删除孩子节点
        }
    }
    parItem->removeChild(item);
    delete item; //需要释放内存
}

void TestTreeWidget::changeItemCaption(QTreeWidgetItem *item)
{
    QString caption = "*" + item->text(TestTreeWidget::colItem);
    item->setText(TestTreeWidget::colItem, caption);
    if (item->childCount() > 0) {
        for (int i = 0; i < item->childCount(); i++) {
            changeItemCaption(item->child(i));
        }
    }
}


void TestTreeWidget::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    qDebug() << "SINGAL currentItemChanged() is emitted" << ((current == Q_NULLPTR) ? "current is nullptr" : "current text:" + current->text(0)) << ((previous == Q_NULLPTR) ? "previous is nullptr" : "previous text:" + previous->text(0));

    if (current == Q_NULLPTR) return;
    if (current == previous) return; //没有切换节点，列的变化也会发射此信号
    switch (current->type()) {
    case itTopItem:
        ui->actTool_AddFile->setEnabled(true);
        ui->actTool_AddFolder->setEnabled(true);
        ui->actTool_DeleteItem->setEnabled(false);
        break;
    case itGroupItem:
        ui->actTool_AddFile->setEnabled(true);
        ui->actTool_AddFolder->setEnabled(true);
        ui->actTool_DeleteItem->setEnabled(true);
        break;
    case itImageItem:
        ui->actTool_AddFile->setEnabled(false); //图片节点下不能添加目录节点
        ui->actTool_AddFolder->setEnabled(true);
        ui->actTool_DeleteItem->setEnabled(true);
        displayImage(current);
        break;
    default:
        break;
    }
}


void TestTreeWidget::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug() << "SINGAL itemActivated() is emitted" << "item text:" << item->text(column) << "column:" << column;
}


void TestTreeWidget::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug() << "SINGAL itemChanged() is emitted" << "item text:" << item->text(column) << "column:" << column;
}


void TestTreeWidget::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug() << "SINGAL itemClicked() is emitted" << "item text:" << item->text(column) << "column:" << column;
}


void TestTreeWidget::on_treeWidget_itemCollapsed(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
    qDebug() << "SINGAL itemCollapsed() is emitted" << "item text:" << item->text(0);
}


void TestTreeWidget::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug() << "SINGAL itemDoubleClicked() is emitted" << "item text:" << item->text(column) << "column:" << column;
}


void TestTreeWidget::on_treeWidget_itemEntered(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug() << "SINGAL itemEntered() is emitted" << "item text:" << item->text(column) << "column:" << column;
}


void TestTreeWidget::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
    qDebug() << "SINGAL itemExpanded() is emitted" << "item text:" << item->text(0);
}


void TestTreeWidget::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug() << "SINGAL itemPressed() is emitted" << "item text:" << item->text(column) << "column:" << column;
}


void TestTreeWidget::on_treeWidget_itemSelectionChanged()
{
    qDebug() << "SINGAL itemSelectionChanged() is emitted";
}


void TestTreeWidget::on_actTool_AddFolder_triggered()
{
    //只有目录树上的当前节点类型是itTopItem或itGroupItem时才可以添加分组节点
    QString dir = QFileDialog::getExistingDirectory(); //选择目录
    if (dir.isEmpty()) return;
    QTreeWidgetItem *parentItem = ui->treeWidget->currentItem();
    if (parentItem == Q_NULLPTR) return;
    if (parentItem->type() == TestTreeWidget::itGroupItem || parentItem->type() == TestTreeWidget::itTopItem) {
        addFolderItem(parentItem, dir);
    }
    parentItem->setExpanded(true);
}


void TestTreeWidget::on_actTool_AddFile_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "选择文件", "", "Images(*.jpg *.png);;All(*.*)");
    if (files.isEmpty()) return;
    QTreeWidgetItem *parentItem, *item;
    item = ui->treeWidget->currentItem();
    if (item == Q_NULLPTR) {
        item = ui->treeWidget->topLevelItem(0); //如果当前节点是空就取顶层节点
    }
    if (item->type() == TestTreeWidget::itImageItem) {
        //如果当前节点是图片节点，取其父节点作为将要添加的图片节点的父节点
        parentItem = item;
    } else if (item->type() == TestTreeWidget::itHeader) {
        parentItem = ui->treeWidget->topLevelItem(0);
    } else {
        parentItem = item;
    }

    for (int i = 0; i < files.size(); i++) {
        addImageItem(parentItem, files.at(i));
    }
    parentItem->setExpanded(true); //展开父节点
}


void TestTreeWidget::on_actTool_DeleteItem_triggered()
{
    if (ui->treeWidget->currentItem() == Q_NULLPTR) return;
    //彻底删除一个节点及其子节点
    deleteItem(ui->treeWidget->currentItem()->parent(), ui->treeWidget->currentItem());
}


void TestTreeWidget::on_actTool_ScanItems_triggered()
{
    //遍历所有的顶层节点，修改标题
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        changeItemCaption(ui->treeWidget->topLevelItem(i));
    }
}


void TestTreeWidget::on_actZoom_FitHeight_triggered()
{
    int h = ui->scrollArea->height();
    int realH = PM_currentImage_M.height();
    F_ratio_M = float(h) / realH;
    SB_scaleRate_M->setValue(100 * F_ratio_M);
    ui->label_pictureExhibition->setPixmap(PM_currentImage_M.scaledToHeight(h));
}


void TestTreeWidget::on_actZoom_FitWidth_triggered()
{
    int w = ui->scrollArea->height();
    int realW = PM_currentImage_M.height();
    F_ratio_M = float(w) / realW;
    SB_scaleRate_M->setValue(100 * F_ratio_M);
    ui->label_pictureExhibition->setPixmap(PM_currentImage_M.scaledToWidth(w));
}


void TestTreeWidget::on_actZoom_In_triggered()
{
    F_ratio_M *= 1.2;
    SB_scaleRate_M->setValue(100 * F_ratio_M);
    int w = F_ratio_M * PM_currentImage_M.width();
    int h = F_ratio_M * PM_currentImage_M.height();
    ui->label_pictureExhibition->setPixmap(PM_currentImage_M.scaled(w, h, Qt::KeepAspectRatio)); //保持长宽比例缩放
}


void TestTreeWidget::on_actZoom_Out_triggered()
{
    F_ratio_M *= 0.8;
    SB_scaleRate_M->setValue(100 * F_ratio_M);
    int w = F_ratio_M * PM_currentImage_M.width();
    int h = F_ratio_M * PM_currentImage_M.height();
    ui->label_pictureExhibition->setPixmap(PM_currentImage_M.scaled(w, h, Qt::KeepAspectRatio));
}


void TestTreeWidget::on_actZoom_RealSize_triggered()
{
    F_ratio_M = 1.0;
    SB_scaleRate_M->setValue(100);
    ui->label_pictureExhibition->setPixmap(PM_currentImage_M);
}


void TestTreeWidget::on_actDock_Float_triggered(bool checked)
{
    ui->dockWidget->setFloating(checked);
}


void TestTreeWidget::on_actDock_Visible_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}

