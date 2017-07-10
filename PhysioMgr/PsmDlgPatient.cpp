#include "PsmDlgPatient.h"
#include "ui_PsmDlgPatient.h"

PsmDlgPatient::PsmDlgPatient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPatient)
{
    ui->setupUi(this);
}

PsmDlgPatient::~PsmDlgPatient()
{
    delete ui;
}
