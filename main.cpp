
#include "ExternalWidgetPicker.h"
#include "TargetApplicationProxy.h"
#include "WidgetInspector.h"

#include "lib/ObjectProxy.h"

#ifdef Q_OS_UNIX
#include "GdbLibraryInjector.h"
#include "PreloadInjector.h"
#elif defined(Q_OS_WIN32)
#include "WindowsHookInjector.h"
#endif

#include <QApplication>
#include <QtCore/QProcess>

#include <QtDebug>

QString injectorLibPath()
{
#ifdef Q_OS_LINUX
	return "lib/libQtInspector.so";
#elif defined(Q_OS_MAC)
	return "lib/libQtInspector.dylib";
#else
	return "QtInspector.dll";
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
	QString entryPoint = "qtInspectorInit";

#ifdef Q_OS_WIN32
	injector.reset(new WindowsHookInjector);
	entryPoint = "_qtInspectorLibInit@12";
#endif

	if (targetPid != 0)
	{
#ifdef Q_OS_UNIX
		injector.reset(new GdbLibraryInjector);
#endif
		if (!injector->inject(targetPid, injectorLibPath(), entryPoint))
		{
			return false;
		}
	}
	else
	{
#ifdef Q_OS_UNIX
		injector.reset(new PreloadInjector);
#endif
		QStringList programArgs;
		for (int i=2; i < args.count(); i++)
		{
			programArgs << args.at(i);
		}
		if (!injector->startAndInject(args.at(1),programArgs,injectorLibPath(),entryPoint,&targetPid))
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

