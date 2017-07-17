#include "PsmService.h"

#include <QList>
#include <QApplication>
#include <QtXml>

PsmService::PsmService() :
    configfile(), database()
{
    hospiRecIdLen = 8;
}

QString PsmService::getConfigFile() const
{
    return this->configfile;
}

void PsmService::setConfigFile(const QString &configfile)
{
    this->configfile = configfile;
}

QMainWindow *PsmService::getParentWindow() const
{
    return this->parent;
}

void PsmService::setParentWindow(QMainWindow *window)
{
    this->parent = window;
}

const PsmDatabase *PsmService::getDatabase() const
{
    return &this->database;
}

PsmDatabase *PsmService::getDatabase()
{
    return &this->database;
}

void PsmService::initParametersFromDom(const QDomElement &docelem)
{
    if (QString::compare(docelem.tagName(), "database") != 0)
        return;

    QDomNode domnode = docelem.firstChild();
    while (!domnode.isNull()) {
        QDomElement domelem = domnode.toElement();
        if (QString::compare(domelem.tagName(), "hospiRecIdLen") == 0) {
            this->hospiRecIdLen = domelem.text().toInt();
        }

        domnode = domnode.nextSibling();
    }

}

bool PsmService::init(QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    if (this->configfile.isEmpty()) {
        QMessageBox::critical(window, "致命错误", "数据库配置文件未设置，请联系开发人员解决！");
        return false;
    }

    QFile file(this->configfile);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(window, "致命错误", "数据库配置文件无法打开，请联系开发人员解决！");
        return false;
    }

    QDomDocument domdoc("PsmDbConfig");
    if (!domdoc.setContent(&file)) {
        file.close();
        QMessageBox::critical(window, "致命错误", "数据库配置文件无法解读，请联系开发人员解决！");
        return false;
    }
    file.close();

    QDomElement docelem = domdoc.documentElement();
    this->database.configConnectionFromDom(docelem);
    if (!this->database.isConnectionConfigured()) {
        QMessageBox::critical(window, "致命错误", "配置文件无法成功配置数据库，请联系开发人员解决！");
        return false;
    }

    this->database.startDatabase();
    if (!this->database.isDatabaseStarted()) {
        QMessageBox::critical(window, "致命错误", "数据库无法启动运转，请联系开发人员解决！");
        return false;
    }

    this->initParametersFromDom(docelem);
    return true;
}

int PsmService::getTableSelectedRow(QTableWidget *tbl)
{
    QList<QTableWidgetSelectionRange> idxlist = tbl->selectedRanges();
    if (idxlist.count() < 1)
        return -1;

    const QTableWidgetSelectionRange &idx = idxlist.at(0);
    return idx.topRow();
}

QString PsmService::readTableSelectedId(QTableWidget *tbl, int idx)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return QString();

    QTableWidgetItem *item = tbl->item(rowidx, idx);
    if (item == NULL)
        return QString();

    return item->text();
}

bool PsmService::readSelectedDepartment(QTableWidget *tbl, PsmSrvDepartment *dep)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    if (itemid == NULL || itemname == NULL)
        return false;

    dep->id = itemid->text();
    dep->name = itemname->text();
    return true;
}

QString PsmService::readSelectedDepartmentId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshDepartmentList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM departments;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成科室列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
}

void PsmService::searchDepartment(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM departments "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成科室信息检索。" + exterrstr);
    return;
}

void PsmService::insertNewDepartment(const PsmSrvDepartment &dep, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO departments VALUES(?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, dep.id);
    query.bindValue(1, dep.name);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的科室。" + exterrstr);
    return;
}

void PsmService::updateDepartment(const PsmSrvDepartment &dep, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE departments SET name=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, dep.name);
    query.bindValue(1, dep.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成科室信息更新。" + exterrstr);
    return;
}

void PsmService::deleteDepartment(const QString &depid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM departments WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, depid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定科室。" + exterrstr);
    return;
}

bool PsmService::readSelectedPhysioItem(QTableWidget *tbl, PsmSrvPhysioItem *physio)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemprice = tbl->item(rowidx, 2);
    if (itemid == NULL || itemname == NULL)
        return false;

    physio->id = itemid->text();
    physio->name = itemname->text();
    physio->price = 1.0;
    if (itemprice != NULL)
        physio->price = itemprice->text().toDouble();
    return true;
}

