
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtNetwork/QLocalServer>

#include "InspectorServer.h"
#include "StartupHelper.h"

#include <iostream>

extern void qtInspectorInit();

class LogFile
{
	public:
		LogFile(const QString& fileName)
		{
			m_file.setFileName(fileName);
			m_file.open(QIODevice::WriteOnly);
			m_stream.setDevice(&m_file);
		}

		QTextStream& stream()
		{
			return m_stream;
		}

	private:
		QFile m_file;
		QTextStream m_stream;
};

#ifdef Q_OS_UNIX
#define LIB_INIT_FUNC __attribute__((constructor))
#else
#define LIB_INIT_FUNC
#endif

LIB_INIT_FUNC void qtInspectorLibInit()
{
	StartupHelper* initHelper = new StartupHelper(qtInspectorInit);
	QObject::connect(initHelper, SIGNAL(startupComplete()), initHelper, SLOT(deleteLater()));
	initHelper->watchForStartup();
}

static QScopedPointer<LogFile> logFile;

// entry point for the inspector helper library
void qtInspectorInit()
{
	QString appName = QFileInfo(QCoreApplication::applicationFilePath()).baseName();
	if (appName == "qtinspector")
	{
		return;
	}

	if (logFile)
	{
		logFile->stream() << "Hmm, we've already attached to this app\n";
		logFile->stream().flush();
		return;
	}
	
	QString logName = QString("/tmp/inspector-log-%1").arg(QCoreApplication::applicationPid());
	LogFile* log = new LogFile(logName);
	logFile.reset(log);
	logFile->stream() << "Hello from the injected app\n";

	new InspectorServer(&logFile->stream(),QCoreApplication::instance());

	logFile->stream().flush();
}

