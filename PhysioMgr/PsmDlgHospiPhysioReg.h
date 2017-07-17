#ifndef PSMDLGHOSPIPHYSIOREG_H
#define PSMDLGHOSPIPHYSIOREG_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgHospiPhysioReg;
}

class PsmDlgHospiPhysioReg : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgHospiPhysioReg(QWidget *parent = 0);
    ~PsmDlgHospiPhysioReg();

    PsmService *getService() const;
    void setService(PsmService *service);

    QString getHospiRecId() const;
    QString getPatientName() const;
    QString getPhysioId() const;
    QString getPhysioName() const;
    int getFreqPeriod() const;
    int getFreqCount() const;
    QDate getStartDate() const;
    QDate getEndDate() const;

    void setHospiRecId(const QString &hospirecid);
    void setPatientName(const QString &patientname);
    void setPhysioId(const QString &physioid);
    void setPhysioName(const QString &physioname);
    void setFreqPeriod(int day);
    void setFreqCount(int num);
    void setStartDate(const QDate &startdate);
    void setEndDate(const QDate &enddate);

    void lockPhysioSel();
    void unlockPhysioSel();

private slots:
    void on_pbPhysio_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PsmDlgHospiPhysioReg *ui;
    PsmService *service;

    QString physioId;
};

#endif // PSMDLGHOSPIPHYSIOREG_H