QString PsmService::readSelectedPhysioItemId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshPhysioItemList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM physio_items;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成理疗项目列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
}

void PsmService::searchPhysioItem(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM physio_items "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成理疗项目检索。" + exterrstr);
    return;
}

void PsmService::insertNewPhysioItem(const PsmSrvPhysioItem &physio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO physio_items VALUES(?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, physio.id);
    query.bindValue(1, physio.name);
    query.bindValue(2, physio.price);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的理疗项目。" + exterrstr);
    return;
}

void PsmService::updatePhysioItem(const PsmSrvPhysioItem &physio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE physio_items SET name=?, price=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, physio.name);
    query.bindValue(1, physio.price);
    query.bindValue(2, physio.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成理疗项目信息更新。" + exterrstr);
    return;
}

void PsmService::deletePhysioItem(const QString &physioid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM physio_items WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, physioid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定理疗项目。" + exterrstr);
    return;
}

bool PsmService::readSelectedDoctor(QTableWidget *tbl, PsmSrvDoctor *doctor)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemtype = tbl->item(rowidx, 2);
    QTableWidgetItem *itemdep = tbl->item(rowidx, 3);
    QTableWidgetItem *itemphone = tbl->item(rowidx, 4);
    if (itemid == NULL || itemname == NULL)
        return false;

    doctor->id = itemid->text();
    doctor->name = itemname->text();
    doctor->isNurse = itemtype->data(Qt::UserRole).toBool();
    doctor->type = itemtype->text();
    doctor->depId = itemdep->data(Qt::UserRole).toString();
    doctor->depName = itemdep->text();
    doctor->phone = itemphone->text();
    return true;
}

QString PsmService::readSelectedDoctorId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshDoctorList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 3 << 5 << 6;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << 4 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM doctors;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成医护人员列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
}

void PsmService::searchDoctor(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 3 << 5 << 6;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << 4 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM doctors "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr OR "
                             "depname LIKE :srchstr OR phonenum LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成医护人员检索。" + exterrstr);
    return;
}

void PsmService::insertDoctor(const PsmSrvDoctor &doctor, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO doctors VALUES(?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, doctor.id);
    query.bindValue(1, doctor.name);
    query.bindValue(2, doctor.isNurse);
    query.bindValue(3, doctor.type);
    query.bindValue(4, doctor.depId);
    query.bindValue(5, doctor.depName);
    query.bindValue(6, doctor.phone);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的医护人员。" + exterrstr);
    return;
}

void PsmService::updateDoctor(const PsmSrvDoctor &doctor, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE doctors SET name=?, isnurse=?, type=?, "
                                          "depid=?, depname=?, phonenum=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, doctor.name);
    query.bindValue(1, doctor.isNurse);
    query.bindValue(2, doctor.type);
    query.bindValue(3, doctor.depId);
    query.bindValue(4, doctor.depName);
    query.bindValue(5, doctor.phone);
    query.bindValue(6, doctor.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成医护人员信息更新。" + exterrstr);
    return;
}

void PsmService::deleteDoctor(const QString &doctorid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM doctors WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, doctorid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定医护人员。" + exterrstr);
    return;
}

bool PsmService::readSelectedPatient(QTableWidget *tbl, PsmSrvPatient *patient)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemdob = tbl->item(rowidx, 2);
    QTableWidgetItem *itemphone = tbl->item(rowidx, 3);
    QTableWidgetItem *itemaddr = tbl->item(rowidx, 4);
    QTableWidgetItem *itemcomment = tbl->item(rowidx, 5);
    if (itemid == NULL || itemname == NULL)
        return false;

    patient->id = itemid->text();
    patient->name = itemname->text();
    patient->dob = itemdob->data(Qt::UserRole).toDate();
    patient->phone = itemphone->text();
    patient->address = itemaddr->text();
    patient->comment = itemcomment->text();
    return true;
}

QString PsmService::readSelectedPatientId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshPatientList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2 << 3 << 4 << 5;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << -1 << -1 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM patients;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成患者列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);

}

