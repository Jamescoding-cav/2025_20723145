/**     @file ModelPart.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkColor.h>
#include <vtkPolyDataMapper.h>

class ModelPart {
public:
    /** Constructor
     * @param data is a List (array) of strings for each property of this item
     * @param parent is the parent of this item (one level up in tree)
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Destructor */
    ~ModelPart();

    /** Add a child to this item. */
    void appendChild(ModelPart* item);

    /** Return child at position 'row' below this item */
    ModelPart* child(int row);

    /** Return number of children */
    int childCount() const;

    /** Get number of data items */
    int columnCount() const;

    /** Return the data item at a particular column */
    QVariant data(int column) const;

    /** Set data for a column */
    void set(int column, const QVariant& value);

    /** Get pointer to parent item */
    ModelPart* parentItem();

    /** Get row index of item, relative to parent item */
    int row() const;

    /** Set colour (0-255 RGB values) */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);
    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();

    /** Set visible flag */
    void setVisible(bool isVisible);

    /** Get visible flag */
    bool visible();

    /** Load STL file and set up VTK pipeline
      * @param fileName path to the STL file
      */
    void loadSTL(QString fileName);

    /** Return actor for GUI rendering
      * @return pointer to vtkActor
      */
    vtkSmartPointer<vtkActor> getActor();

    /** Return new actor for VR rendering */
    vtkActor* getNewActor();

private:
    QList<ModelPart*>                           m_childItems;
    QList<QVariant>                             m_itemData;
    ModelPart* m_parentItem;

    bool                                        isVisible;
    unsigned char                               colourR;
    unsigned char                               colourG;
    unsigned char                               colourB;

    vtkSmartPointer<vtkSTLReader>               file;
    vtkSmartPointer<vtkPolyDataMapper>          mapper;
    vtkSmartPointer<vtkActor>                   actor;
};

#endif