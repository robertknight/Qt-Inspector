
#include "ExternalWidgetPicker.h"
#include "TargetApplicationProxy.h"
#include "WidgetInspector.h"

#include "ObjectProxy.h"

#include <QtGui/QApplication>
#include <QtCore/QProcess>

#include <QtDebug>

int main(int argc, char** argv)
{
	QApplication app(argc,argv);

	QStringList args = app.arguments();
	if (args.count() < 2)
	{
		qWarning() << "Usage: qtinspector <pid>|<program>";
		return -1;
	}

	QProcess process;
	int targetPid = args.at(1).toInt();
	if (targetPid == 0)
	{
		process.start(args.at(1));
		process.waitForStarted();
		targetPid = process.pid();
	}

	qDebug() << "connecting to app" << targetPid;

	TargetApplicationProxy proxy;
	if (!proxy.connectToTarget(targetPid))
	{
		qWarning() << "Failed to inject helper library into process <pid>";
	}

	WidgetPicker* picker = new ExternalWidgetPicker(&proxy,0);

	WidgetInspector inspector(&proxy);
	inspector.setWidgetPicker(picker);
	inspector.show();

	int result = app.exec();

	if (process.state() == QProcess::Running && !process.waitForFinished())
	{
		qWarning() << "Failed to wait for process" << process.pid() << "to exit";
	}

	return result;
}

