#include "TargetApplicationProxy.h"
#include "WidgetInspector.h"

#include <QtGui/QApplication>

#include <QtDebug>

int main(int argc, char** argv)
{
	QApplication app(argc,argv);

	QStringList args = app.arguments();
	if (args.count() < 2)
	{
		qWarning() << "Usage: qtinspector <pid>";
		return -1;
	}

	int targetPid = args.at(1).toInt();
	qDebug() << "connecting to app" << targetPid;

	TargetApplicationProxy proxy;
	if (!proxy.connect(targetPid))
	{
		qWarning() << "Failed to inject helper library into process <pid>";
	}

	WidgetInspector inspector;
	inspector.show();

	return app.exec();
}

