#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "seconddialog.h"

#include <QHeaderView>
#include <QPushButton>
#include <QStatusBar>

#include <QFileDialog>
#include <QFileInfo>
#include <QModelIndex>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , partList(nullptr)
{
    ui->setupUi(this);

    // Model for TreeView
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    // Build a demo tree
    ModelPart* rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        const QString name = QString("TopLevel %1").arg(i);
        const QString visible = "true";

        ModelPart* top = new ModelPart({ name, visible });
        rootItem->appendChild(top);

        for (int j = 0; j < 5; j++) {
            const QString childName = QString("Item %1,%2").arg(i).arg(j);
            const QString childVisible = "true";

            ModelPart* child = new ModelPart({ childName, childVisible });
            top->appendChild(child);
        }
    }

    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->expandAll();

    // Status bar via signal
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);

    // Button 1
    connect(ui->pushButton, &QPushButton::released,
        this, &MainWindow::handleButton);

    // Button 2
    connect(ui->pushButton_2, &QPushButton::released,
        this, &MainWindow::handleButton2);

    // Tree view click - shows selected item name in status bar
    connect(ui->treeView, &QTreeView::clicked,
        this, &MainWindow::handleTreeClicked);

    // Add context menu action to tree view
    ui->treeView->addAction(ui->actionItem_Options);
}

MainWindow::~MainWindow()
{
    delete partList;
    delete ui;
}

void MainWindow::handleButton()
{
    emit statusUpdateMessage("Button 1 was clicked", 2000);
}

void MainWindow::handleButton2()
{
    // Get the currently selected tree item
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected - please select a tree item first", 2000);
        return;
    }

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    // Open the second dialog and pre-populate it with the part's current values
    SecondDialog dialog(this);
    dialog.loadFromModelPart(part);

    if (dialog.exec() == QDialog::Accepted) {
        // Save the edited values back to the part
        dialog.saveToModelPart(part);

        // Refresh the treeview so name/visible column updates are shown
        QModelIndex topLeft = partList->index(index.row(), 0, index.parent());
        QModelIndex bottomRight = partList->index(index.row(), 1, index.parent());
        emit partList->dataChanged(topLeft, bottomRight);

        emit statusUpdateMessage("Dialog accepted: " + part->data(0).toString(), 0);
    }
    else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}

void MainWindow::handleTreeClicked()
{
    // Get the currently selected item index
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    // Get a pointer to the selected ModelPart
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Get the name from column 0 and display in status bar
    QString name = selectedPart->data(0).toString();
    emit statusUpdateMessage(name + " selected", 0);
}

void MainWindow::on_actionOpenFile_triggered()
{
    const QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QString(),
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*.*)")
    );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("Open File cancelled", 0);
        return;
    }

    emit statusUpdateMessage("Selected: " + fileName, 0);

    // Rename currently selected tree item
    QModelIndex current = ui->treeView->currentIndex();
    if (!current.isValid()) {
        emit statusUpdateMessage("No tree item selected to rename", 0);
        return;
    }

    // Force rename to column 0 (name column)
    QModelIndex nameIndex = current.sibling(current.row(), 0);
    const QString baseName = QFileInfo(fileName).fileName();

    QAbstractItemModel* model = ui->treeView->model();
    if (!model) {
        emit statusUpdateMessage("No model attached to treeView", 4000);
        return;
    }

    if (!model->setData(nameIndex, baseName, Qt::EditRole)) {
        emit statusUpdateMessage("Rename failed (model setData returned false)", 5000);
        return;
    }

    emit statusUpdateMessage("Renamed selected item to: " + baseName, 4000);
}

void MainWindow::on_actionItem_Options_triggered()
{
    // Get the currently selected tree item
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected - right click a tree item first", 2000);
        return;
    }

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    // Open the second dialog and pre-populate it with the part's current values
    SecondDialog dialog(this);
    dialog.loadFromModelPart(part);

    if (dialog.exec() == QDialog::Accepted) {
        // Save the edited values back to the part
        dialog.saveToModelPart(part);

        // Refresh the treeview so name/visible column updates are shown
        QModelIndex topLeft = partList->index(index.row(), 0, index.parent());
        QModelIndex bottomRight = partList->index(index.row(), 1, index.parent());
        emit partList->dataChanged(topLeft, bottomRight);

        emit statusUpdateMessage("Item updated: " + part->data(0).toString(), 0);
    }
    else {
        emit statusUpdateMessage("Edit cancelled", 0);
    }
}