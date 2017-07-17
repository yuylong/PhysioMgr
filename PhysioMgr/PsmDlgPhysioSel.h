#ifndef PSMDLGPHYSIOSEL_H
#define PSMDLGPHYSIOSEL_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgPhysioSel;
}

class PsmDlgPhysioSel : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPhysioSel(QWidget *parent = 0);
    ~PsmDlgPhysioSel();

private:
    Ui::PsmDlgPhysioSel *ui;
    PsmService *service;
};

#endif // PSMDLGPHYSIOSEL_H
