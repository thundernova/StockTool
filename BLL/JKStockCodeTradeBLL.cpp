#include "stdafx.h"
#include "JKStockCodeTradeBLL.h"
#include "JKStockCodeTradeItemBLL.h"
#include "Model/JKStockCodeTradeModel.h"
#include "JKStockTradeUtil.h"
#include "JKBLLContainer.h"
#include "JKBLLContext.h"


void JKStockCodeTradeBLL::setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice)
{
	ptrModel->type = (JKUInt)type;
	ptrModel->date = date;
	ptrModel->buyCount = count;
	ptrModel->buyPrice = buyPrice;
}

void JKStockCodeTradeBLL::sell(double sellPrice, size_t sellCount, size_t sellSumCount, JKString soldDate,
	float stampTax, float transfer, float commission)
{
	if (this->getCouldSellCount() < sellCount || sellCount <= 0 || sellSumCount < sellCount)
		return;

	JKStockTradeUtil stockTrackUtil(stampTax, transfer, commission);
	double expactEarning = stockTrackUtil.getExpactEarning(sellPrice, StockCodeTradeBLLConstRefPtr(this), sellCount);
	
	JKRef_Ptr<JKStockCodeTradeItemBLL> _refStockCodeTradeItem = NewBLL(JKStockCodeTradeItemBLL, JKStockCodeTradeItemModel, getOriginID());
	_refStockCodeTradeItem->setParams(sellPrice, sellCount, sellSumCount, stampTax, transfer, commission);
	_refStockCodeTradeItem->setsoldDate(soldDate);
	_refStockCodeTradeItem->setRealEarning(expactEarning);
	ptrModel->addStockCodeTradeItem(_refStockCodeTradeItem->getModel());

	this->updateType();
}

TradeType JKStockCodeTradeBLL::getType() 
{
	return (TradeType)ptrModel->type;
}
HoldStockType JKStockCodeTradeBLL::getHoldType()
{
	return (HoldStockType)ptrModel->holdType;
}
void JKStockCodeTradeBLL::setHoldType(HoldStockType type)
{
	ptrModel->holdType = (int)type;
}
void JKStockCodeTradeBLL::updateType()
{
	int couldSellCount = this->getCouldSellCount();

	if (couldSellCount == 0)
	{
		ptrModel->type = (int)TradeType::SELL;
	}
	else if (couldSellCount == this->getCount())
	{
		ptrModel->type = (int)TradeType::BUY;
	}
	else
	{
		ptrModel->type = (int)TradeType::PART;
	}
}

JKString JKStockCodeTradeBLL::getDate()
{
	return ptrModel->date;
}
JKUInt64 JKStockCodeTradeBLL::getCount()
{
	return ptrModel->buyCount;
}
JKUInt64 JKStockCodeTradeBLL::getSoldCount()
{
	JKUInt64 count = 0;
	for (auto &var : ptrModel->vecSellItem)
	{
		count += var->sellCount;
	}
	return count;
}
JKUInt64 JKStockCodeTradeBLL::getCouldSellCount()
{
	JKUInt64 count = 0;
	for (auto &var : ptrModel->vecSellItem)
	{
		count += var->sellCount;
	}
	return ptrModel->buyCount - count;
}

JKString JKStockCodeTradeBLL::getSoldDate()
{
	if (ptrModel->vecSellItem.size() != 0)
	{
		return ptrModel->vecSellItem[0]->soldDate;
	}
	return "";
}

double JKStockCodeTradeBLL::getBuyPrice()
{
	return ptrModel->buyPrice;
}

double JKStockCodeTradeBLL::getSellPrice()
{
	double total = .0f;
	JKUInt64 count = 0;
	for (auto &var : ptrModel->vecSellItem)
	{
		total += var->sellCount* var->sellPrice;
		count += var->sellCount;
	}
	return total / count;
}

double JKStockCodeTradeBLL::getSoldAmount()
{
	double total = .0f;
	for (auto &var : ptrModel->vecSellItem)
	{
		total += var->sellCount* var->sellPrice;
	}
	return total;
}

double JKStockCodeTradeBLL::getBuyAmount()
{
	return ptrModel->buyPrice * ptrModel->buyCount;
}

double JKStockCodeTradeBLL::getRealEarning()
{
	double realEarning = 0;
	for (auto &var : ptrModel->vecSellItem)
	{
		realEarning += var->realEarning;
	}
	return realEarning;
}

int JKStockCodeTradeBLL::getTradeItemsCount()
{
	return ptrModel->vecSellItem.size();
}

StockCodeTradeItemBLLPtr JKStockCodeTradeBLL::getTradeItem(int index)
{
	if (index >= ptrModel->vecSellItem.size())
		return StockCodeTradeItemBLLPtr();

	return LoadBLL(JKStockCodeTradeItemBLL, JKStockCodeTradeItemModel, ptrModel->vecSellItem[index].get_id(), getOriginID());
}

void JKStockCodeTradeBLL::getTradeItems(std::vector<JKRef_Ptr<JKStockCodeTradeItemBLL>>& vecTradeItems)
{
	for (auto &var : ptrModel->vecSellItem)
	{
		JKRef_Ptr<JKStockCodeTradeItemBLL> _refStockCodeTradeItem = LoadBLL(JKStockCodeTradeItemBLL, JKStockCodeTradeItemModel, var.get_id(), getOriginID());
		vecTradeItems.push_back(_refStockCodeTradeItem);
	}
}

void JKStockCodeTradeBLL::upgradeDataVersion(int dataVersion)
{
	/** 升级说明
	 *	之前版本卖出没有创建JKStockCodeTradeItemModel，
	 *	后来添加上之后，需要对之前的工程创建JKStockCodeTradeItem
	 */
	//if (dataVersion < 1)
	//{
	//	if (this->getType() == TradeType::SELL)
	//	{
	//		JKRef_Ptr<JKProjectBLL> _refProject = BLLContext.getProject();
	//		float stampTax = _refProject->getStampTax();
	//		float transfer = _refProject->getTransfer();
	//		float commission = _refProject->getCommission();

	//		double sellPrice = this->getSellPrice();
	//		size_t count = this->getCount();

	//		this->sell(sellPrice, count, count, stampTax, transfer, commission);
	//	}
	//}
}

void JKStockCodeTradeBLL::save()
{
	//SaveBean(JKStockCodeSettingBLL, JKStockCodeSettingModel, ptrModel->codeSetting);
	for (int i = 0; i < ptrModel->vecSellItem.size(); ++i)
	{
		SaveBean(JKStockCodeTradeItemBLL, JKStockCodeTradeItemModel, ptrModel->vecSellItem[i]);
	}
}

void JKStockCodeTradeBLL::destroy()
{
	//DestroyBean(JKStockCodeSettingBLL, JKStockCodeSettingModel, ptrModel->codeSetting);

	for (int i = 0; i < ptrModel->vecSellItem.size(); ++i)
	{
		DestroyBean(JKStockCodeTradeItemBLL, JKStockCodeTradeItemModel, ptrModel->vecSellItem[i]);
	}
}
