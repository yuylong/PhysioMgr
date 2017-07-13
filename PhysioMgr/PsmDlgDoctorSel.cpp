#include "PsmDlgDoctorSel.h"
#include "ui_PsmDlgDoctorSel.h"

PsmDlgDoctorSel::PsmDlgDoctorSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDoctorSel)
{
    ui->setupUi(this);
    this->service = NULL;
}

PsmDlgDoctorSel::~PsmDlgDoctorSel()
{
    delete ui;
}

PsmService *PsmDlgDoctorSel::getService() const
{
    return this->service;
}

void PsmDlgDoctorSel::setService(PsmService *service)
{
    this->service = service;
}

bool PsmDlgDoctorSel::getSelectedDoctor(PsmSrvDoctor *doctor)
{
    if (this->service == NULL || doctor == NULL)
        return false;

    return this->service->readSelectedDoctor(ui->tableWidget, doctor);
}

void PsmDlgDoctorSel::on_pbRefresh_clicked()
{
    if (this->service == NULL)
        return;

    if (ui->leCond->text().isEmpty())
        this->service->refreshDoctorList(ui->lblCnt, ui->tableWidget, this);
    else
        this->service->searchDoctor(ui->leCond->text(), ui->lblCnt, ui->tableWidget, this);
}

void PsmDlgDoctorSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
