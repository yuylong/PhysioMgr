#include "PsmDlgPatientSel.h"
#include "ui_PsmDlgPatientSel.h"

PsmDlgPatientSel::PsmDlgPatientSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPatientSel)
{
    ui->setupUi(this);
    this->service = NULL;
}

PsmDlgPatientSel::~PsmDlgPatientSel()
{
    delete ui;
}

PsmService *PsmDlgPatientSel::getService() const
{
    return this->service;
}

void PsmDlgPatientSel::setService(PsmService *service)
{
    this->service = service;
}

void PsmDlgPatientSel::setAndRefreshCond(const QString &cond)
{
    ui->leCond->setText(cond);
    this->on_pbRefresh_clicked();
}

bool PsmDlgPatientSel::getSelectedPatient(PsmSrvPatient *patient)
{
    if (this->service == NULL || patient == NULL)
        return false;

    return this->service->readSelectedPatient(ui->tableWidget, patient);
}

void PsmDlgPatientSel::on_pbRefresh_clicked()
{
    if (this->service == NULL)
        return;

    if (ui->leCond->text().isEmpty())
        this->service->refreshPatientList(ui->lblCount, ui->tableWidget, this);
    else
        this->service->searchPatient(ui->leCond->text(), ui->lblCount, ui->tableWidget, this);
}

void PsmDlgPatientSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
