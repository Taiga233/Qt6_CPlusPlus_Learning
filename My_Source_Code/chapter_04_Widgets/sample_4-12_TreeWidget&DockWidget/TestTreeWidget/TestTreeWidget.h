#ifndef TESTTREEWIDGET_H
#define TESTTREEWIDGET_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QSpinBox>
#include <QLabel>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class TestTreeWidget; }
QT_END_NAMESPACE

#define Private_Data private
#define Private_Func private
class TestTreeWidget : public QMainWindow
{
    Q_OBJECT

public:
    TestTreeWidget(QWidget *parent = nullptr);
    ~TestTreeWidget();

Private_Data:
    //枚举类型treeItemType， 用于创建 QTreeWidgetItem 时作为节点的type, 自定义类型必须大于1000；itTopItem 顶层节点;  itGroupItem 组节点； itImageItem 图片。
    enum TreeItemType {itTopItem = 1001, itGroupItem, itImageItem, itHeader};
    enum treeColNum{colItem = 0, colType = 1, colDate}; //目录树列的编号
    QLabel *LB_fileName_M; //用于状态栏上显示文件名字
    QLabel *LB_nodeText_M; //用于状态栏上显示节点标题
    QLabel *LB_scaling_M; //缩放比例提示
    QSpinBox *SB_scaleRate_M; //缩放比例输入框
    QPixmap PM_currentImage_M; //当前的图片
    float F_ratio_M; //缩放的比例

Private_Func:
    //自定义控件初始化
    void buildWidget();
    //构建表头
    void buildTreeHeader();
    //目录树初始化
    void buildTreeItems();

    //添加一个目录节点
    void addFolderItem(QTreeWidgetItem *parItem, QString dirName);
    //从目录全名称中获取最后的文件夹名称
    QString getFinalFolderName(const QString &fullPathName) const;
    //添加一个图片节点
    void addImageItem(QTreeWidgetItem *parItem, QString aFilename);
    //显示一个图片节点的图片
    void displayImage(QTreeWidgetItem *item);
    //彻底删除一个节点，及其子节点
    void deleteItem(QTreeWidgetItem *parItem, QTreeWidgetItem *item);
    //遍历改变节点标题
    void changeItemCaption(QTreeWidgetItem *item);

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemCollapsed(QTreeWidgetItem *item);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemEntered(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemExpanded(QTreeWidgetItem *item);

    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemSelectionChanged();

    void on_actTool_AddFolder_triggered();

    void on_actTool_AddFile_triggered();

    void on_actTool_DeleteItem_triggered();

    void on_actTool_ScanItems_triggered();

    void on_actZoom_FitHeight_triggered();

    void on_actZoom_FitWidth_triggered();

    void on_actZoom_In_triggered();

    void on_actZoom_Out_triggered();

    void on_actZoom_RealSize_triggered();

    void on_actDock_Float_triggered(bool checked);

    void on_actDock_Visible_triggered(bool checked);

private:
    Ui::TestTreeWidget *ui;
};
#undef Private_Data
#undef Private_Func
#endif // TESTTREEWIDGET_H
