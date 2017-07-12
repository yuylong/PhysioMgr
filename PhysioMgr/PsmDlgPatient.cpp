#include "PsmDlgPatient.h"
#include "ui_PsmDlgPatient.h"

#include <QMessageBox>

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

QString PsmDlgPatient::getPatientId() const
{
    return ui->lePatientId->text();
}

QString PsmDlgPatient::getPatientName() const
{
    return ui->lePatientName->text();
}

QDate PsmDlgPatient::getDob() const
{
    return ui->deDob->date();
}

QString PsmDlgPatient::getPhoneNum() const
{
    return ui->lePhoneNum->text();
}

QString PsmDlgPatient::getAddress() const
{
    return ui->leAddress->text();
}

QString PsmDlgPatient::getComment() const
{
    return ui->leComment->text();
}

void PsmDlgPatient::setPatientId(const QString &id)
{
    ui->lePatientId->setText(id);
}

void PsmDlgPatient::setPatientName(const QString &name)
{
    ui->lePatientName->setText(name);
}

void PsmDlgPatient::setDob(const QDate &dob)
{
    ui->deDob->setDate(dob);
}

void PsmDlgPatient::setPhoneNum(const QString &phone)
{
    ui->lePhoneNum->setText(phone);
}

void PsmDlgPatient::setAddress(const QString &addr)
{
    ui->leAddress->setText(addr);
}

void PsmDlgPatient::setComment(const QString &comment)
{
    ui->leComment->setText(comment);
}

void PsmDlgPatient::lockPatientId()
{
    ui->lePatientId->setEnabled(false);
}

void PsmDlgPatient::unlockPatientId()
{
    ui->lePatientId->setEnabled(true);
}

void PsmDlgPatient::on_buttonBox_accepted()
{
    if (ui->lePatientId->text().isEmpty() || ui->lePatientName->text().isEmpty()) {
        QMessageBox::warning(this, "信息填写错误", "患者的编号和姓名必须填写。");
        return;
    }

    this->accept();
}
