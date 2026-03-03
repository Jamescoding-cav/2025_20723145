#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Exercise 3: connect our custom signal to the status bar
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // Connect buttons to slots
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot for button 1
void MainWindow::handleButton()
{
    emit statusUpdateMessage("Button 1 was clicked", 0);
}

// Slot for button 2
void MainWindow::handleButton2()
{
    emit statusUpdateMessage("Button 2 was clicked", 0);
}
