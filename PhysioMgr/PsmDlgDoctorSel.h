#ifndef PSMDLGDOCTORSEL_H
#define PSMDLGDOCTORSEL_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgDoctorSel;
}

class PsmDlgDoctorSel : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgDoctorSel(QWidget *parent = 0);
    ~PsmDlgDoctorSel();

    PsmService *getService() const;
    void setService(PsmService *service);

    bool getSelectedDoctor(PsmSrvDoctor *doctor);

private slots:
    void on_pbRefresh_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::PsmDlgDoctorSel *ui;
    PsmService *service;
};

#endif // PSMDLGDOCTORSEL_H
