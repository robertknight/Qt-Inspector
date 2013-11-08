#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>

typedef void(*InitFunc)();

class StartupHelper : public QObject
{
		Q_OBJECT

	public:
		StartupHelper(InitFunc initFunc)
			: m_initFunc(initFunc)
		{}

		void watchForStartup();
		

	Q_SIGNALS:
		void startupComplete();

	private Q_SLOTS:
		void runInitFunc();	

	private:
		void doWait();

		InitFunc m_initFunc;
};


