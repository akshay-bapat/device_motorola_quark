Device configuration for Moto MAXX (Quark)
==============================

Copyright 2016 - 2017 - 2018 The BHB27 Project :stuck_out_tongue_closed_eyes:

Copyright 2016 - The CyanogenMod Project

Copyright 2017 - 2018 The LineageOS Project

**I use this tree to build Oreo on lineage-15.x and lineage-15.x base source**

:exclamation: This tree is current **WIP** AKA doesn't work :exclamation:

How to build this...
The below may be out of data check XDA thread to make shore the described tree below are the one be used today...
Use the below in /home/user/source/.repo/local_manifests/roomservice.xml

	<?xml version="1.0" encoding="UTF-8"?>
	<manifest>

	  <!-- Common qcom to build dtb and some etc lib-->
	  <project name="LineageOS/android_device_qcom_common" path="device/qcom/common" remote="github" revision="lineage-15.0" />
	  <!-- Strings for CMActions-->
	  <project name="LineageOS/android_packages_resources_devicesettings" path="packages/resources/devicesettings" remote="github" revision="lineage-15.0" />
	  <!-- Device/kernel/vendor-->
	  <project name="bhb27/device_motorola_quark" path="device/motorola/quark" remote="github" revision="O" />
	  <project name="bhb27/BHB27Kernel" path="kernel/motorola/apq8084" remote="github" revision="O" />
	  <project name="bhb27/proprietary_vendor_motorola" path="vendor/motorola" remote="github" revision="O" />

	</manifest>

The Motorola Moto Maxx (codenamed _"quark"_) is a high-end smartphone from Motorola mobility.
It was announced on November 2014.

Basic   | Spec Sheet
-------:|:-------------------------
CPU     | Quad-core 2.7 GHz Krait 450
Chipset | Qualcomm Snapdragon 805
GPU     | Adreno 420
Memory  | 3GB RAM
Shipped Android Version | 4.4.4
Storage | 64 GB
MicroSD | No
Battery | Non-removable Li-Po 3900 mAh battery
Display | 1440 x 2560 pixels, 5.2 inches (~565 ppi pixel density)
Camera  | 21 MP (5248 x 3936), autofocus, dual-LED flash


![MOTO MAXX](https://raw.githubusercontent.com/bhb27/scripts/master/etc/images/moto-maxx.png "MOTO MAXX")
