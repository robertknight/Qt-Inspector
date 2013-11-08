
#include "ExternalWidgetPicker.h"
#include "GdbLibraryInjector.h"
#include "PreloadInjector.h"
#include "TargetApplicationProxy.h"
#include "WidgetInspector.h"

#include "lib/ObjectProxy.h"

#include <QtGui/QApplication>
#include <QtCore/QProcess>

#include <QtDebug>

QString injectorLibPath()
{
#ifdef Q_OS_LINUX
	return "lib/libQtInspector.so";
#elif defined(Q_OS_MAC)
	return "lib/libQtInspector.dylib";
#else
#error Platform not supported
#endif
}

int main(int argc, char** argv)
{
	QApplication app(argc,argv);

	QStringList args = app.arguments();
	if (args.count() < 2)
	{
		qWarning() << "Usage: qtinspector <pid>|<program> (<args>...)";
		return -1;
	}

	QProcess process;
	int targetPid = args.at(1).toInt();

	// inject the helper library
	QScopedPointer<Injector> injector;
	if (targetPid != 0)
	{
		injector.reset(new GdbLibraryInjector);
		if (!injector->inject(targetPid, injectorLibPath(), "qtInspectorInit"))
		{
			return false;
		}
	}
	else
	{
		injector.reset(new PreloadInjector);
		QStringList programArgs;
		for (int i=2; i < args.count(); i++)
		{
			programArgs << args.at(i);
		}
		if (!injector->startAndInject(args.at(1),programArgs,injectorLibPath(),"qtInspectorInit",&targetPid))
		{
			return false;
		}
	}

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

