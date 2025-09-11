/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPENSR_RESOURCEMANAGER_P_H
#define OPENSR_RESOURCEMANAGER_P_H

#include "OpenSR/ResourceManager.h"

#include <OpenSR/libRangerQt.h>
#include <QNetworkReply>
#include <QDir>

class QFile;
class QBuffer;

namespace OpenSR
{
class ResourceManagerNAM : public QNetworkAccessManager
{
    Q_OBJECT
public:
    ResourceManagerNAM(ResourceManager *manager, QObject *parent);
    ~ResourceManagerNAM() override;

    QNetworkReply *createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData) override;

private:
    ResourceManager *m_manger;

private Q_SLOTS:
    void emitReplyFinished();
};

class ResourceReply : public QNetworkReply
{
    Q_OBJECT
public:
    ~ResourceReply() override;

protected:
    ResourceReply(const QUrl &url, QIODevice *device, QObject *parent);

    qint64 bytesAvailable() const override;
    bool canReadLine() const override;
    bool isSequential() const override;
    qint64 size() const override;
    bool seek(qint64 pos) override;

    void close() override;

public Q_SLOTS:
    void abort() override;

protected:
    qint64 readData(char *data, qint64 maxSize) override;

private:
    QIODevice *m_device;
    friend class ResourceManagerNAM;

private Q_SLOTS:
    void emitFinished();
};

class FSProvider : public ResourceProvider
{
public:
    FSProvider(const QString &dir);
    ~FSProvider() override;

    void load(ResourceNode &root) override;
    QIODevice *getDevice(const ResourceNode &node, QObject *parent = nullptr) override;

private:
    void load(ResourceNode &current, const QDir &dir);
    QString m_dir;
};

class PGKResourceInfo : public ResourceInfo
{
public:
    PGKResourceInfo(PKGItem *item);
    ~PGKResourceInfo() override;

    PKGItem *item;
};

class PKGIODevice : public QIODevice
{
public:
    PKGIODevice(const PKGItem &item, QIODevice *archiveDev, QObject *parent = 0);
    ~PKGIODevice() override;

    qint64 bytesAvailable() const override;
    bool canReadLine() const override;
    bool isSequential() const override;
    qint64 size() const override;
    bool seek(qint64 pos) override;

    void close() override;

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QByteArray m_data;
    QBuffer *m_buffer;
};

class PKGProvider : public ResourceProvider
{
public:
    PKGProvider(const QString &file);
    ~PKGProvider() override;

    void load(ResourceNode &root) override;
    QIODevice *getDevice(const ResourceNode &node, QObject *parent = nullptr) override;

private:
    void load(ResourceNode &current, PKGItem *item);
    void cleanup(PKGItem *item);

    PKGItem *m_root;
    QString m_path;
};

} // namespace OpenSR

#endif // OPENSR_RESOURCEMANAGER_P_H
