
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtNetwork/QLocalServer>

#include "InspectorServer.h"

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

static QScopedPointer<LogFile> logFile;

// entry point for the inspector helper library
void qtInspectorInit()
{
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

