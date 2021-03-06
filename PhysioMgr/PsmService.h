#ifndef PSMSERVICE_H
#define PSMSERVICE_H

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QMap>
#include <QtSql>
#include <QtXml>
#include <QLabel>
#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QMainWindow>
#include "PsmDatabase.h"

struct PsmSrvDepartment {
    QString id;
    QString name;
};

struct PsmSrvPhysioItem {
    QString id;
    QString name;
    double price;
};

struct PsmSrvDoctor {
    QString id;
    QString name;
    bool isNurse;
    QString type;
    QString depId;
    QString depName;
    QString phone;
};

struct PsmSrvPatient {
    QString id;
    QString name;
    QDate dob;
    QString phone;
    QString address;
    QString comment;
};

struct PsmSrvHospiRec {
    QString id;
    QString patientid;
    QString patientname;
    QString depid;
    QString depname;
    QString roomid;
    QString disease;
    QString doctorid;
    QString doctorname;
    QString nurseid;
    QString nursename;
    QDate startdate;
    QDate enddate;
};

struct PsmSrvHospiPhysio {
    QString hospirecid;
    QString patientid;
    QString patientname;
    QString physioid;
    QString physioname;
    int freqperiod;
    int freqcount;
    QDate startdate;
    QDate enddate;
};

struct PsmSrvPhysioLog {
    QString patientid;
    QString patientname;
    QString physioid;
    QString physioname;
    QString nurseid;
    QString nursename;
    QString machineid;
    QDateTime optime;
};

class PsmService
{
protected:
    QString configfile;
    PsmDatabase database;
    QMainWindow *parent;

    int hospiRecIdLen;
    QString machineid;
    void initParametersFromDom(const QDomElement &docelem);

public:
    PsmService();

    QString getConfigFile() const;
    void setConfigFile(const QString &configfile);
    QMainWindow *getParentWindow() const;
    void setParentWindow(QMainWindow *window);
    const PsmDatabase *getDatabase() const;
    PsmDatabase *getDatabase();

    bool init(QWidget *window = NULL);

    QString getMachineId() const;

    /* Departments */
    bool readSelectedDepartment(QTableWidget *tbl, PsmSrvDepartment *dep);
    QString readSelectedDepartmentId(QTableWidget *tbl);
    void refreshDepartmentList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void searchDepartment(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertNewDepartment(const PsmSrvDepartment &dep, QWidget *window = NULL);
    void updateDepartment(const PsmSrvDepartment &dep, QWidget *window = NULL);
    void deleteDepartment(const QString &depid, QWidget *window = NULL);

    /* Physio Items */
    bool readSelectedPhysioItem(QTableWidget *tbl, PsmSrvPhysioItem *physio);
    QString readSelectedPhysioItemId(QTableWidget *tbl);
    void refreshPhysioItemList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void searchPhysioItem(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertNewPhysioItem(const PsmSrvPhysioItem &physio, QWidget *window = NULL);
    void updatePhysioItem(const PsmSrvPhysioItem &physio, QWidget *window = NULL);
    void deletePhysioItem(const QString &physioid, QWidget *window = NULL);

    /* Doctors */
    bool readSelectedDoctor(QTableWidget *tbl, PsmSrvDoctor *doctor);
    QString readSelectedDoctorId(QTableWidget *tbl);
    void refreshDoctorList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void searchDoctor(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertDoctor(const PsmSrvDoctor &doctor, QWidget *window = NULL);
    void updateDoctor(const PsmSrvDoctor &doctor, QWidget *window = NULL);
    void deleteDoctor(const QString &doctorid, QWidget *window = NULL);

    /* Patients */
    bool readSelectedPatient(QTableWidget *tbl, PsmSrvPatient *patient);
    QString readSelectedPatientId(QTableWidget *tbl);
    void refreshPatientList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void searchPatient(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    int getPatientCount(const QString &srchstr);
    bool getFirstPatient(const QString &srchstr, PsmSrvPatient *patient, QWidget *window = NULL);
    void insertPatient(const PsmSrvPatient &patient, QString *patientid, QWidget *window = NULL);
    void updatePatient(const PsmSrvPatient &patient, QWidget *window = NULL);
    void deletePatient(const QString &patientid, QWidget *window = NULL);

    /* Hospi-records */
    bool readSelectedHospiRec(QTableWidget *tbl, PsmSrvHospiRec *hospirec);
    QString readSelectedHospiRecId(QTableWidget *tbl);
    bool readOneHospiRec(const QString &hospirecid, PsmSrvHospiRec *hospirec);
    void searchHospiRec(const QString &srchstr, const QDate &startdate, const QDate &enddate,
                        QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertHospiRec(const PsmSrvHospiRec &hospirec, QString *hospirecid = NULL, QWidget *window = NULL);
    void updateHospiRec(const PsmSrvHospiRec &hospirec, QWidget *window = NULL);
    void deleteHospiRec(const QString &hospirecid, QWidget *window = NULL);

    /* Hospi-Physio-registration */
    bool readSelectedHospiPhysio(QTableWidget *tbl, PsmSrvHospiPhysio *hospiphysio);
    void listHospiPhysio(const QString &hospirecid, QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertHospiPhysio(const PsmSrvHospiPhysio &hospiphysio, QWidget *window = NULL);
    void updateHospiPhysio(const PsmSrvHospiPhysio &hospiphysio,
                           const QDate &oldstartdate = QDate(), QWidget *window = NULL);
    void deleteHospiPhysio(const PsmSrvHospiPhysio &hospiphysio, QWidget *window = NULL);

    /* Physio Logs */
    bool readSelectedPhysioLog(QTableWidget *tbl, PsmSrvPhysioLog *physiolog);
    void listHospiPhysioLog(const PsmSrvHospiPhysio &hospiphysio,
                            QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    QList<PsmSrvHospiPhysio> getNowPermitPhysio(const QString &patientid, const QString &physioid,
                                                const QDate &checkdate);
    bool checkPhysioPermitNow(const QDate &checkdate, const PsmSrvHospiPhysio &hospiphysio);
    bool checkPhysioPermitNow(const QDate &checkdate, const QList<PsmSrvHospiPhysio> &hplist);
    bool insertPhysioLog(const PsmSrvPhysioLog &physiolog, QWidget *window = NULL);
    bool tryAddPhysioLog(const PsmSrvPhysioLog &physiolog, QWidget *window = NULL);
    bool deletePhysioLog(const PsmSrvPhysioLog &physiolog, QWidget *window = NULL);

    QDateTime getDbTime();
    void insertPhysioLogToTable(const PsmSrvPhysioLog &physiolog, QTableWidget *tbl);

    /* Import & Export */
    void exportDatabase(const QString &dumpexec, const QString &outfile, QWidget *window = NULL);
    void importDatabase(const QString &sqlexec, const QString &infile, QWidget *window = NULL);

private:    
    int getTableSelectedRow(QTableWidget *tbl);
    QString readTableSelectedId(QTableWidget *tbl, int idx);

    QString getNextPatientId();

    QString getCurrentMaxHospiRecId();
    bool checkHospiRecId(const QString &curid, bool *hasletter = NULL);
    bool needWarningForHospiRecId(const QString &curid, bool hasletter);
    QString getNextHospiRecId(const QString &curid, bool *hasletter = NULL);
    QString getCurrentNextHostpiRecId(QWidget *window = NULL);

    QDate getDateBucket(const QDate &startdate, const QDate &checkdate, int period);
};

#endif // PSMSERVICE_H
