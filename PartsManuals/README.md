# ECGMonitor
Project

# Parts Mamuals Folder
For storing particular chip's manual/datasheet

# TI Solution Web-book
## ECG Monitor Solution
Link: [http://www.ti.com.cn/solution/cn/ecg_electrocardiogram](http://www.ti.com.cn/solution/cn/ecg_electrocardiogram)
### Important Parts
+ ADS1298 (Complex analog front, not going to use, too expensive)
+ ADS1293 (This stuff is what we are going to use)
+ MSP430FG439(Recommender), MSP430F149(easy to buy) 

# Manual List

## AFE4400 Integrated Analog Pulse Oximeters
datasheet: [AFE4400.pdf](https://github.com/celelion/ECGMonitor/blob/master/PartsManuals/AFE4400.pdf)

web:[TI Link](http://www.ti.com.cn/product/cn/afe4400)

buy:[DigiKey Link](https://www.digikey.com.cn/product-detail/zh/texas-instruments/AFE4400RHAR/296-38860-1-ND/5142978)

example:[TI](http://www.ti.com.cn/tool/cn/TIDA-00311)

PDF:[tidu542](https://github.com/celelion/ECGMonitor/blob/master/PartsManuals/tidu542.pdf)

## ADS1293 Analog front-end chip
datasheet: [ads1293.pdf](https://github.com/celelion/ECGMonitor/blob/master/PartsManuals/ads1293.pdf)

web:[TI Link](http://www.ti.com.cn/product/cn/ADS1293/technicaldocuments)

buylink:[digikey](https://www.digikey.com.cn/product-detail/zh/texas-instruments/ADS1293CISQE-NOPB/296-35511-1-ND/3768438)

example:[TI Link](http://www.ti.com.cn/tool/cn/TIDA-00096)

PDF:[tidu195a.pdf](https://github.com/celelion/ECGMonitor/blob/master/PartsManuals/tidu195a.pdf)

## ADS1298 Analog front-end chip
ADS-1298-Analog Front-End for Biopotential Measurement Chip.pdf
website:[TI Link](http://www.ti.com.cn/product/cn/ads1298)

## MSP430FG439

link:[http://www.ti.com.cn/product/cn/MSP430FG439/toolssoftware](http://www.ti.com.cn/product/cn/MSP430FG439/toolssoftware)

## MSP430F149

link:[http://www.ti.com.cn/product/cn/MSP430F149/technicaldocuments](http://www.ti.com.cn/product/cn/MSP430F149/technicaldocuments)

# About ADS1293 Ctrl Regs
AFE_RES(0x13h<0x00h)-> 102.4-kHz clock freq for ADC