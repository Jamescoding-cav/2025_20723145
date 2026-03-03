#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    // Optional: show everything expanded while testing
    // ui->treeView->expandAll();

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
}

MainWindow::~MainWindow()
{
    delete partList;
    delete ui;
}

void MainWindow::handleButton()
{
    emit statusUpdateMessage("Button 1 was clicked", 0);
}

void MainWindow::handleButton2()
{
    emit statusUpdateMessage("Button 2 was clicked", 0);
}
