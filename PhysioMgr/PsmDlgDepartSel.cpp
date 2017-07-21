#include "PsmDlgDepartSel.h"
#include "ui_PsmDlgDepartSel.h"

PsmDlgDepartSel::PsmDlgDepartSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDepartSel)
{
    ui->setupUi(this);
    this->service = NULL;
    ui->leCond->setFocus();
}

PsmDlgDepartSel::~PsmDlgDepartSel()
{
    delete ui;
}

PsmService *PsmDlgDepartSel::getService() const
{
    return this->service;
}

void PsmDlgDepartSel::setService(PsmService *service)
{
    this->service = service;
}

bool PsmDlgDepartSel::getSelectedDepart(PsmSrvDepartment *dep)
{
    if (this->service == NULL || dep == NULL)
        return false;

    return this->service->readSelectedDepartment(ui->tableWidget, dep);
}

void PsmDlgDepartSel::on_pushButton_clicked()
{
    if (this->service == NULL)
        return;

    if (ui->leCond->text().isEmpty())
        this->service->refreshDepartmentList(ui->lblCnt, ui->tableWidget, this);
    else
        this->service->searchDepartment(ui->leCond->text(), ui->lblCnt, ui->tableWidget, this);

    ui->leCond->selectAll();
    ui->leCond->setFocus();
}

void PsmDlgDepartSel::on_leCond_returnPressed()
{
    this->on_pushButton_clicked();
}

void PsmDlgDepartSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
