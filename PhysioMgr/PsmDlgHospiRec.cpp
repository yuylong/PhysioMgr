#include "PsmDlgHospiRec.h"
#include "ui_PsmDlgHospiRec.h"

PsmDlgHospiRec::PsmDlgHospiRec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgHospiRec)
{
    ui->setupUi(this);

    ui->deStartDate->setDate(QDate::currentDate());
    ui->deEndDate->setDate(QDate::currentDate().addDays(7));
}

PsmDlgHospiRec::~PsmDlgHospiRec()
{
    delete ui;
}
