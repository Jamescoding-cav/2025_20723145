#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "ModelPart.h"
#include "ModelPartList.h"

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void handleButton();
    void handleButton2();

    /** Called when user clicks an item in the tree view.
      * Displays the selected item name in the status bar.
      */
    void handleTreeClicked();

    /** Update the VTK renderer - removes all actors and re-adds from tree */
    void updateRender();

    /** Recursively iterate through tree and add actors to renderer */
    void updateRenderFromTree(const QModelIndex& index);

signals:
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    void on_actionOpenFile_triggered();
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow* ui;
    ModelPartList* partList;

    vtkSmartPointer<vtkRenderer>                    renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow>   renderWindow;
};

#endif // MAINWINDOW_H