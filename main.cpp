
#include "ExternalWidgetPicker.h"
#include "TargetApplicationProxy.h"
#include "WidgetInspector.h"

#include "ObjectProxy.h"

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
	if (!proxy.connectToTarget(targetPid))
	{
		qWarning() << "Failed to inject helper library into process <pid>";
	}

	WidgetPicker* picker = new ExternalWidgetPicker(&proxy,0);

	WidgetInspector inspector;
	inspector.setRootObjects(proxy.fetchTopLevelWidgets());
	inspector.setWidgetPicker(picker);
	inspector.show();

	return app.exec();
}

