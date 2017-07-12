#include "PsmDlgHospiRec.h"
#include "ui_PsmDlgHospiRec.h"

PsmDlgHospiRec::PsmDlgHospiRec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgHospiRec)
{
    ui->setupUi(this);

    ui->deStartDate->setDate(QDate::currentDate());
    ui->deEndDate->setDate(QDate::currentDate().addDays(7));
}

PsmDlgHospiRec::~PsmDlgHospiRec()
{
    delete ui;
}

QString PsmDlgHospiRec::getHospiRecId() const
{
    return ui->leHospiRecId->text();
}

QString PsmDlgHospiRec::getPatientId() const
{
    return ui->lePatientId->text();
}

QString PsmDlgHospiRec::getPatientName() const
{
    return ui->lePatientName->text();
}

QString PsmDlgHospiRec::getDepartId() const
{
    return this->depid;
}

QString PsmDlgHospiRec::getDepartName() const
{
    if (this->depid.isEmpty())
        return QString();
    else
        return ui->pbDepart->text();
}

QString PsmDlgHospiRec::getRoomId() const
{
    return ui->leRoomId->text();
}

QString PsmDlgHospiRec::getDisease() const
{
    return ui->leDisease->text();
}

QString PsmDlgHospiRec::getDoctorId() const
{
    return this->doctorid;
}

QString PsmDlgHospiRec::getDoctorName() const
{
    if (this->doctorid.isEmpty())
        return QString();
    else
        return ui->pbDoctor->text();
}

QString PsmDlgHospiRec::getNurseId() const
{
    return this->nurseid;
}

QString PsmDlgHospiRec::getNurseName() const
{
    if (this->nurseid.isEmpty())
        return QString();
    else
        return ui->pbNurse->text();
}

QDate PsmDlgHospiRec::getStartDate() const
{
    return ui->deStartDate->date();
}

QDate PsmDlgHospiRec::getEndDate() const
{
    return ui->deEndDate->date();
}

void PsmDlgHospiRec::setHospiRecId(const QString &id)
{
    ui->leHospiRecId->setText(id);
}

void PsmDlgHospiRec::setPatientId(const QString &patientid)
{
    ui->lePatientId->setText(patientid);
}

void PsmDlgHospiRec::setPatientName(const QString &patientname)
{
    ui->lePatientName->setText(patientname);
}

void PsmDlgHospiRec::setDepartId(const QString &depid)
{
    this->depid = depid;
}

void PsmDlgHospiRec::setDepartName(const QString &depname)
{
    ui->pbDepart->setText(depname);
}

void PsmDlgHospiRec::setRoomId(const QString &roomid)
{
    ui->leRoomId->setText(roomid);
}

void PsmDlgHospiRec::setDisease(const QString &disease)
{
    ui->leDisease->setText(disease);
}
