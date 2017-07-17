#include "PsmDlgHospiPhysioReg.h"
#include "ui_PsmDlgHospiPhysioReg.h"

PsmDlgHospiPhysioReg::PsmDlgHospiPhysioReg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgHospiPhysioReg)
{
    ui->setupUi(this);
}

PsmDlgHospiPhysioReg::~PsmDlgHospiPhysioReg()
{
    delete ui;
}
