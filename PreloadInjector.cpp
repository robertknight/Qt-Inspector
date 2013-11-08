#include "PreloadInjector.h"

#include "lib/InspectorServer.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QProcessEnvironment>

#include <unistd.h>

bool PreloadInjector::startAndInject(const QString& program, const QStringList& args,
  const QString& libraryPath, const QString& entryPoint, int* pid)
{
	*pid = 0;

	QProcess* process = new QProcess;
	QObject::connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), process, SLOT(deleteLater()));
	process->setProcessChannelMode(QProcess::ForwardedChannels);

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

	QString libPath = QFileInfo(libraryPath).absoluteFilePath();
	if (!QFile::exists(libPath))
	{
		return false;
	}

#ifdef Q_OS_MAC
	QString var = "DYLD_INSERT_LIBRARIES";
	QString value = env.value(var);
	env.insert(var, QString("%1:%2").arg(libraryPath).arg(value));
#elif defined(Q_OS_LINUX)
	QString var = "LD_PRELOAD";
	QString value = env.value(var);
	env.insert(var, QString("%1 %2").arg(libraryPath).arg(value));
#else
#error Platform not supported
#endif

	process->setProcessEnvironment(env);
	process->start(program, args);
	if (!process->waitForStarted())
	{
		qWarning() << "Failed to start" << program;
		return false;
	}

	*pid = process->pid();

	QString socketPath = InspectorServer::socketName(*pid);
	while (!QFile::exists(socketPath))
	{
#ifdef Q_OS_UNIX
		usleep(100 * 1000);
#endif
	}

	return true;
}

bool PreloadInjector::inject(int pid, const QString& libraryPath, const QString& entryPoint)
{
	Q_UNUSED(pid);
	Q_UNUSED(libraryPath);
	Q_UNUSED(entryPoint);
	return false;
}

