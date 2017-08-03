#ifndef PSMDLGPATIENT_H
#define PSMDLGPATIENT_H

#include <QDialog>
#include <QDate>

namespace Ui {
class PsmDlgPatient;
}

class PsmDlgPatient : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPatient(QWidget *parent = 0);
    ~PsmDlgPatient();

    QString getPatientId() const;
    QString getPatientName() const;
    QDate getDob() const;
    QString getPhoneNum() const;
    QString getAddress() const;
    QString getComment() const;

    void setPatientId(const QString &id);
    void setPatientName(const QString &name);
    void setDob(const QDate &dob);
    void setPhoneNum(const QString &phone);
    void setAddress(const QString &addr);
    void setComment(const QString &comment);
    void lockPatientId();
    void unlockPatientId();

private slots:
    void on_buttonBox_accepted();

    void on_lePatientId_returnPressed();
    void on_lePatientName_returnPressed();
    void on_lePhoneNum_returnPressed();
    void on_leAddress_returnPressed();
    void on_leComment_returnPressed();

private:
    Ui::PsmDlgPatient *ui;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // PSMDLGPATIENT_H
