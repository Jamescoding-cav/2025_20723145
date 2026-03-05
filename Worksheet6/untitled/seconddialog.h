#ifndef SECONDDIALOG_H
#define SECONDDIALOG_H

#include <QDialog>
#include "ModelPart.h"

namespace Ui {
    class SecondDialog;
}

class SecondDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecondDialog(QWidget* parent = nullptr);
    ~SecondDialog();

    /** Load values from a ModelPart into the dialog widgets */
    void loadFromModelPart(ModelPart* part);

    /** Save dialog widget values back into a ModelPart */
    void saveToModelPart(ModelPart* part);

private:
    Ui::SecondDialog* ui;
};

#endif // SECONDDIALOG_H