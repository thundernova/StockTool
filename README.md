
使用到开源库
Hiberlite https://github.com/paulftw/hiberlite.git
Qt库
JKCore https://github.com/Mr-WangJin/jk_cpp.git

# StockTool
炒股辅助工具


# 炒股辅助工具 功能点 #

1. 记录用户输入的股票数据，包括股票号，买入数据量和价钱，卖出数量和价钱， 税收点，设置买入次数，买入偏差百分比。
2. 第一次开始买入股票时，，计算出最低百分比，
3. 输入买入数量和价钱，计算出总投资需要多少。最差损失。
4. 输入目标价位，计算出能盈利多少。
5. 输入当前股价，判断是否买入或者卖出。 


  
工程数据格式
{
    "name": "",
    "createDate": "",
    "stockCode": [
        {
            "Setting": {
                "taxPercent": "",
                "offsetPercent": "",
                "targetPrice": ""
            },
            "code": "",
            "name": "",
            "latestPrice": "",
            "trade": [
                {
                    "type": "",
                    "date": "",
                    "tradingPrice": "",
                    "tradingCount": ""
                }
            ]
        }
    ]
}