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
