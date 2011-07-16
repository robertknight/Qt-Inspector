#pragma once

#include <QtCore/QObject>

class ObjectProxy;

class QLocalSocket;

/** Represents an external Qt process. */
class TargetApplicationProxy : public QObject
{
	Q_OBJECT

	public:
		TargetApplicationProxy();

		bool connect(int pid);
		void disconnect();

		/** Return a list of top-level widgets in the application. */
		QList<ObjectProxy*> fetchTopLevelWidgets();

	private:
		QLocalSocket* m_socket;
};

