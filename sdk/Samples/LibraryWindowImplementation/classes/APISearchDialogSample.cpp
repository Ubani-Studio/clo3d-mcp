#include "APISearchDialogSample.h"

#include <QDebug>

#include "APIStorage.h"
#include "CLOAPIInterface.h"

namespace CLOAPISample
{	
	APISearchDialogSample::APISearchDialogSample(QWidget* parent, Qt::WindowFlags flags, SEARCH_ITEM_MODE mode)
	: QDialog(parent, flags)
	{
		setupUi(this);

		UTILITY_API->UpdateCloStyleForPlugIn(this);

		m_SearchMode = mode;

		generateUI();

		QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(OnAccept()));
		QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnReject()));
		QObject::connect(cbServiceDivision, SIGNAL(currentIndexChanged(int)), this, SLOT(OnServiceDivisionComboBox(int)));
	}
	
	APISearchDialogSample::~APISearchDialogSample()
	{

	}

	void APISearchDialogSample::SetSearchMode(SEARCH_ITEM_MODE mode)
	{
		m_SearchMode = mode;

		if (m_SearchMode == SEARCH_PATTERN)
			lblTitle->setText("Pattern Search Criteria");
		else if (m_SearchMode == SEARCH_TRIM)
			lblTitle->setText("TRIM Search Criteria");
		else if (m_SearchMode == SEARCH_FABRIC)
			lblTitle->setText("Fabric Search Criteria");
		else if(m_SearchMode == SEARCH_PROJECT)
			lblTitle->setText("Project Search Criteria");
		else if (m_SearchMode == SEARCH_AVATAR)
			lblTitle->setText("Avatar Search Criteria");
		else if (m_SearchMode == SEARCH_GARMENT)
			lblTitle->setText("Garment Search Criteria");
		else if (m_SearchMode == SEARCH_TOPSTITCH)
			lblTitle->setText("Topstitch Search Criteria");
		else if (m_SearchMode == SEARCH_BUTTONHEAD)
			lblTitle->setText("Buttonhead Search Criteria");
		else if (m_SearchMode == SEARCH_BUTTONHOLE)
			lblTitle->setText("Buttonhole Search Criteria");
		else if (m_SearchMode == SEARCH_ZIPPER_PULLER)
			lblTitle->setText("Zipper Puller Search Criteria");
		else if (m_SearchMode == SEARCH_ZIPPER_SLIDER)
			lblTitle->setText("Zipper Slider Search Criteria");
		else if (m_SearchMode == SEARCH_ZIPPER_TOP_STOPPER)
			lblTitle->setText("Zipper Top Stopper Search Criteria");
		else if (m_SearchMode == SEARCH_ZIPPER_BOTTOM_STOPPER)
			lblTitle->setText("Zipper Bottom Stopper Search Criteria");


		generateUI();
	}

	void APISearchDialogSample::SetSearchModeByItemId(const string & itemId)
	{
		QString qItemId = QString::fromStdString(itemId);

		if (qItemId.compare(ITEM_ID_PATTERN) == 0)
			SetSearchMode(CLOAPISample::SEARCH_PATTERN);
		else if (qItemId.compare(ITEM_ID_TRIM) == 0)
			SetSearchMode(CLOAPISample::SEARCH_TRIM);
		else if (qItemId.compare(ITEM_ID_FABRIC) == 0)
			SetSearchMode(CLOAPISample::SEARCH_FABRIC);
		else if (qItemId.compare(ITEM_ID_PROJECT) == 0)
			SetSearchMode(CLOAPISample::SEARCH_PROJECT);
		else if (qItemId.compare(ITEM_ID_AVATAR) == 0)
			SetSearchMode(CLOAPISample::SEARCH_AVATAR);
		else if (qItemId.compare(ITEM_ID_GARMENT) == 0)
			SetSearchMode(CLOAPISample::SEARCH_GARMENT);
		else if (qItemId.compare(ITEM_ID_TOPSTITCH) == 0)
			SetSearchMode(CLOAPISample::SEARCH_TOPSTITCH);
		else if (qItemId.compare(ITEM_ID_BUTTONHEAD) == 0)
			SetSearchMode(CLOAPISample::SEARCH_BUTTONHEAD);
		else if (qItemId.compare(ITEM_ID_BUTTONHOLE) == 0)
			SetSearchMode(CLOAPISample::SEARCH_BUTTONHOLE);
		else if (qItemId.compare(ITEM_ID_ZIPPER_PULLER) == 0)
			SetSearchMode(CLOAPISample::SEARCH_ZIPPER_PULLER);
		else if (qItemId.compare(ITEM_ID_ZIPPER_SLIDER) == 0)
			SetSearchMode(CLOAPISample::SEARCH_ZIPPER_SLIDER);
		else if (qItemId.compare(ITEM_ID_ZIPPER_TOP_STOPPER) == 0)
			SetSearchMode(CLOAPISample::SEARCH_ZIPPER_TOP_STOPPER);
		else if (qItemId.compare(ITEM_ID_ZIPPER_BOTTOM_STOPPER) == 0)
			SetSearchMode(CLOAPISample::SEARCH_ZIPPER_BOTTOM_STOPPER);
		else
			SetSearchMode(CLOAPISample::SEARCH_NONE);
	}

	string APISearchDialogSample::GetSearchResult()
	{
		return m_ResultItem.toStdString();
	}

	void APISearchDialogSample::OnAccept()
	{		
		calculateSampleSearchResult();
		accept();
	}

	void APISearchDialogSample::OnReject()
	{
		reject();
	}

	void APISearchDialogSample::OnServiceDivisionComboBox(int _index)
	{
		cbBrand->clear();
		cbBrand->addItems(m_BrandListPerServiceDivision[cbServiceDivision->currentText()]);
	}

	void APISearchDialogSample::generateUI()
	{
		generateHouseOfBrandListUI();
		generateBrandListUI();
		generateProductTypeListUI();
		generateSeasonCreatedListUI();

		setComboBoxLists();
	}

	void APISearchDialogSample::generateHouseOfBrandListUI()
	{
		m_ServiceDivisionList.clear();
		m_ServiceDivisionList << SD_ALL << SD_CLO << SD_MD << SD_NXT << SD_FITTING << SD_OVERSEAS;
	}

	void APISearchDialogSample::generateBrandListUI()
	{
		m_BrandListPerServiceDivision.clear();

		if (m_ServiceDivisionList.empty())
			return;

		if (m_ServiceDivisionList.contains(SD_CLO))
			m_BrandListPerServiceDivision[SD_CLO] = QStringList() << BR_ALL << BR_CLO_BASIC << BR_CLO_LUXURY << BR_CLO_MENS \
			<< BR_CLO_WOMENS << BR_CLO_KIDS_BOYS << BR_CLO_KIDS_GIRLS << BR_CLO_SPORTS;

		if (m_ServiceDivisionList.contains(SD_MD))
			m_BrandListPerServiceDivision[SD_MD] = QStringList() << BR_ALL << BR_MD_GAME << BR_MD_ANIMATION << BR_MD_TV_SHOW << BR_MD_MOVIEW << BR_MD_CG_STUDIO;

		if (m_ServiceDivisionList.contains(SD_NXT))
			m_BrandListPerServiceDivision[SD_NXT] = QStringList() << BR_ALL << BR_CLOSET_WEB << BR_CLOSET_MOBILE;

		if (m_ServiceDivisionList.contains(SD_FITTING))
			m_BrandListPerServiceDivision[SD_FITTING] = QStringList() << BR_ALL << BR_ONLINE << BR_C_MIRROR;

		if (m_ServiceDivisionList.contains(SD_OVERSEAS))
			m_BrandListPerServiceDivision[SD_OVERSEAS] = QStringList() << BR_ALL << BR_OVERSEAS_CASUALS << BR_OVERSEAS_3D;

		if (m_ServiceDivisionList.contains(SD_ALL))
		{
			m_BrandListPerServiceDivision[SD_ALL] = QStringList() << BR_ALL;

			if (m_ServiceDivisionList.contains(SD_CLO) && m_BrandListPerServiceDivision[SD_CLO].size()>0)
			{
				for(int i=1;i< m_BrandListPerServiceDivision[SD_CLO].size();++i)
					m_BrandListPerServiceDivision[SD_ALL] << m_BrandListPerServiceDivision[SD_CLO][i];
			}

			if (m_ServiceDivisionList.contains(SD_MD) && m_BrandListPerServiceDivision[SD_MD].size() > 0)
			{
				for (int i = 1; i < m_BrandListPerServiceDivision[SD_MD].size(); ++i)
					m_BrandListPerServiceDivision[SD_ALL] << m_BrandListPerServiceDivision[SD_MD][i];
			}

			if (m_ServiceDivisionList.contains(SD_NXT) && m_BrandListPerServiceDivision[SD_NXT].size() > 0)
			{
				for (int i = 1; i < m_BrandListPerServiceDivision[SD_NXT].size(); ++i)
					m_BrandListPerServiceDivision[SD_ALL] << m_BrandListPerServiceDivision[SD_NXT][i];
			}
		
			if (m_ServiceDivisionList.contains(SD_FITTING) && m_BrandListPerServiceDivision[SD_FITTING].size() > 0)
			{
				for (int i = 1; i < m_BrandListPerServiceDivision[SD_FITTING].size(); ++i)
					m_BrandListPerServiceDivision[SD_ALL] << m_BrandListPerServiceDivision[SD_FITTING][i];
			}

			if (m_ServiceDivisionList.contains(SD_OVERSEAS) && m_BrandListPerServiceDivision[SD_OVERSEAS].size() > 0)
			{
				for (int i = 1; i < m_BrandListPerServiceDivision[SD_OVERSEAS].size(); ++i)
					m_BrandListPerServiceDivision[SD_ALL] << m_BrandListPerServiceDivision[SD_OVERSEAS][i];
			}
		}			
	}

	void APISearchDialogSample::generateProductTypeListUI()
	{
		m_ProductTypeList.clear();

		if (m_SearchMode == SEARCH_PATTERN || m_SearchMode == SEARCH_PROJECT || m_SearchMode == SEARCH_GARMENT)
		{
			m_ProductTypeList << PT_ALL << PT_BERMUDA << PT_BLAZER << PT_BLOUSE << PT_BOXER << PT_BOXER_SHORT << PT_CAMISOLES \
				<< PT_CAPRIS << PT_CAPS << PT_CARGO << PT_CIRCULAR_KNIT_DRESS << PT_CIRCULAR_KNIT_TOP << PT_DENIMS << PT_DRESSES \
				<< PT_FULL_SLEEVE_SHIRTS << PT_FLAT_KNIT_DRESS << PT_FLAT_KNIT_TOP << PT_HALF_SLEEVE_SHIRTS << PT_JACKETS \
				<< PT_JEANS << PT_JODHPURI << PT_JOGGERS << PT_JUMPSUITS << PT_KNIT_BOTTOM << PT_KURTA << PT_LEGGINS \
				<< PT_NEHRU_JACKET << PT_PANTS << PT_PLEATED_TROUSERS << PT_PYJAMA << PT_SHIRT << PT_SHORTS << PT_SKIRT \
				<< PT_SUITS << PT_SWEATER << PT_SWEAT_SHIRT << PT_SUIT_2T << PT_3PC_SUIT_2T << PT_TANK_TOP << PT_TANK \
				<< PT_TRACK_PANT << PT_TROUSERS << PT_T_SHIRTS_CREW_NECK << PT_T_SHIRTS_POLO << PT_T_SHIRTS_V_NECK << PT_T_SHIRTS_WITH_POCKET \
				<< PT_T_SHIRTS_WITH_SHORT << PT_T_SHIRTS_WITH_JOGGER << PT_TUNICS << PT_VM_MY_FIT_SHIRTS << PT_WAIST_COAT \
				<< PT_BODY_SUIT << PT_ROMPER ;
		}
		else if (m_SearchMode == SEARCH_FABRIC)
		{
			m_ProductTypeList << PT_ALL << PT_FABRIC_MATTE << PT_FABRIC_SHINY << PT_FABRIC_SILK_SATIN << PT_FABRIC_VELVET;
		}
		else if (m_SearchMode == SEARCH_TRIM)
		{

		}
		else if (m_SearchMode == SEARCH_TOPSTITCH)
		{

		}
		else if (m_SearchMode == SEARCH_BUTTONHEAD)
		{

		}
		else if (m_SearchMode == SEARCH_BUTTONHOLE)
		{

		}
		else if (m_SearchMode == SEARCH_ZIPPER_PULLER)
		{

		}
		else if (m_SearchMode == SEARCH_ZIPPER_SLIDER)
		{

		}
		else if (m_SearchMode == SEARCH_ZIPPER_TOP_STOPPER)
		{

		}
		else if (m_SearchMode == SEARCH_ZIPPER_BOTTOM_STOPPER)
		{

		}
		else
		{

		}
	}

	void APISearchDialogSample::generateSeasonCreatedListUI()
	{
		m_SeasonCreatedList.clear();

		m_SeasonCreatedList << SEASON_ALL << SEASON_INSTITUTIONAL << SEASON_PEM_CORE << SEASON_REGULAR_SEASON_CORE << SEASON_AUTUMN_WINTER_19 \
			<< SEASON_SPRING_SUMMER_20 << SEASON_AUTUMN_WINTER_20;
	}

	void APISearchDialogSample::setComboBoxLists()
	{	
		// Service Division
		cbServiceDivision->clear();
		cbServiceDivision->hide();
		lblServiceDivision->hide();
		
		if (!m_ServiceDivisionList.empty())
		{
			cbServiceDivision->addItems(m_ServiceDivisionList);
			cbServiceDivision->show();
			lblServiceDivision->show();
		}
			
		// Brand
		cbBrand->clear();
		cbBrand->hide();
		lblBrand->hide();

		if (!m_BrandListPerServiceDivision[SD_ALL].empty())
		{
			cbBrand->addItems(m_BrandListPerServiceDivision[SD_ALL]);
			cbBrand->show();
			lblBrand->show();
		}

		// Product type
		cbProductType->clear();
		cbProductType->hide();
		lblProductType->hide();

		if (!m_ProductTypeList.empty())
		{
			cbProductType->addItems(m_ProductTypeList);
			cbProductType->show();
			lblProductType->show();
		}

		// Season
		cbSeasonCreated->clear();
		cbSeasonCreated->hide();
		lblSeasonCreated->hide();

		if (!m_SeasonCreatedList.empty())
		{
			cbSeasonCreated->addItems(m_SeasonCreatedList);
			cbSeasonCreated->show();
			lblSeasonCreated->show();
		}
	}

	void APISearchDialogSample::calculateSampleSearchResult()
	{
		vector <LibraryAPIItem> searchItemList = GetSearchItemList();

		if (searchItemList.empty())
		{
			m_ResultItem = "";
			return;
		}

		m_ResultItem = generateJSONFromLibraryAPIItemList(searchItemList);
	}

	vector<LibraryAPIItem> APISearchDialogSample::GetSearchItemList()
	{
		vector<LibraryAPIItem> result;

		QString searchText = lineEdit_ItemName->text();
		
		QVariantMap searchKeyValues;

		if (m_SearchMode == SEARCH_PATTERN)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PATTERN;
		else if (m_SearchMode == SEARCH_TRIM)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TRIM;
		else if (m_SearchMode == SEARCH_FABRIC)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_FABRIC;
		else if (m_SearchMode == SEARCH_PROJECT)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PROJECT;
		else if (m_SearchMode == SEARCH_AVATAR)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_AVATAR;
		else if (m_SearchMode == SEARCH_GARMENT)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_GARMENT;
		else if (m_SearchMode == SEARCH_TOPSTITCH)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TOPSTITCH;
		else if (m_SearchMode == SEARCH_BUTTONHEAD)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_BUTTONHEAD;
		else if (m_SearchMode == SEARCH_BUTTONHOLE)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_BUTTONHOLE;
		else if (m_SearchMode == SEARCH_ZIPPER_PULLER)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_PULLER;
		else if (m_SearchMode == SEARCH_ZIPPER_SLIDER)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		else if (m_SearchMode == SEARCH_ZIPPER_TOP_STOPPER)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_TOP_STOPPER;
		else if (m_SearchMode == SEARCH_ZIPPER_BOTTOM_STOPPER)
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_BOTTOM_STOPPER;
		else
			searchKeyValues[META_DATA_KEY_0_DATA_TYPE] = QString("All");

		searchKeyValues[META_DATA_KEY_1_SERVICE_DIVISION] = cbServiceDivision->currentText();
		searchKeyValues[META_DATA_KEY_2_BRAND] = cbBrand->currentText();
		searchKeyValues[META_DATA_KEY_3_PRODUCT_TYPE] = cbProductType->currentText();
		searchKeyValues[META_DATA_KEY_4_SEASON_CREATED] = cbSeasonCreated->currentText();
		
		if (API_STORAGE)
			result = API_STORAGE->GetSearchList(searchText, searchKeyValues);

		return result;
	}
}