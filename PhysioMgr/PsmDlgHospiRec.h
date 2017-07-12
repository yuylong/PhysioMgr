#ifndef PSMDLGHOSPIREC_H
#define PSMDLGHOSPIREC_H

#include <QDialog>

namespace Ui {
class PsmDlgHospiRec;
}

class PsmDlgHospiRec : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgHospiRec(QWidget *parent = 0);
    ~PsmDlgHospiRec();

private:
    Ui::PsmDlgHospiRec *ui;
};

#endif // PSMDLGHOSPIREC_H
