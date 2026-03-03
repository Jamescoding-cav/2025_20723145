#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QPushButton>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , partList(nullptr)
{
    ui->setupUi(this);

    // -------------------------
    // Exercise 4: Model-based TreeView
    // -------------------------
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    // Build a demo tree
    ModelPart* rootItem = partList->getRootItem();

    // 3 top-level items, each with 5 children
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            QString childVisible("true");

            ModelPart* childChildItem = new ModelPart({ childName, childVisible });
            childItem->appendChild(childChildItem);
        }
    }

    // Make sure the text isn't truncated with "..."
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // -------------------------
    // Exercise 3: status bar updates via custom signal
    // -------------------------
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // Buttons -> slots
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);

    // Exercise 6 note:
    // If the action objectName is "actionOpenFile" and the slot name is
    // "on_actionOpenFile_triggered", Qt will auto-connect this for you.
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

// -------------------------
// Exercise 6: toolbar/menu action
// -------------------------
void MainWindow::on_actionOpenFile_triggered()
{
    // Open a file picker dialog
    const QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QString(),
        tr("All Files (*.*)")
        );

    if (filename.isEmpty()) {
        emit statusUpdateMessage("Open File cancelled", 2000);
        return;
    }

    // Show just the file name (not full path) in the status bar
    const QString baseName = QFileInfo(filename).fileName();
    emit statusUpdateMessage(QString("Opened: %1").arg(baseName), 4000);
}
