#pragma once

#include "ObjectProxy.h"

/** Interface which provides access to a list of root objects
  * for use with the WidgetInspector
  */
class RootObjectList
{
	public:
		virtual QList<ObjectProxy::Pointer> rootObjects() = 0;
};

