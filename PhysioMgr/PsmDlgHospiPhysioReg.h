#ifndef PSMDLGHOSPIPHYSIOREG_H
#define PSMDLGHOSPIPHYSIOREG_H

#include <QDialog>

namespace Ui {
class PsmDlgHospiPhysioReg;
}

class PsmDlgHospiPhysioReg : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgHospiPhysioReg(QWidget *parent = 0);
    ~PsmDlgHospiPhysioReg();

private:
    Ui::PsmDlgHospiPhysioReg *ui;
};

#endif // PSMDLGHOSPIPHYSIOREG_H
