#include "seconddialog.h"
#include "ui_seconddialog.h"

SecondDialog::SecondDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SecondDialog)
{
    ui->setupUi(this);
}

SecondDialog::~SecondDialog()
{
    delete ui;
}

void SecondDialog::loadFromModelPart(ModelPart* part)
{
    if (!part) return;

    // Load the part name into the line edit
    ui->nameEdit->setText(part->data(0).toString());

    // Load colour values into the spin boxes
    ui->spinR->setValue(part->getColourR());
    ui->spinG->setValue(part->getColourG());
    ui->spinB->setValue(part->getColourB());

    // Load visibility into the checkbox
    ui->visibleCheckBox->setChecked(part->visible());
}

void SecondDialog::saveToModelPart(ModelPart* part)
{
    if (!part) return;

    // Save name back to the part (column 0)
    part->set(0, ui->nameEdit->text());

    // Save colour
    part->setColour(
        (unsigned char)ui->spinR->value(),
        (unsigned char)ui->spinG->value(),
        (unsigned char)ui->spinB->value()
    );

    // Save visibility flag
    part->setVisible(ui->visibleCheckBox->isChecked());

    // Also update the visible column string (column 1) so treeview reflects it
    part->set(1, ui->visibleCheckBox->isChecked() ? "true" : "false");
}