void PsmService::searchPatient(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2 << 3 << 4 << 5;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << -1 << -1 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM patients "
                       "WHERE UPPER(id) = UPPER(:instr) OR name LIKE :srchstr OR phonenum = :instr;");
    if (!ok)
        goto bad;

    query.bindValue(":instr", srchstr);
    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成患者信息检索。" + exterrstr);
    return;
}

void PsmService::insertPatient(const PsmSrvPatient &patient, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO patients VALUES(?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, patient.id);
    query.bindValue(1, patient.name);
    query.bindValue(2, patient.dob);
    query.bindValue(3, patient.phone);
    query.bindValue(4, patient.address);
    query.bindValue(5, patient.comment);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的患者信息。" + exterrstr);
    return;
}

void PsmService::updatePatient(const PsmSrvPatient &patient, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE patients SET name=?, dob=?, phonenum=?, "
                                           "address=?, comment=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, patient.name);
    query.bindValue(1, patient.dob);
    query.bindValue(2, patient.phone);
    query.bindValue(3, patient.address);
    query.bindValue(4, patient.comment);
    query.bindValue(5, patient.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成患者信息更新。" + exterrstr);
    return;
}

void PsmService::deletePatient(const QString &patientid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM patients WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, patientid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定患者。" + exterrstr);
    return;
}

bool PsmService::readSelectedHospiRec(QTableWidget *tbl, PsmSrvHospiRec *hospirec)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itempatient = tbl->item(rowidx, 1);
    QTableWidgetItem *itemdep = tbl->item(rowidx, 2);
    QTableWidgetItem *itemroomid = tbl->item(rowidx, 3);
    QTableWidgetItem *itemdisease = tbl->item(rowidx, 4);
    QTableWidgetItem *itemdoctor = tbl->item(rowidx, 5);
    QTableWidgetItem *itemnurse = tbl->item(rowidx, 6);
    QTableWidgetItem *itemstart = tbl->item(rowidx, 7);
    QTableWidgetItem *itemend = tbl->item(rowidx, 8);
    if (itemid == NULL || itempatient == NULL)
        return false;

    hospirec->id = itemid->text();
    hospirec->patientid = itempatient->data(Qt::UserRole).toString();
    hospirec->patientname = itempatient->text();
    hospirec->depid = itemdep->data(Qt::UserRole).toString();
    hospirec->depname = itemdep->text();
    hospirec->roomid = itemroomid->text();
    hospirec->disease = itemdisease->text();
    hospirec->doctorid = itemdoctor->data(Qt::UserRole).toString();
    hospirec->doctorname = itemdoctor->text();
    hospirec->nurseid = itemnurse->data(Qt::UserRole).toString();
    hospirec->nursename = itemnurse->text();
    hospirec->startdate = itemstart->data(Qt::UserRole).toDate();
    hospirec->enddate = itemend->data(Qt::UserRole).toDate();
    return true;
}

QString PsmService::readSelectedHospiRecId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

bool PsmService::readOneHospiRec(const QString &hospirecid, PsmSrvHospiRec *hospirec)
{
    if (hospirecid.isEmpty() || hospirec == NULL)
        return false;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM hospi_records WHERE UPPER(id) = UPPER(?);");
    if (!ok)
        return false;

    query.bindValue(0, hospirecid);
    ok = query.exec();
    if (!ok)
        return false;

    if (query.size() < 1)
        return false;

    ok = query.first();
    if (!ok)
        return false;

    QSqlRecord rec = query.record();
    hospirec->id = rec.value(0).toString();
    hospirec->patientid = rec.value(1).toString();
    hospirec->patientname = rec.value(2).toString();
    hospirec->depid = rec.value(3).toString();
    hospirec->depname = rec.value(4).toString();
    hospirec->roomid = rec.value(5).toString();
    hospirec->disease = rec.value(6).toString();
    hospirec->doctorid = rec.value(7).toString();
    hospirec->doctorname = rec.value(8).toString();
    hospirec->nurseid = rec.value(9).toString();
    hospirec->nursename = rec.value(10).toString();
    hospirec->startdate = rec.value(11).toDate();
    hospirec->enddate = rec.value(12).toDate();
    return true;
}

