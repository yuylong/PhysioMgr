#ifndef PSMDLGHOSPIPHYSIO_H
#define PSMDLGHOSPIPHYSIO_H

#include <QDialog>

namespace Ui {
class PsmDlgHospiPhysio;
}

class PsmDlgHospiPhysio : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgHospiPhysio(QWidget *parent = 0);
    ~PsmDlgHospiPhysio();

private:
    Ui::PsmDlgHospiPhysio *ui;
};

#endif // PSMDLGHOSPIPHYSIO_H
