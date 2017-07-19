#ifndef PSMDLGPATIENTSEL_H
#define PSMDLGPATIENTSEL_H

#include <QDialog>

namespace Ui {
class PsmDlgPatientSel;
}

class PsmDlgPatientSel : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPatientSel(QWidget *parent = 0);
    ~PsmDlgPatientSel();

private:
    Ui::PsmDlgPatientSel *ui;
};

#endif // PSMDLGPATIENTSEL_H
