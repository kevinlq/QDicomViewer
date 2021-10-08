#include "ContentWidgetContainer.h"

#include "CoreInclude.h"
#include "coreconstants.h"
#include "CoreUtil.h"
#include "CoreDataStruct.h"

#include "ListPanel/QListPanel.h"
#include "ListPanel/QListPanelManager.h"
#include "ViewPanel/ViewPanel.h"
#include "StatusPanel/QStatusPanel.h"

using namespace Core;
using namespace Core::Internal;

ContentWidgetContainer::ContentWidgetContainer(QWidget *parent)
    : QWidget(parent)
    , m_pListPanel(new QListPanel(this))
    , m_pViewPanel(new ViewPanel(this))
    , m_pStatusPanel(new QStatusPanel(this))
{
    m_pListPanel->setFixedWidth(Constants::LIST_PANEL_WIDTH);
    m_pListPanel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_pViewPanel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_pStatusPanel->setMaximumHeight(Constants::STATUS_PANEL_HEIGHT);
    m_pStatusPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    QHBoxLayout* pHLayout = new QHBoxLayout;
    pHLayout->setSpacing(0);
    pHLayout->addWidget(m_pListPanel);
    pHLayout->addWidget(m_pViewPanel);
    pHLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout* pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setSpacing(0);
    pVBoxLayout->addLayout(pHLayout);
    pVBoxLayout->addWidget(m_pStatusPanel);
    pVBoxLayout->setContentsMargins(0,0,0,0);

    setLayout(pVBoxLayout);

    connect(ListPanelMgr, &QListPanelManager::signalAddItem, m_pListPanel, &QListPanel::onAddItem);
    connect(m_pViewPanel, &ViewPanel::signalAddDicom, [](const QVariantList &lsFiles){
        qDebug() << "lsFiles " << lsFiles;

        QFileInfoList lsFileInfos;
        CoreUtil::getDicomFiles(lsFiles,lsFileInfos);

        qDebug() << "########## file info ===============";
        for (int i = 0; i < lsFileInfos.size(); i++)
        {
            qDebug() << "file:" << lsFileInfos[i].filePath() << lsFileInfos[i].fileName();
        }
    });
}

ContentWidgetContainer::~ContentWidgetContainer()
{
}

bool ContentWidgetContainer::init()
{
    // test
    ListItemData *pData = new ListItemData;
    pData->m_strPatientName = "CHEN";
    pData->m_strPatientBirthday = "1949/10/1";
    pData->m_strSeriesDataTime = "2003/5/10 8:30:18";
    pData->m_strModaly = "MR";
    pData->m_nSeriesNumber = 1;
    pData->m_bTitleInfo = true;

    ListItemData* pDataThumb1 = new ListItemData;
    pDataThumb1->copyFrom(pData);
    pDataThumb1->m_bTitleInfo = false;
    QImage image(128,128, QImage::Format_ARGB32);
    image.fill(QColor(65,65,100));

    pDataThumb1->m_strDescription = "Test dicom 1";
    pDataThumb1->m_baThumbnailBuffer =  CoreUtil::imageToBuffer(&image);

    ListItemData* pDataThumb2 = new ListItemData;
    pDataThumb2->copyFrom(pData);
    pDataThumb2->m_bTitleInfo = false;

    image.fill(QColor(125,100,200));
    pDataThumb2->m_strDescription = "Test dicom 2";
    pDataThumb2->m_baThumbnailBuffer = CoreUtil::imageToBuffer(&image);

    QListPanelManager::getInstance()->addItem(pData);
    QListPanelManager::getInstance()->addItem(pDataThumb1);
    QListPanelManager::getInstance()->addItem(pDataThumb2);

    return true;
}

void ContentWidgetContainer::updateLayout(int nRow, int nCol)
{
    m_pViewPanel->updateLayout(nRow, nCol);
}
