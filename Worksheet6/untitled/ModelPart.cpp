/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"

  /* Commented out for now, will be uncommented later when you have
   * installed the VTK library
   */
   //#include <vtkSmartPointer.h>
   //#include <vtkDataSetMapper.h>


ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data),
    m_parentItem(parent),
    isVisible(true),
    colourR(255),
    colourG(255),
    colourB(255)
{
}


ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}


void ModelPart::appendChild(ModelPart* item) {
    item->m_parentItem = this;
    m_childItems.append(item);
}


ModelPart* ModelPart::child(int row) {
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    return m_childItems.count();
}

int ModelPart::columnCount() const {
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant& value) {
    if (column < 0 || column >= m_itemData.size())
        return;
    m_itemData.replace(column, value);
}

ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

int ModelPart::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    colourR = R;
    colourG = G;
    colourB = B;
}

unsigned char ModelPart::getColourR() {
    return colourR;
}

unsigned char ModelPart::getColourG() {
    return colourG;
}

unsigned char ModelPart::getColourB() {
    return colourB;
}

void ModelPart::setVisible(bool v) {
    isVisible = v;
}

bool ModelPart::visible() {
    return isVisible;
}

void ModelPart::loadSTL(QString fileName) {
    /* This is a placeholder function that you will need to modify if you want to use it */

    /* 1. Use the vtkSTLReader class to load the STL file
     *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */

     /* 2. Initialise the part's vtkMapper */

     /* 3. Initialise the part's vtkActor and link to the mapper */
}

//vtkSmartPointer<vtkActor> ModelPart::getActor() {
//}

//vtkActor* ModelPart::getNewActor() {
//    return nullptr;
//}

