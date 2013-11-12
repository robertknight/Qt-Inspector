#pragma once

#include "Injector.h"

class WindowsHookInjector : public Injector
{
    public:
        virtual bool startAndInject(const QString& program, const QStringList& args,
                            const QString& libraryPath, const QString& entryPoint,
                            int* pid);
        virtual bool inject(int pid, const QString& libraryPath, const QString& entryPoint);
};
