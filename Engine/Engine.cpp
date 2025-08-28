/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2017 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Engine.h"

#include <OpenSR/libRangerQt.h>

#include "OpenSR/DATTranslator.h"
#include "OpenSR/PluginInterface.h"
#include "OpenSR/QMLHelper.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/SoundManager.h"

#include <QBoxLayout>
#include <QDebug>
#include <QDir>
#include <QJSEngine>
#include <QPluginLoader>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QResource>
#include <QSettings>
#include <QString>

namespace OpenSR
{
namespace
{
void mergeMap(QVariantMap &source, const QVariantMap &append)
{
    auto end = append.end();
    for (auto i = append.begin(); i != end; ++i)
    {
        auto target = source.find(i.key());
        if (target == source.end())
        {
            source.insert(i.key(), i.value());
        }
        else
        {
            *target = append;
        }
    }
}
} // namespace

class Engine::EnginePrivate
{
public:
    EnginePrivate()
    {
    }

    QScopedPointer<QQmlApplicationEngine> qmlEngine;
    SoundManager *sound = nullptr;
    ResourceManager *resources = nullptr;
    QVariantMap datRoot;
    QString dataDir;
    QUrl startupScript;
    QUrl mainQML;
    bool running = false;
    bool testMode = false;
};

Engine::Engine(int &argc, char **argv) : QApplication(argc, argv), d_osr_ptr(new EnginePrivate())
{
    Q_D(Engine);

    installTranslator(new DATTranslator(this));

    d->sound = new SoundManager(this);
    d->resources = new ResourceManager(this);

    d->qmlEngine.reset(new QQmlApplicationEngine());

    QML::QMLHelper::registerQMLTypes("OpenSR");
    d->qmlEngine->setNetworkAccessManagerFactory(d->resources->qmlNAMFactory());

    d->qmlEngine->globalObject().setProperty("Engine", d->qmlEngine->newQObject(this));
    d->qmlEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);
    d->qmlEngine->rootContext()->setContextProperty("isTestMode", false);

    d->testMode = arguments().contains("--test-mode");

    if (d->testMode)
    {
        d->qmlEngine->rootContext()->setContextProperty("isTestMode", true);
    }
}

Engine::~Engine()
{
    Q_D(Engine);
}

int Engine::run()
{
    Q_D(Engine);

    QString libraryPath = QDir::current().absolutePath();

    QSettings settings;

    QString dataDir = settings.value("engine/mainDataDir").toString();
    if (!dataDir.isEmpty())
    {
        setDataDir(dataDir);
    }
    else if (d->dataDir.isEmpty())
    {
        setDataDir(QDir::current().absolutePath() + "/data");
    }

    addLibraryPath(libraryPath);

    d->qmlEngine->addImportPath(d->dataDir);
    d->qmlEngine->addImportPath(":/");

    d->qmlEngine->addPluginPath(libraryPath);

    d->resources->addFileSystemPath(d->dataDir);
    d->resources->addFileSystemPath(":/");

    QUrl qml = settings.value("engine/mainQML").toString();
    if (!qml.isEmpty())
    {
        setMainQML(qml);
    }

    QUrl startup = settings.value("engine/startupScript").toString();
    if (!startup.isEmpty())
    {
        setStartupScript(startup);
    }

    d->sound->start();

    auto scriptExec = [this] {
        Q_D(Engine);
        if (!d->startupScript.isEmpty() && !d->testMode)
        {
            execScript(d->startupScript);
        } else {
            execScript(QUrl("res:/opensrTestMode.js"));
        }
    };

    if (!d->mainQML.isEmpty())
    {
        d->qmlEngine->load(d->mainQML);
        connect(d->qmlEngine.get(), &QQmlApplicationEngine::objectCreated, scriptExec);
    }
    else
    {
        scriptExec();
    }

    d->running = true;

    return exec();
}

void Engine::addRCCArchive(const QString &path)
{
    QResource::registerResource(path);
}

void Engine::showQMLComponent(const QString &url)
{
    Q_D(Engine);

    for (auto root : d->qmlEngine->rootObjects())
    {
        QMetaObject::invokeMethod(root, "destroyAndChangeScreen", Q_ARG(QVariant, QUrl(url)),
                                  Q_ARG(QVariant, QVariantMap()));
    }
}

