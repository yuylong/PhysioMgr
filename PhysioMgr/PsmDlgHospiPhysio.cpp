#include "PsmDlgHospiPhysio.h"
#include "ui_PsmDlgHospiPhysio.h"

#include "PsmDlgHospiPhysioReg.h"

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

    this->hospirec = hospirec;
    ui->lePatientName->setText(hospirec.patientname);
    ui->leDepartName->setText(hospirec.depname);
    ui->leRoomId->setText(hospirec.roomid);
    ui->leDisease->setText(hospirec.disease);
    ui->leDoctorName->setText(hospirec.doctorname);
    ui->leNurseName->setText(hospirec.nursename);
    ui->leDates->setText(hospirec.startdate.toString(Qt::ISODate) + " - " +
                         hospirec.enddate.toString(Qt::ISODate));

    this->refreshPhysioList();
}

void PsmDlgHospiPhysio::clearHospiRecId()
{
    this->hospirec.id = QString();
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

void PsmDlgHospiPhysio::refreshPhysioList()
{
    if (this->service == NULL)
        return;

    this->service->listHospiPhysio(ui->leHospiRecId->text(), ui->lblCount, ui->tableWidget, this);
}

void PsmDlgHospiPhysio::on_pbRefresh_clicked()
{
    this->refreshPhysioList();
}

void PsmDlgHospiPhysio::on_pbAdd_clicked()
{
    if (this->service == NULL)
        return;

    PsmDlgHospiPhysioReg dialog(this);
    dialog.setService(this->service);
    dialog.setHospiRecId(this->hospirec.id);
    dialog.setPatientName(this->hospirec.patientname);
    dialog.setStartDate(this->hospirec.startdate);
    dialog.setEndDate(this->hospirec.enddate);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)\
        return;

    PsmSrvHospiPhysio hospiphysio;
    hospiphysio.hospirecid = this->hospirec.id;
    hospiphysio.physioid = dialog.getPhysioId();
    hospiphysio.physioname = dialog.getPhysioName();
    hospiphysio.freqperiod = dialog.getFreqPeriod();
    hospiphysio.freqcount = dialog.getFreqCount();
    hospiphysio.startdate = dialog.getStartDate();
    hospiphysio.enddate = dialog.getEndDate();

    this->service->insertHospiPhysio(hospiphysio, this);
    this->refreshPhysioList();
}
