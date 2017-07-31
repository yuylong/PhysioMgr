#ifndef PSMDLGDOCTOR_H
#define PSMDLGDOCTOR_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgDoctor;
}

class PsmDlgDoctor : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgDoctor(QWidget *parent = 0);
    ~PsmDlgDoctor();

    PsmService *getService() const;
    void setService(PsmService *service);

    QString getDoctorId() const;
    QString getDoctorName() const;
    bool getIsNurse() const;
    QString getType() const;
    QString getDepartId() const;
    QString getDepartName() const;
    QString getPhoneNum() const;

    void setDoctorId(const QString &id);
    void setDoctorName(const QString &name);
    void setIsNurse(bool isnurse);
    void setType(const QString &type);
    void setDepartId(const QString &depid);
    void setDepartName(const QString &depname);
    void setPhoneNum(const QString &phonenum);

    void lockDoctorId();
    void unlockDoctorId();

private slots:
    void on_pbDoctorDepart_clicked();
    void on_buttonBox_accepted();

    void on_leDoctorID_returnPressed();
    void on_leDoctorName_returnPressed();
    void on_leDoctorPhone_returnPressed();

private:
    Ui::PsmDlgDoctor *ui;
    PsmService *service;

    QString departId;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // PSMDLGDOCTOR_H
