
#include "WidgetInspector.h"

#include "ObjectInspector.h"
#include "ObjectTreeModel.h"
#include "OutOfProcessClipboard.h"
#include "WidgetPicker.h"
#include "WidgetInspectorShortcut.h"

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>

#include <QtDebug>

WidgetInspector::WidgetInspector(QWidget* parent)
: QWidget(parent)
, m_externalClipboard(new OutOfProcessClipboard(this))
{
	setWindowTitle(tr("Widget Inspector"));

	m_objectModel = new ObjectTreeModel(this);
	m_objectTree = new QTreeView(this);
	m_objectTree->header()->setVisible(false);
	m_objectTree->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_objectTree->setModel(m_objectModel);
	m_objectTree->header()->setResizeMode(0,QHeaderView::ResizeToContents);
	m_objectTree->header()->setStretchLastSection(false);

	connect(m_objectTree->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
	        this,SLOT(selectionChanged(QModelIndex,QModelIndex)));

	m_objectInspector = new ObjectInspector(this);

	QVBoxLayout* layout = new QVBoxLayout(this);

	QSplitter* topSplitter = new QSplitter(this);
	topSplitter->addWidget(m_objectTree);
	topSplitter->addWidget(m_objectInspector);
	layout->addWidget(topSplitter);

	QHBoxLayout* searchLayout = new QHBoxLayout;
	searchLayout->addWidget(new QLabel(tr("Search:"),this));

	QLineEdit* searchEdit = new QLineEdit(this);
	connect(searchEdit,SIGNAL(textChanged(QString)),this,SLOT(search(QString)));
	searchLayout->addWidget(searchEdit);
	layout->addLayout(searchLayout);

	QPushButton* copyToDebuggerButton = new QPushButton(tr("Copy Reference"),this);
	connect(copyToDebuggerButton,SIGNAL(clicked()),
	        this,SLOT(copyDebuggerReference()));

	m_picker = new WidgetPicker(this);
	connect(m_picker,SIGNAL(widgetPicked(QWidget*)),this,SLOT(pickerFinished(QWidget*)));
	QPushButton* pickButton = new QPushButton(tr("Pick Widget"),this);
	connect(pickButton,SIGNAL(clicked()),m_picker,SLOT(start()));

	QPushButton* refreshButton = new QPushButton(tr("Refresh"),this);
	connect(refreshButton,SIGNAL(clicked()),this,SLOT(resetModel()));

	QHBoxLayout* actionLayout = new QHBoxLayout;
	actionLayout->addStretch();
	actionLayout->addWidget(pickButton);
	actionLayout->addWidget(copyToDebuggerButton);
	actionLayout->addWidget(refreshButton);
	layout->addLayout(actionLayout);

	resize(700,400);

	resetModel();
}

void WidgetInspector::pickerFinished(QWidget* widget)
{
	select(widget);
}

void WidgetInspector::copyDebuggerReference()
{
	QObject* object = m_objectInspector->object();
	if (!object)
	{
		return;
	}
	QString reference = QString("(%1*)(%2)")
	  .arg(object->metaObject()->className())
	  .arg(ObjectInspector::formatAddress(object));

#ifdef Q_OS_LINUX
	m_externalClipboard->setText(reference);
#else
	QApplication::clipboard()->setText(reference);
#endif
}

void WidgetInspector::selectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous);
	m_objectInspector->setObject(ObjectTreeModel::objectFromIndex(current));
}

void WidgetInspector::resetModel()
{
	QList<QObject*> objects;
	Q_FOREACH(QObject* object, QApplication::topLevelWidgets())
	{
		objects << object;
	}
	m_objectModel->setRootObjects(objects);
}

void WidgetInspector::search(const QString& query)
{
	QList<QObject*> results = m_objectModel->search(query);
	if (!results.isEmpty())
	{
		select(results.first());
	}
}

void WidgetInspector::select(QObject* object)
{
	QModelIndex index = m_objectModel->index(object);
	if (!index.isValid())
	{
		// if no matching object is found in the model, then try refreshing
		// the model and searching again
		resetModel();
		index = m_objectModel->index(object);
	}

	m_objectTree->scrollTo(index);
	m_objectTree->selectionModel()->setCurrentIndex(index,QItemSelectionModel::SelectCurrent);
}

void WidgetInspector::registerGlobalShortcut(const QKeySequence& key, QWidget* parentWidget)
{
	WidgetInspectorShortcut* shortcut = new WidgetInspectorShortcut(parentWidget);
	shortcut->setKey(key);
	shortcut->setContext(Qt::ApplicationShortcut);
}

