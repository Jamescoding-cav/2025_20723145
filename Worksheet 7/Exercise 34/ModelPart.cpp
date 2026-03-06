/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"

#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkProperty.h>

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

    /* If actor already exists, update its colour immediately */
    if (actor != nullptr) {
        actor->GetProperty()->SetColor(
            (double)R / 255.0,
            (double)G / 255.0,
            (double)B / 255.0
        );
    }
}

unsigned char ModelPart::getColourR() { return colourR; }
unsigned char ModelPart::getColourG() { return colourG; }
unsigned char ModelPart::getColourB() { return colourB; }

void ModelPart::setVisible(bool v) {
    isVisible = v;
    if (actor != nullptr) {
        actor->SetVisibility(isVisible ? 1 : 0);
    }
}

bool ModelPart::visible() {
    return isVisible;
}

void ModelPart::loadSTL(QString fileName) {
    /* 1. Use the vtkSTLReader class to load the STL file */
    file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();

    /* 2. Initialise the part's vtkMapper */
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(file->GetOutputPort());

    /* 3. Initialise the part's vtkActor and link to the mapper */
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    /* Set colour from stored RGB values */
    actor->GetProperty()->SetColor(
        (double)colourR / 255.0,
        (double)colourG / 255.0,
        (double)colourB / 255.0
    );

    /* Set visibility */
    actor->SetVisibility(isVisible ? 1 : 0);
}

vtkSmartPointer<vtkActor> ModelPart::getActor() {
    return actor;
}

vtkActor* ModelPart::getNewActor() {
    /* Create a new mapper for VR use */
    vtkSmartPointer<vtkPolyDataMapper> newMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    newMapper->SetInputConnection(file->GetOutputPort());

    /* Create new actor and link to mapper */
    vtkActor* newActor = vtkActor::New();
    newActor->SetMapper(newMapper);

    /* Link properties from original actor so colour/visibility are shared */
    newActor->SetProperty(actor->GetProperty());

    return newActor;
}