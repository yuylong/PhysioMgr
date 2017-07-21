#ifndef PSMDLGDEPARTSEL_H
#define PSMDLGDEPARTSEL_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgDepartSel;
}

class PsmDlgDepartSel : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgDepartSel(QWidget *parent = 0);
    ~PsmDlgDepartSel();

    PsmService *getService() const;
    void setService(PsmService *service);

    bool getSelectedDepart(PsmSrvDepartment *dep);

private slots:
    void on_pushButton_clicked();
    void on_leCond_returnPressed();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::PsmDlgDepartSel *ui;
    PsmService *service;
};

#endif // PSMDLGDEPARTSEL_H
