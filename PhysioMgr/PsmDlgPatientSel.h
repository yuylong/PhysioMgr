#ifndef PSMDLGPATIENTSEL_H
#define PSMDLGPATIENTSEL_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgPatientSel;
}

class PsmDlgPatientSel : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPatientSel(QWidget *parent = 0);
    ~PsmDlgPatientSel();

    PsmService *getService() const;
    void setService(PsmService *service);

    void setAndRefreshCond(const QString &cond);
    bool getSelectedPatient(PsmSrvPatient *patient);

private slots:
     void on_pbRefresh_clicked();

     void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::PsmDlgPatientSel *ui;
    PsmService *service;
};

#endif // PSMDLGPATIENTSEL_H
