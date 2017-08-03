#include "PsmDlgHospiRec.h"
#include "ui_PsmDlgHospiRec.h"

#include <QKeyEvent>

#include "PsmDlgDepartSel.h"
#include "PsmDlgDoctorSel.h"

PsmDlgHospiRec::PsmDlgHospiRec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgHospiRec)
{
    ui->setupUi(this);

    ui->deStartDate->setDate(QDate::currentDate());
    ui->deEndDate->setDate(QDate::currentDate().addDays(7));
    ui->pbDepart->setFocus();

    this->returnFiltered = false;
    this->installEventFilter(this);
}

PsmDlgHospiRec::~PsmDlgHospiRec()
{
    delete ui;
}

bool PsmDlgHospiRec::eventFilter(QObject *obj, QEvent *event)
{
    if ( obj == this ) {
        if ( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ( this->returnFiltered &&
                 (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) ) {
                this->returnFiltered = false;
                return true;
            }
        }
    }

    return QDialog::eventFilter(obj, event);
}

PsmService *PsmDlgHospiRec::getService() const
{
    return this->service;
}

void PsmDlgHospiRec::setService(PsmService *service)
{
    this->service = service;
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

void PsmDlgHospiRec::setDoctorId(const QString &doctorid)
{
    this->doctorid = doctorid;
}

void PsmDlgHospiRec::setDoctorName(const QString &doctorname)
{
    ui->pbDoctor->setText(doctorname);
}

void PsmDlgHospiRec::setNurseId(const QString &nurseid)
{
    this->nurseid = nurseid;
}

void PsmDlgHospiRec::setNurseName(const QString &nursename)
{
    ui->pbNurse->setText(nursename);
}

void PsmDlgHospiRec::setStartDate(const QDate &date)
{
    ui->deStartDate->setDate(date);
}

void PsmDlgHospiRec::setEndDate(const QDate &date)
{
    ui->deEndDate->setDate(date);
}

void PsmDlgHospiRec::on_pbDepart_clicked()
{
    PsmDlgDepartSel dialog(this);
    dialog.setService(this->service);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvDepartment depart;
    bool ok = dialog.getSelectedDepart(&depart);
    if (!ok)
        return;

    this->depid = depart.id;
    ui->pbDepart->setText(depart.name);

    ui->leRoomId->setFocus();
}

void PsmDlgHospiRec::on_leRoomId_returnPressed()
{
    this->returnFiltered = true;
    ui->leDisease->setFocus();
}

void PsmDlgHospiRec::on_leDisease_returnPressed()
{
    this->returnFiltered = true;
    ui->pbDoctor->setFocus();
}

void PsmDlgHospiRec::on_pbDoctor_clicked()
{
    PsmDlgDoctorSel dialog(this);
    dialog.setService(this->service);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvDoctor doctor;
    bool ok = dialog.getSelectedDoctor(&doctor);
    if (!ok)
        return;

    this->doctorid = doctor.id;
    ui->pbDoctor->setText(doctor.name);

    ui->pbNurse->setFocus();
}

void PsmDlgHospiRec::on_pbNurse_clicked()
{
    PsmDlgDoctorSel dialog(this);
    dialog.setService(this->service);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvDoctor nurse;
    bool ok = dialog.getSelectedDoctor(&nurse);
    if (!ok)
        return;

    this->nurseid = nurse.id;
    ui->pbNurse->setText(nurse.name);

    ui->deStartDate->setFocus();
}
