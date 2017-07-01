/*-
 * GNU GENERAL PUBLIC LICENSE, version 3
 * See LICENSE file for detail.
 *
 * Author: Yulong Yu, Jun 29th, 2017
 * Copyright(c) 2015-2017 Yulong Yu. All rights reserved.
 */

#include "PsmDatabase.h"

#include <QIODevice>
#include <QFile>
#include <QtXml>

QString PsmDatabase::connectname("PsmDb");

PsmDatabase::PsmDatabase()
{
    this->host = QString();
    this->username = QString();
    this->password = QString();
    this->schema = QString();
    this->isStarted = false;
}

QString PsmDatabase::getHost() const
{
    return this->host;
}

QString PsmDatabase::getUsername() const
{
    return this->username;
}

QString PsmDatabase::getPassword() const
{
    return this->password;
}

QString PsmDatabase::getSchema() const
{
    return this->schema;
}

bool PsmDatabase::isConnectionConfigured() const
{
    return (this->host.isNull() || this->username.isNull() || this->password.isNull() || this->schema.isNull());
}

void PsmDatabase::configConnection(QString host, QString username, QString password, QString schema)
{
    if (this->isStarted)
        return;

    this->host = host;
    this->username = username;
    this->password = password;
    this->schema = schema;
}

void PsmDatabase::configConnectionFromFile(QString path)
{
    if (this->isStarted)
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument domdoc("PsmDbConfig");
    if (!domdoc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement docelem = domdoc.documentElement();
    if (QString::compare(docelem.tagName(), "database") != 0)
        return;

    QDomNode domnode = docelem.firstChild();
    while (!domnode.isNull()) {
        QDomElement domelem = domnode.toElement();
        if (QString::compare(domelem.tagName(), "host") == 0)
            this->host = domelem.text();
        else if (QString::compare(domelem.tagName(), "username") == 0)
            this->username = domelem.text();
        else if (QString::compare(domelem.tagName(), "password") == 0)
            this->password = domelem.text();
        else if (QString::compare(domelem.tagName(), "schema") == 0)
            this->schema = domelem.text();

        domnode = domnode.nextSibling();
    }
}

bool PsmDatabase::startDatabase()
{
    if (!this->isConnectionConfigured())
        return false;
    if (this->isStarted)
        return true;

    if (QSqlDatabase::contains(connectname))
        QSqlDatabase::removeDatabase(connectname);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectname);
    if (!db.isValid())
        return false;

    db.setHostName(this->host);
    db.setUserName(this->username);
    db.setPassword(this->password);
    db.setDatabaseName(this->schema);
    this->isStarted = true;

    // Try open and close the database.
    if (!db.open())
        return true;
    db.close();
    return true;
}


void PsmDatabase::stopDatabase()
{
    if (!this->isStarted)
        return;

    QSqlDatabase::removeDatabase(connectname);
    this->isStarted = false;
    return;
}

bool PsmDatabase::isDatabaseStarted() const
{
    return this->isStarted;
}
