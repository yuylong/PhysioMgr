#include "PsmDlgDepartSel.h"
#include "ui_PsmDlgDepartSel.h"

#include <QKeyEvent>

PsmDlgDepartSel::PsmDlgDepartSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDepartSel)
{
    ui->setupUi(this);
    this->service = NULL;
    this->keyPressedInLe = false;

    this->installEventFilter(this);
    ui->leCond->setFocus();
}

PsmDlgDepartSel::~PsmDlgDepartSel()
{
    delete ui;
}

bool PsmDlgDepartSel::eventFilter(QObject *obj, QEvent *event)
{
    if ( obj == this ) {
        if ( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ( this->keyPressedInLe &&
                 (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) ) {
                this->keyPressedInLe = false;
                return true;
            }
        }
    }

    return QDialog::eventFilter(obj, event);
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
    if ( ui->tableWidget->rowCount() > 0 ) {
        ui->tableWidget->selectRow(0);
        ui->tableWidget->setFocus();
    } else {
        ui->leCond->setFocus();
    }
}

void PsmDlgDepartSel::on_leCond_returnPressed()
{
    this->keyPressedInLe = true;
    this->on_pushButton_clicked();
}

void PsmDlgDepartSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
