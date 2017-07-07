#include "PsmDlgDoctor.h"
#include "ui_PsmDlgDoctor.h"

PsmDlgDoctor::PsmDlgDoctor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDoctor)
{
    ui->setupUi(this);
    this->service = NULL;
}

PsmDlgDoctor::~PsmDlgDoctor()
{
    delete ui;
}

PsmService *PsmDlgDoctor::getService() const
{
    return this->service;
}

void PsmDlgDoctor::setService(PsmService *service)
{
    this->service = service;
}

QString PsmDlgDoctor::getDoctorId() const
{
    return ui->leDoctorID->text();
}

QString PsmDlgDoctor::getDoctorName() const
{
    return ui->leDoctorName->text();
}

bool PsmDlgDoctor::getIsNurse() const
{
    return ui->cmbDoctorType->currentIndex() > 0;
}

QString PsmDlgDoctor::getType() const
{
    return ui->cmbDoctorType->currentText();
}

QString PsmDlgDoctor::getDepartId() const
{
    return this->departId;
}

QString PsmDlgDoctor::getDepartName() const
{
    return ui->pbDoctorDepart->text();
}

QString PsmDlgDoctor::getPhoneNum() const
{
    return ui->leDoctorPhone->text();
}