void PsmService::searchHospiRec(const QString &srchstr, const QDate &startdate, const QDate &enddate,
                                QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 2 << 4 << 5 << 6 << 8 << 10 << 11 << 12;
    if (datamap.isEmpty())
        datamap << -1 << 1 << 3 << -1 << -1 << 7 << 9 << 11 << 12;

    if (window == NULL)
        window = this->parent;

    QString dqstr = "";
    if (startdate.isValid())
        dqstr += " AND enddate >= :sdate";
    if (enddate.isValid())
        dqstr += " AND startdate <= :edate";

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM hospi_records "
                       "WHERE (UPPER(id) = UPPER(:instr) OR UPPER(pati_id) = UPPER(:instr) OR "
                              "pati_name LIKE :srchstr OR UPPER(room_id) = UPPER(:instr))" + dqstr + ";");
    if (!ok)
        goto bad;

    query.bindValue(":instr", srchstr);
    query.bindValue(":srchstr", "%" + srchstr + "%");
    if (startdate.isValid())
        query.bindValue(":sdate", startdate);
    if (enddate.isValid())
        query.bindValue(":edate", enddate);
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成住院信息检索。" + exterrstr);
    return;
}

QString PsmService::getCurrentMaxHospiRecId()
{
    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT MAX(id) FROM hospi_records WHERE LENGTH(id) = ?;");
    if (!ok)
        return QString();

    query.bindValue(0, this->hospiRecIdLen);
    ok = query.exec();
    if (!ok)
        return QString();

    if (query.size() < 1)
        return QString();

    ok = query.first();
    if (!ok)
        return QString();

    QSqlRecord rec = query.record();
    QString curid = rec.value(0).toString();
    return curid;
}

bool PsmService::checkHospiRecId(const QString &curid, bool *hasletter)
{
    bool _hasletter = false;

    for (int i = 0; i < curid.length(); i++) {
        QChar ch = curid.at(i);
        if (!ch.isDigit() && !ch.isUpper())
            return false;
        if (ch == 'I' || ch == 'O')
            return false;

        if (ch.isLetter())
            _hasletter = true;
    }
    if (hasletter != NULL)
        *hasletter = _hasletter;
    return true;
}

bool PsmService::needWarningForHospiRecId(const QString &curid, bool hasletter)
{
    QChar warnletter;
    int ignorecnt;
    if (hasletter) {
        warnletter = 'Z';
        ignorecnt = 2;
    } else {
        warnletter = '9';
        ignorecnt = 2;
    }

    for (int i = 0; i < curid.length() - ignorecnt; i++) {
        if (curid.at(i) != warnletter)
            return false;
    }
    return true;
}

QString PsmService::getNextHospiRecId(const QString &curid, bool *hasletter)
{
    bool _hasletter = false;
    bool ok = this->checkHospiRecId(curid, &_hasletter);
    if (!ok)
        return QString();

    QString nextid = curid;
    for (int i = nextid.length() - 1; i >= 0; i--) {
        char ch = nextid.at(i).toLatin1();
        if ((ch >= '0' && ch < '9') || (ch >= 'A' && ch < 'Z') ) {
            if (ch == 'I' - 1 || ch == 'O' - 1)
                nextid.replace(i, 1, QChar::fromLatin1(ch + 2));
            else
                nextid.replace(i, 1, QChar::fromLatin1(ch + 1));
            break;
        } else if (ch == '9' && !_hasletter && i != 0) {
            nextid.replace(i, 1, QChar::fromLatin1('0'));
            continue;
        } else if (ch == '9') {
            nextid.replace(i, 1, QChar::fromLatin1('A'));
            break;
        } else if (ch == 'Z' && i != 0) {
            nextid.replace(i, 1, QChar::fromLatin1('0'));
            continue;
        } else {
            return QString();
        }
    }
    if (hasletter != NULL)
        *hasletter = _hasletter;
    return nextid;
}

