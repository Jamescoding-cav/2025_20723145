#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "seconddialog.h"

#include <QHeaderView>
#include <QPushButton>
#include <QStatusBar>
#include <QFileDialog>
#include <QFileInfo>
#include <QModelIndex>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

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

    // -------------------------------------------------------
    // VTK Setup
    // -------------------------------------------------------

    /* Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->SetSize(800, 600);
    ui->vtkWidget->setRenderWindow(renderWindow);

    /* Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.1, 0.1, 0.1);
    renderWindow->AddRenderer(renderer);

    /* Create a cylinder and add to renderer as a test object */
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    /* Reset camera */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    /* Force render through the widget's own render window reference */
    ui->vtkWidget->renderWindow()->Render();
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
        dialog.saveToModelPart(part);

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
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString name = selectedPart->data(0).toString();
    emit statusUpdateMessage(name + " selected", 0);
}

void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->ResetCamera();
    ui->vtkWidget->renderWindow()->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        /* If part is visible, add its actor to the renderer */
        if (selectedPart->visible()) {
            vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
            if (actor != nullptr) {
                renderer->AddActor(actor);
            }
        }
    }

    /* Check if this part has children */
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    /* Loop through children and add their actors */
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
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

    emit statusUpdateMessage("Loading: " + fileName, 0);

    /* Get currently selected tree item - add as child of it if valid */
    QModelIndex currentIndex = ui->treeView->currentIndex();

    /* Add a new item to the tree */
    QList<QVariant> data;
    data << QFileInfo(fileName).fileName() << "true";

    QModelIndex newIndex = partList->appendChild(currentIndex, data);

    /* Get the newly created ModelPart and load the STL */
    ModelPart* newPart = static_cast<ModelPart*>(newIndex.internalPointer());
    if (newPart) {
        newPart->loadSTL(fileName);
    }

    /* Refresh the renderer */
    updateRender();

    /* Refresh the tree view */
    ui->treeView->expandAll();

    emit statusUpdateMessage("Loaded: " + QFileInfo(fileName).fileName(), 4000);
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected - right click a tree item first", 2000);
        return;
    }

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    SecondDialog dialog(this);
    dialog.loadFromModelPart(part);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToModelPart(part);

        QModelIndex topLeft = partList->index(index.row(), 0, index.parent());
        QModelIndex bottomRight = partList->index(index.row(), 1, index.parent());
        emit partList->dataChanged(topLeft, bottomRight);

        /* Refresh the renderer in case visibility changed */
        updateRender();

        emit statusUpdateMessage("Item updated: " + part->data(0).toString(), 0);
    }
    else {
        emit statusUpdateMessage("Edit cancelled", 0);
    }
}