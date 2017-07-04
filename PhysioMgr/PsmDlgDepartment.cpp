#include "PsmDlgDepartment.h"
#include "ui_PsmDlgDepartment.h"

PsmDlgDepartment::PsmDlgDepartment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDepartment)
{
    ui->setupUi(this);
}

PsmDlgDepartment::~PsmDlgDepartment()
{
    delete ui;
}

QString PsmDlgDepartment::getDepartID() const
{
    return ui->leDepartID->text();
}

QString PsmDlgDepartment::getDepartName() const
{
    return ui->leDepartName->text();
}

void PsmDlgDepartment::setDepartID(const QString &id)
{
    ui->leDepartID->setText(id);
}

void PsmDlgDepartment::setDepartName(const QString &name)
{
    ui->leDepartName->setText(name);
}

void PsmDlgDepartment::lockDepartID()
{
    ui->leDepartID->setEnabled(false);
}

void PsmDlgDepartment::unlockDepartID()
{
    ui->leDepartID->setEnabled(true);
}
