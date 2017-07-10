#ifndef PSMDLGPATIENT_H
#define PSMDLGPATIENT_H

#include <QDialog>

namespace Ui {
class PsmDlgPatient;
}

class PsmDlgPatient : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPatient(QWidget *parent = 0);
    ~PsmDlgPatient();

private:
    Ui::PsmDlgPatient *ui;
};

#endif // PSMDLGPATIENT_H
