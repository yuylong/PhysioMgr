#include "PsmDlgPhysioSel.h"
#include "ui_PsmDlgPhysioSel.h"

PsmDlgPhysioSel::PsmDlgPhysioSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPhysioSel)
{
    ui->setupUi(this);
    this->service = NULL;
}

PsmDlgPhysioSel::~PsmDlgPhysioSel()
{
    delete ui;
}

PsmService *PsmDlgPhysioSel::getService() const
{
    return this->service;
}

void PsmDlgPhysioSel::setService(PsmService *service)
{
    this->service = service;
}

bool PsmDlgPhysioSel::getSelectedPhysioItem(PsmSrvPhysioItem *physio)
{
    if (this->service == NULL || physio == NULL)
        return false;

    return this->service->readSelectedPhysioItem(ui->tableWidget, physio);
}

void PsmDlgPhysioSel::on_pbRefresh_clicked()
{
    if (this->service == NULL)
        return;

    if (ui->leCond->text().isEmpty())
        this->service->refreshPhysioItemList(ui->lblCount, ui->tableWidget, this);
    else
        this->service->searchPhysioItem(ui->leCond->text(), ui->lblCount, ui->tableWidget, this);
}

void PsmDlgPhysioSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