SoundManager *Engine::sound() const
{
    Q_D(const Engine);
    return d->sound;
}

ResourceManager *Engine::resources() const
{
    Q_D(const Engine);
    return d->resources;
}

QQmlEngine *Engine::qmlEngine() const
{
    Q_D(const Engine);
    return d->qmlEngine.get();
}

QJSEngine *Engine::scriptEngine() const
{
    Q_D(const Engine);
    return d->qmlEngine.get();
}

void Engine::addDATFile(const QString &url, bool isCache)
{
    Q_D(Engine);
    QScopedPointer<QIODevice> dev(d->resources->getIODevice(QUrl(url)));
    if (!dev || !dev->isOpen())
    {
        return;
    }
    QVariantMap dat = loadDAT(dev.get(), isCache);
    mergeMap(d->datRoot, dat);
}

QVariant Engine::datValue(const QString &path) const
{
    Q_D(const Engine);

    if (path.isEmpty())
    {
        return QVariant();
    }

    QList<QString> pathes = path.split('.', Qt::SkipEmptyParts);
    QVariantMap current = d->datRoot;
    QVariant result = current;

    for (const QString &p : pathes)
    {
        auto it = current.find(p);
        if (it == current.end())
        {
            qWarning() << "No such DAT record: " << path;
            return QVariant();
        }
        result = it.value();
        if (result.typeId() != QMetaType::QVariantMap)
        {
            break;
        }
        current = result.toMap();
    }
    return result;
}

void Engine::loadPlugin(const QString &name)
{
    QPluginLoader loader(name);
    if (!loader.load())
    {
        qWarning().noquote() << QString("Cannot load plugin \"%1\": %2").arg(name, loader.errorString());
        return;
    }
    // TODO: Unloading
    PluginInterface *plugin = qobject_cast<PluginInterface *>(loader.instance());
    if (!plugin)
    {
        qWarning().noquote() << QString("Cannot load plugin \"%1\": %2").arg(name, tr("seems not a plugin."));
        loader.unload();
        return;
    }
    if (!plugin->initPlugin(this))
    {
        qWarning().noquote() << QString("Cannot load plugin \"%1\"").arg(name);
        loader.unload();
        return;
    }
    loader.instance()->setParent(this);
}

void Engine::execScript(const QUrl &url)
{
    Q_D(Engine);

    QScopedPointer<QIODevice> dev(resources()->getIODevice(url));
    if (!dev)
    {
        qWarning().noquote() << QString("Cannot exec script %1: file not found.").arg(url.toString());
        return;
    }

    QString script = QString::fromUtf8(dev->readAll());
    dev->close();

    QJSValue result = d->qmlEngine->evaluate(script, url.toString());
    if (result.isError())
    {
        qWarning().noquote()
            << QString("%1:%2: %3").arg(url.toString(), result.property("lineNumber").toString(), result.toString());
    }
}

QString Engine::dataDir() const
{
    Q_D(const Engine);
    return d->dataDir;
}

void Engine::setDataDir(const QString &dir)
{
    Q_D(Engine);
    if (d->running)
    {
        qWarning() << "Cannot set data dir while Engine is running";
        return;
    }
    if (dir != d->dataDir)
    {
        d->dataDir = dir;
        emit(dataDirChanged());
    }
}

QUrl Engine::startupScript() const
{
    Q_D(const Engine);
    return d->startupScript;
}

void Engine::setStartupScript(const QUrl &script)
{
    Q_D(Engine);
    if (d->running)
    {
        qWarning() << "Cannot set startup script while Engine is running";
        return;
    }
    if (script != d->startupScript)
    {
        d->startupScript = script;
        emit(startupScriptChanged());
    }
}

QUrl Engine::mainQML() const
{
    Q_D(const Engine);
    return d->mainQML;
}

void Engine::setMainQML(const QUrl &qml)
{
    Q_D(Engine);
    if (d->running)
    {
        qWarning() << "Cannot set main QML component while Engine is running";
        return;
    }
    if (qml != d->mainQML)
    {
        d->mainQML = qml;
        emit(mainQMLChanged());
    }
}
} // namespace OpenSR
