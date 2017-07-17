#include "PsmDlgHospiPhysio.h"
#include "ui_PsmDlgHospiPhysio.h"

PsmDlgHospiPhysio::PsmDlgHospiPhysio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgHospiPhysio)
{
    ui->setupUi(this);
    this->service = NULL;
}

PsmDlgHospiPhysio::~PsmDlgHospiPhysio()
{
    delete ui;
}

PsmService *PsmDlgHospiPhysio::getService() const
{
    return this->service;
}

void PsmDlgHospiPhysio::setService(PsmService *service)
{
    this->service = service;
}

QString PsmDlgHospiPhysio::getHospiRecId() const
{
    return ui->leHospiRecId->text();
}

void PsmDlgHospiPhysio::setHospiRecId(const QString hospirecid)
{
    ui->leHospiRecId->setText(hospirecid);
    if (this->service == NULL)
        return;

    PsmSrvHospiRec hospirec;
    bool ok = this->service->readOneHospiRec(hospirecid, &hospirec);
    if (!ok)
        return;

    ui->lePatientName->setText(hospirec.patientname);
    ui->leDepartName->setText(hospirec.depname);
    ui->leRoomId->setText(hospirec.roomid);
    ui->leDisease->setText(hospirec.disease);
    ui->leDoctorName->setText(hospirec.doctorname);
    ui->leNurseName->setText(hospirec.nursename);
    ui->leDates->setText(hospirec.startdate.toString(Qt::ISODate) + " - " +
                         hospirec.enddate.toString(Qt::ISODate));

    this->service->listHospiPhysio(hospirecid, ui->lblCount, ui->tableWidget, this);
}

void PsmDlgHospiPhysio::clearHospiRecId()
{
    ui->leHospiRecId->setText("");
    ui->lePatientName->setText("");
    ui->leDepartName->setText("");
    ui->leRoomId->setText("");
    ui->leDisease->setText("");
    ui->leDoctorName->setText("");
    ui->leNurseName->setText("");
    ui->leDates->setText("");
    ui->lblCount->setText("0");
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}
