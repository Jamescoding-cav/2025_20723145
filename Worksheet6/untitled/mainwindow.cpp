#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QHeaderView>
#include <QPushButton>
#include <QStatusBar>

#include <QFileDialog>
#include <QFileInfo>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , partList(nullptr)
{
    ui->setupUi(this);

    // Model for TreeView
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    // Build a demo tree (OK for worksheet exercises)
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

    // Buttons
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);
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
    emit statusUpdateMessage("Button 2 was clicked", 2000);
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

    // IMPORTANT: Explicit role (avoids signature mismatch / weird defaults)
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