QString PsmService::getCurrentNextHostpiRecId(QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    QString curid = getCurrentMaxHospiRecId();
    if (curid.isEmpty())
        return QString("0").repeated(this->hospiRecIdLen - 1) + "1";

    bool hasletter = false;
    QString nextid = getNextHospiRecId(curid, &hasletter);
    if (nextid.isEmpty()) {
        QMessageBox::critical(window, "系统错误", "数据库无法继续生成住院号，请联系开发人员解决。");
        return QString();
    }

    bool needwarn = this->needWarningForHospiRecId(nextid, hasletter);
    if (needwarn) {
        if (hasletter) {
            QMessageBox::warning(window, "系统告警", "可用住院号已接近枯竭，请联系开发人员扩充号码池。");
        } else {
            QMessageBox::warning(window, "系统告警", "纯数字住院号已快用尽，用尽后将引入字母编号。\n"
                                         "不希望使用字母编号，请联系开发人员扩充号码池。");
        }
    }
    return nextid;
}

void PsmService::insertHospiRec(const PsmSrvHospiRec &hospirec, QString *hospirecid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    if (!hospirec.id.isEmpty())
        QMessageBox::warning(window, "警告", "住院号需自动生成，预置号码无效！");

    bool ok;
    QString nextid;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("LOCK TABLES hospi_records WRITE;");
    if (!ok)
        goto bad_nolock;

    nextid = this->getCurrentNextHostpiRecId(window);
    if (nextid.isEmpty())
        goto bad;

    query = this->database.getQuery();
    ok = query.prepare("INSERT INTO hospi_records VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, nextid);
    query.bindValue(1, hospirec.patientid);
    query.bindValue(2, hospirec.patientname);
    query.bindValue(3, hospirec.depid);
    query.bindValue(4, hospirec.depname);
    query.bindValue(5, hospirec.roomid);
    query.bindValue(6, hospirec.disease);
    query.bindValue(7, hospirec.doctorid);
    query.bindValue(8, hospirec.doctorname);
    query.bindValue(9, hospirec.nurseid);
    query.bindValue(10, hospirec.nursename);
    query.bindValue(11, hospirec.startdate);
    query.bindValue(12, hospirec.enddate);
    ok = query.exec();
    if (!ok)
        goto bad;

    query = this->database.getQuery();
    query.exec("UNLOCK TABLES;");

    if (hospirecid != NULL)
        *hospirecid = nextid;
    return;

bad:
    query = this->database.getQuery();
    query.exec("UNLOCK TABLES;");
bad_nolock:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的住院信息。" + exterrstr);
    return;
}

void PsmService::updateHospiRec(const PsmSrvHospiRec &hospirec, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE hospi_records "
                       "SET pati_id=?, pati_name=?, dep_id=?, dep_name=?, room_id=?, disease=?, "
                           "doct_id=?, doct_name=?, nurse_id=?, nurse_name=?, startdate=?, enddate=? "
                       "WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospirec.patientid);
    query.bindValue(1, hospirec.patientname);
    query.bindValue(2, hospirec.depid);
    query.bindValue(3, hospirec.depname);
    query.bindValue(4, hospirec.roomid);
    query.bindValue(5, hospirec.disease);
    query.bindValue(6, hospirec.doctorid);
    query.bindValue(7, hospirec.doctorname);
    query.bindValue(8, hospirec.nurseid);
    query.bindValue(9, hospirec.nursename);
    query.bindValue(10, hospirec.startdate);
    query.bindValue(11, hospirec.enddate);
    query.bindValue(12, hospirec.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成住院信息更新。" + exterrstr);
    return;
}

void PsmService::deleteHospiRec(const QString &hospirecid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM hospi_records WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospirecid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定住院信息。" + exterrstr);
    return;
}

void PsmService::listHospiPhysio(const QString &hospirecid, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 2 << 3 << 4 << 5 << 6;
    if (datamap.isEmpty())
        datamap << 1 << 0 << -1 << 5 << 6;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM hospi_physio WHERE hospi_id = ?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospirecid);
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成住院理疗项目信息检索。" + exterrstr);
    return;
}

void PsmService::insertHospiPhysio(const PsmSrvHospiPhysio &hospiphysio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO hospi_physio VALUES(?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, hospiphysio.hospirecid);
    query.bindValue(1, hospiphysio.physioid);
    query.bindValue(2, hospiphysio.physioname);
    query.bindValue(3, hospiphysio.freqperiod);
    query.bindValue(4, hospiphysio.freqcount);
    query.bindValue(5, hospiphysio.startdate);
    query.bindValue(6, hospiphysio.enddate);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加住院理疗项目信息。" + exterrstr);
    return;
}
