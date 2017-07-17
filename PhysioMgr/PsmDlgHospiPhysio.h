#ifndef PSMDLGHOSPIPHYSIO_H
#define PSMDLGHOSPIPHYSIO_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgHospiPhysio;
}

class PsmDlgHospiPhysio : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgHospiPhysio(QWidget *parent = 0);
    ~PsmDlgHospiPhysio();

    PsmService *getService() const;
    void setService(PsmService *service);

    QString getHospiRecId() const;
    void setHospiRecId(const QString hospirecid);
    void clearHospiRecId();

private:
    Ui::PsmDlgHospiPhysio *ui;
    PsmService *service;
};

#endif // PSMDLGHOSPIPHYSIO_H
