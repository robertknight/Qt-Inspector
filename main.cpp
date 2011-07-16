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
	QList<ObjectProxy*> objects = proxy.fetchTopLevelWidgets();
	Q_FOREACH(ObjectProxy* proxy, objects)
	{
		qDebug() << "fetched object" << proxy->className() << proxy->objectName() << "with" <<
		         proxy->children().count() << "children";
	}

	WidgetInspector inspector;
	inspector.show();

	return app.exec();
}

