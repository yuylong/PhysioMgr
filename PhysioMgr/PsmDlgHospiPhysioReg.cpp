#include "PsmDlgHospiPhysioReg.h"
#include "ui_PsmDlgHospiPhysioReg.h"

#include "PsmDlgPhysioSel.h"

PsmDlgHospiPhysioReg::PsmDlgHospiPhysioReg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgHospiPhysioReg)
{
    ui->setupUi(this);
    this->service = NULL;
}

PsmDlgHospiPhysioReg::~PsmDlgHospiPhysioReg()
{
    delete ui;
}

PsmService *PsmDlgHospiPhysioReg::getService() const
{
    return this->service;
}

void PsmDlgHospiPhysioReg::setService(PsmService *service)
{
    this->service = service;
}

QString PsmDlgHospiPhysioReg::getHospiRecId() const
{
    return ui->leHospiRecId->text();
}

QString PsmDlgHospiPhysioReg::getPatientName() const
{
    return ui->lePatientName->text();
}

QString PsmDlgHospiPhysioReg::getPhysioId() const
{
    return this->physioId;
}

QString PsmDlgHospiPhysioReg::getPhysioName() const
{
    return ui->pbPhysio->text();
}

int PsmDlgHospiPhysioReg::getFreqPeriod() const
{
    return ui->sbFreqPeriod->value();
}

int PsmDlgHospiPhysioReg::getFreqCount() const
{
    return ui->sbFreqCount->value();
}

QDate PsmDlgHospiPhysioReg::getStartDate() const
{
    return ui->deStartDate->date();
}

QDate PsmDlgHospiPhysioReg::getEndDate() const
{
    return ui->deEndDate->date();
}

void PsmDlgHospiPhysioReg::setHospiRecId(const QString &hospirecid)
{
    ui->leHospiRecId->setText(hospirecid);
}

void PsmDlgHospiPhysioReg::setPatientName(const QString &patientname)
{
    ui->lePatientName->setText(patientname);
}

void PsmDlgHospiPhysioReg::setPhysioId(const QString &physioid)
{
    this->physioId = physioid;
}

void PsmDlgHospiPhysioReg::setPhysioName(const QString &physioname)
{
    ui->pbPhysio->setText(physioname);
}

void PsmDlgHospiPhysioReg::setFreqPeriod(int day)
{
    ui->sbFreqPeriod->setValue(day);
}

void PsmDlgHospiPhysioReg::setFreqCount(int num)
{
    ui->sbFreqCount->setValue(num);
}

void PsmDlgHospiPhysioReg::setStartDate(const QDate &startdate)
{
    ui->deStartDate->setDate(startdate);
}

void PsmDlgHospiPhysioReg::setEndDate(const QDate &enddate)
{
    ui->deEndDate->setDate(enddate);
}

void PsmDlgHospiPhysioReg::lockPhysioSel()
{
    ui->pbPhysio->setEnabled(false);
}

void PsmDlgHospiPhysioReg::unlockPhysioSel()
{
    ui->pbPhysio->setEnabled(true);
}

void PsmDlgHospiPhysioReg::on_pbPhysio_clicked()
{
    if (this->service == NULL)
        return;

    PsmDlgPhysioSel dialog(this);
    dialog.setService(this->service);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvPhysioItem physio;
    bool ok = dialog.getSelectedPhysioItem(&physio);
    if (!ok)
        return;

    this->physioId = physio.id;
    ui->pbPhysio->setText(physio.name);

    ui->sbFreqPeriod->setFocus();
}

void PsmDlgHospiPhysioReg::on_buttonBox_accepted()
{
    if (this->physioId.isEmpty()) {
        QMessageBox::warning(this, "填选信息缺失", "理疗项目必须选择！");
        return;
    }

    this->accept();
}
