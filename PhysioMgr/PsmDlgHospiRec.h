#ifndef PSMDLGHOSPIREC_H
#define PSMDLGHOSPIREC_H

#include <QDialog>
#include <QString>
#include "PsmService.h"

namespace Ui {
class PsmDlgHospiRec;
}

class PsmDlgHospiRec : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgHospiRec(QWidget *parent = 0);
    ~PsmDlgHospiRec();

    PsmService *getService() const;
    void setService(PsmService *service);

    QString getHospiRecId() const;
    QString getPatientId() const;
    QString getPatientName() const;
    QString getDepartId() const;
    QString getDepartName() const;
    QString getRoomId() const;
    QString getDisease() const;
    QString getDoctorId() const;
    QString getDoctorName() const;
    QString getNurseId() const;
    QString getNurseName() const;
    QDate getStartDate() const;
    QDate getEndDate() const;

    void setHospiRecId(const QString &id);
    void setPatientId(const QString &patientid);
    void setPatientName(const QString &patientname);
    void setDepartId(const QString &depid);
    void setDepartName(const QString &depname);
    void setRoomId(const QString &roomid);
    void setDisease(const QString &disease);
    void setDoctorId(const QString &doctorid);
    void setDoctorName(const QString &doctorname);
    void setNurseId(const QString &nurseid);
    void setNurseName(const QString &nursename);
    void setStartDate(const QDate &date);
    void setEndDate(const QDate &date);

private slots:
    void on_pbDepart_clicked();

    void on_pbDoctor_clicked();

    void on_pbNurse_clicked();

private:
    Ui::PsmDlgHospiRec *ui;
    PsmService *service;

    QString depid;
    QString doctorid;
    QString nurseid;
};

#endif // PSMDLGHOSPIREC_H
