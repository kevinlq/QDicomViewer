#include "QListPanel.h"
#include "../CoreInclude.h"

#include "../coreconstants.h"
#include "../CoreDataStruct.h"

#include "ListDelegate.h"
#include "DicomListView.h"
#include "QListPanelManager.h"

#define BACKGROUND_STYLE QLatin1String("QWidget{background-color:rgb(90,90,90);}")
#define NORMAL_STYLE QLatin1String("QWidget{background-color:rgb(130,130,130);}")
#define HIGHLINE_STYLE QLatin1String("QWidget{background-color:rgb(180,180,180);}")

using namespace Core;


QListPanel::QListPanel(QWidget *parent) : QWidget(parent)
{
    setStyleSheet(HIGHLINE_STYLE);

    m_pModel = new QStandardItemModel;

    m_pListView = new DicomListView(this);
    m_pListView->setItemDelegate(new ListDelegate(this));
    m_pListView->setModel(m_pModel);
    m_pListView->setSpacing(2);
    m_pListView->setViewMode(QListView::IconMode);
    m_pListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListView->setDragEnabled(true);
    m_pListView->setAcceptDrops(true);
    m_pListView->setDragDropMode(QAbstractItemView::DragDrop);
    m_pListView->setMouseTracking(true);
    m_pListView->setMovement(QListView::Free);
    m_pListView->setSelectionMode(QAbstractItemView::SingleSelection);

    //m_pListView->setSelectionBehavior(QAbstractItemView::SelectItems);

    m_pListView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    QVBoxLayout* pVLayout = new QVBoxLayout;
    pVLayout->setSpacing(0);
    pVLayout->addWidget(m_pListView);
    //pVLayout->addStretch();
    pVLayout->setContentsMargins(0,0,0,0);

    setLayout(pVLayout);
}

QListPanel::~QListPanel()
{
    if (nullptr != m_pModel)
    {
        m_pModel->clear();
        delete m_pModel;
        m_pModel = nullptr;
    }
}

void QListPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QRect rc = this->rect();

    painter.fillRect(rc, Constants::BACKGROUND_COLOR);
}

void QListPanel::onAddItem(const ListItemData &pData)
{
    QStandardItem* pItem = new QStandardItem ();
    pItem->setData(QVariant::fromValue(pData), Qt::UserRole+1);
    m_pModel->appendRow(pItem);
}
