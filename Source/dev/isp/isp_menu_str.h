#ifndef __ISP_MENU_STR_H__
#define __ISP_MENU_STR_H__

//*************************************************************************************************
// Menu String table
//-------------------------------------------------------------------------------------------------
// *
// (\xa6 == ENTER)
//
//
//							ENGLISH 				CHANESS(중국어번체) 			CHANESS(SIMPLE)(중국어간체)		Japanese												Korean
/* !! - DON'T ERASE - !!*/
STRING1(,					_B,"");
STRING1(e,					_B,"\xa6");
STRING1(PERCENT,			_B,"%");
STRING5(NOT_USED,			_B,"NOT_USED",			_B,"NOT_USED",					_B,"NOT_USED",					_W,_P(270,271,18,272,14),								_B,_P(113,184,209));
STRING1(ENTER_d_RETURN,		_B,"ENTER - RETURN");
STRING1(MENU_IS_NOT_AVAILABLE,	_B,"MENU IS NOT AVAILABLE");
/* !! - DON'T ERASE - !!*/

STRING5(ON, 				_B,"ON",				_B,_P(100),						_B,_P(100),						_B,_P(26,194),											_B,_P(184,209));
STRING5(ONe,				_B,"ON\xa6",			_B,_P(100,0xa6),				_B,_P(100,0xa6),				_B,_P(26,194,166),										_B,_P(184,209,166));
STRING5(ONp,				_B,"ON\xa0",			_B,_P(100,0xa0),				_B,_P(100,0xa0),				_B,_P(26,194,160),										_B,_P(184,209,160));
STRING5(OFF,				_B,"OFF",				_W,_P(259),						_W,_P(259),						_B,_P(26,122),											_B,_P(113,184,209));
STRING5(LOW,				_B,"LOW",				_B,_P(208), 					_B,_P(208),						_B,_P(197),												_B,_P(26,212));
STRING5(MIDDLE, 			_B,"MIDDLE",			_B,_P(222), 					_B,_P(222),						_B,_P(198),												_B,_P(229,15));
STRING5(HIGH,				_B,"HIGH",				_B,_P(28),						_B,_P(28),						_B,_P(199),												_B,_P(29,212));

STRING5(MENU,				_B,"MENU",				_B,_P(14,15),					_B,_P(14,15),					_B,_P(132,118,186,195),									_B,_P(110,30));
//STRING5(LENS,				_B,"\x04 LENS",			_B,_P(4,' ',16,17), 			_B,_P(4,' ',16,17),				_B,_P(4,' ',192,194,109),								_B,_P(4,' ',103,230));
STRING5(EXPOSURE,			_B,"\x05 EXPOSURE",		_B,_P(5,' ',24, 20),			_B,_P(5,' ',24, 20),			_B,_P(5,' ',212,207),									_B,_P(5,' ',28,236,225,223));
STRING5(BACKLIGHT,			_B,"\x06 BACKLIGHT",	_B,_P(6,' ',25,20,26,27),		_B,_P(6,' ',25,20,26,27),		_B,_P(6,' ',211,207,216,217),							_B,_P(6,' ',204,21));
STRING5(DAYnNIGHT,			_B,"\x07 DAY&NIGHT",	_B,_P(7,' ',204,'&',186),		_B,_P(7,' ',204,'&',186),		_B,_P(7,' ',115,24,'&',117,24,112),						_B,_P(7,' ',227,201,15,112,93,188,223));
STRING5(DAY,				_B,"DAY",				_B,_P(204),						_B,_P(204),						_B,_P(115,24),											_B,_P(227,15));
STRING5(NIGHT,				_B,"NIGHT",				_B,_P(186),						_B,_P(186),						_B,_P(117,24,112),										_B,_P(201,15));
//STRING5(WHITE_BAL,		_B,"\x08 WHITE BAL",	_B,_P(8,' ',59,241,245),		_B,_P(8,' ',59,241,245),		_B,_P(8,' ',123,23,24,112,124,189,194,106),				_B,_P(8,' ',252,213,246,120,101,194));
STRING5(COLORm,				_B,"\x08 COLOR",		_B,_P(8,' ',228,125), 			_B,_P(8,' ',228,125),			_B,_P(8,' ',30,189,195),								_B,_P(8,' ',239,100));
STRING5(DNR,				_B,"\x09 DNR",			_B,_P(9,' ',253,220),			_B,_P(9,' ',253,220),			_B,_P(9,' ',115,108,110,191,120,24,109,245,246),		_B,_P(9,' ', 28,213,230,224,18));
//STRING5(DNR,				_B,"DNR",				_B,_P(253,220),					_B,_P(253,220),					_B,_P(115,108,110,191,120,24,109,245,246),				_B,_P(28,213,230,224,18));
STRING5(IMAGE,				_B,"\x0a IMAGE",		_B,_P(10,' ',190,123),			_B,_P(10,' ',190,123),			_B,_P(10,' ',24,132,195,108),							_B,_P(10,' ',205,185,138,223));
STRING5(MOTION, 			_B,"\x0b MOTION",		_B,_P(11,' ',195,201,213,231),	_B,_P(11,' ',195,201,213,231),	_B,_P(11 ,' ',184,195,105,187,194),						_B,_P(11 ,' ',210,232,215,16,231));
//STRING1(DIS,				_B,"\x03 DIS");
STRING5(SYSTEM, 			_B,"\x0c SYSTEM", 		_B,_P(12,' ',218,236),			_B,_P(12,' ',218,236),			_B,_P(12,' ',105,106,111,131),							_B,_P(12,' ',195,194,243));
STRING5(EXIT,				_B,"\x0d EXIT",			_B,_P(13,' ',107,100),			_B,_P(13,' ',107,100),			_B,_P(13,' ',251,252),									_B,_P(13,' ',226,107));
STRING5(nEXIT,				_B,"EXIT",				_B,_P(107,100),					_B,_P(107,100),					_B,_P(251,252),											_B,_P(226,107));
STRING5(RETURN, 			_B,"RETURN",			_B,_P(111,249), 				_B,_P(111,249),					_B,_P(202,22),											_B,_P(139,22));

STRING5(ALC,				_B,"ALC",				_B,_P(18,19,20,21), 			_B,_P(18,19,20,21),				_B,_P(205,206,207,208,209,210),							_B,_P(225,108,17,112,93));
STRING5(ELC,				_B,"ELC",				_B,_P(22,23,20,21), 			_B,_P(22,23,20,21),				_B,_P(203,204,207,208,209,210),							_B,_P(191,241,112,93));
//STRING5(ALCe,				_B,"ALC\xa6",			_B,_P(18,19,20,21,166), 		_B,_P(18,19,20,21,166),			_B,_P(205,206,207,208,209,210,166),						_B,_P(225,108,17,112,93,166));
//STRING5(ELCe,				_B,"ELC\xa6",			_B,_P(22,23,20,21,166), 		_B,_P(22,23,20,21,166),			_B,_P(203,204,207,208,209,210,166),						_B,_P(191,241,112,93,166));

STRING5(NORMAL, 			_B,"NORMAL",			_W,_P(238,308),					_W,_P(238,308),					_B,_P(213,214),											_B,_P(138,244));
STRING5(DEBLUR, 			_B,"DEBLUR",			_B,"DEBLUR",					_B,"DEBLUR",					_B,_P(19,20,16,216,217),								_B,_P(218,185,224,18));
STRING5(INDOOR, 			_B,"INDOOR",			_B,_P(184,35),					_B,_P(184,35),					_W,_P(285,219),											_B,_P(197,27));
STRING5(OUTDOOR,			_B,"OUTDOOR",			_B,_P(184,193), 				_B,_P(184,193),					_B,_P(218,230),											_B,_P(197,208));

STRING5(BRIGHTNESS, 		_B,"BRIGHTNESS",		_B,_P(29,99),					_B,_P(29,99),					_B,_P(196,22,17),										_B,_P(117,24,225,223));
STRING5(SHUTTER,			_B,"SHUTTER",			_B,_P(211,205,234,110), 		_B,_P(211,205,234,110),			_B,_P(105,185,113,110,195,106,127,195,116),				_B,_P(191,241));
STRING5(AUTO,				_B,"AUTO",				_B,_P(18,19),					_B,_P(18,19),					_B,_P(26,195,112),										_B,_P(217,92));
STRING5(AUTOe,				_B,"AUTO\xa6",			_B,_P(18,19,166),				_B,_P(18,19,166),				_B,_P(26,195,112,166),									_B,_P(217,92,166));
STRING5(AUTO_MODE,			_B,"AUTO MODE",			_B,_P(18,19,' ',109,140),		_B,_P(18,19,' ',109,140),		_B,_P(26,195,112,' ',184,195,116),						_B,_P(217,92,' ',112,93));
STRING5(MANUAL, 			_B,"MANUAL",			_B,_P(131,19),					_B,_P(131,19),					_B,_P(129,118,186,23,191),								_B,_P(193,92));
STRING5(MANUALe,			_B,"MANUAL\xa6",		_B,_P(131,19,166),				_B,_P(131,19,166),				_B,_P(129,118,186,23,191,166),							_B,_P(193,92,166));
STRING5(FLICKER, 			_B,"FLICKER",			_W,_P(273,271),					_W,_P(273,271),					_B,_P(122,190,113,30,195),								_W,_P(284,108,280));
STRING5(SPEED,				_B,"SPEED", 			_B,_P(22,23,234,110),			_B,_P(22,23,234,110),			_B,_P(106,127,195,116),									_B,_P(192,91));
//STRING1(SHUT_25,			_B,"1/25");
//STRING1(SHUT_30,			_B,"1/30");
//STRING1(SHUT_50,			_B,"1/50");
//STRING1(SHUT_60,			_B,"1/60");
//STRING1(SHUT_100,			_B,"1/100");
//STRING1(SHUT_120,			_B,"1/120");
//STRING1(SHUT_250,			_B,"1/250");
STRING5(SHUTTER_AUTO,		_B,"SHUTTER-AUTO",		_B,_P(211,205,234,110,'-',18,19), 	_B,_P(211,205,234,110,'-',18,19),	_B,_P(105,185,113,110,195,106,127,195,116,'-',26,195,112),			_B,_P(191,241,'-',217,92));
STRING5(SHUTTER_MANUAL,		_B,"SHUTTER-MANUAL",	_B,_P(211,205,234,110,'-',131,19), 	_B,_P(211,205,234,110,'-',131,19),	_B,_P(105,185,113,110,195,106,127,195,116,'-',129,118,186,23,191),	_B,_P(191,241,'-',193,92));

//STRING1(SHUT_500,			_B,"1/500");
//STRING1(SHUT_1000,			_B,"1/1000");
//STRING1(SHUT_2000,			_B,"1/2000");
//STRING1(SHUT_4000,			_B,"1/4000");
//STRING1(SHUT_8000,			_B,"1/8000");
//STRING1(SHUT_15000,			_B,"1/15000");
//STRING1(SHUT_30000,			_B,"1/30000");
//STRING1(SHUT_60000,			_B,"1/60000");

//STRING1(SHUT_200,			_B,"1/200");
//STRING1(SHUT_400, 			_B,"1/400");
//STRING1(SHUT_800,			_B,"1/800");
//STRING1(SHUT_1600,			_B,"1/1600");
//STRING1(SHUT_3200,			_B,"1/3200");
//STRING1(SHUT_6400,			_B,"1/6400");
//STRING1(SHUT_12800,			_B,"1/12800");
//STRING1(SHUT_25600, 		_B,"1/25600");
//STRING1(SHUT_51200, 		_B,"1/51200");

STRING5(SENS_UP,			_B,"SENS-UP",			_B,_P(216,209,105), 			_B,_P(216,209,105),				_B,_P(107,194,106,23,113,128),							_W,_P(267,194,270));
STRING1(X2, 				_B,"X2");
STRING1(X4, 				_B,"X4");
STRING1(X8,					_B,"X8");
STRING1(X16,				_B,"X16");
STRING1(X32,				_B,"X32");
STRING1(X64,				_B,"X64");
//STRING5(AGC,				_B,"AGC",				_B,_P(18,19,224,202),			_B,_P(18,19,224,202),			_B,_P(26,195,112,103,24,194,100,194,112,193,195,191),	_B,_P(19,214));

STRING1(SENSOR_GAIN,		_B,"SENSOR GAIN");
STRING1(ISP_GAIN,			_B,"ISP GAIN");
STRING1(EXTRA_GAIN,			_B,"EXTRA GAIN");
STRING1(AE_CURRENT,			_B,"AE CURRENT");
STRING1(AE_POSITION,		_B,"AE POSITION");

STRING5(HLCe,				_B,"HLC\xa6",			_B,_P(28,29,30,42,166), 		_B,_P(28,29,30,42,166),			_B,_P(121,24,189,24,112,216,217,166),					_W,_P(204,21,279,262,166));
STRING5(BLCe,				_B,"BLC\xa6",			_B,_P(25,20,118,124,166),		_B,_P(25,20,118,124,166),		_B,_P(124,113,31,189,24,112,216,217,166),				_B,_P(204,21,138,223,166));
STRING5(WDRe,				_B,"WDR\xa6",			_B,_P(242,19,235,166),			_B,_P(242,19,235,166),			_B,"WDR\xa6",											_B,_P(21,204,204,21,138,223,166));

STRING5(HLC,				_B,"HLC",				_B,_P(28,29,30,42), 			_B,_P(28,29,30,42),				_B,_P(121,24,189,24,112,216,217),						_W,_P(204,21,279,262));
STRING5(BLC,				_B,"BLC",				_B,_P(25,20,118,124),			_B,_P(25,20,118,124),			_B,_P(124,113,31,189,24,112,216,217),					_B,_P(204,21,138,223));
STRING5(WDR,				_B,"WDR",				_B,_P(242,19,235),				_B,_P(242,19,235),				_B,"WDR",												_B,_P(21,204,204, 21,138,223));

STRING5(LEVEL,				_B,"LEVEL", 			_B,_P(102,33),					_B,_P(102,33),					_B,_P(192,125,191),										_B,_P(102,137));
STRING5(COLOR,				_B,"COLOR", 			_B,_P(228,125), 				_B,_P(228,125),					_B,_P(30,189,195),										_B,_P(239,100));
STRING5(WHT,				_B,"WHT",				_W,_P(269),						_W,_P(269),						_W,_P(260),												_W,_P(285,269));
STRING5(YEL,				_B,"YEL",				_W,_P(293),						_W,_P(293),						_B,_P(24,25,193,195),									_W,_P(28,263));
STRING5(CYN,				_B,"CYN",				_W,_P(288),						_W,_P(288),						_B,_P(105,23,194),										_W,_P(272,186));
STRING5(GRN,				_B,"GRN",				_W,_P(266),						_W,_P(266),						_W,_P(261),												_W,_P(235,264));
STRING5(MAG,				_B,"MAG",				_W,_P(278),						_W,_P(278),						_W,_P(129,262,194,110),									_W,_P(217,286));
STRING5(RED,				_B,"RED",				_B,_P(247),						_B,_P(247),						_B,_P(200),												_W,_P(266,255));
STRING5(BLU,				_B,"BLU",				_W,_P(265),						_W,_P(265),						_B,_P(201),												_W,_P(283,263));
STRING5(BLK,				_B,"BLK",				_B,_P(158),						_B,_P(158),						_W,_P(259),												_W,_P(256,223));
STRING5(CUSTOMIZE,			_B,"CUSTOMIZE",			_W,_P(18,23,282),				_W,_P(18,23,282),				_B,_P(30,106,110,129,24,109),							_B,_P(184,209,217));
STRING5(HPOS,				_B,"H-POS", 			_B,_P(136,241,100,137), 		_B,_P(136,241,100,137),			_B,_P(220,221,224,225),									_B,_P(14,105,211,237));
STRING5(VPOS,				_B,"V-POS", 			_B,_P(130,225,100,137), 		_B,_P(130,225,100,137),			_B,_P(222,223,224,225),									_B,_P(189,105,211,237));
STRING5(HSIZE,				_B,"H-SIZE",			_B,_P(136,241,28,99),			_B,_P(136,241,28,99),			_B,_P(220,221,104,24,109),								_B,_P(14,105,240,24));
STRING5(VSIZE,				_B,"V-SIZE",			_B,_P(130,225,97,99),			_B,_P(130,225,97,99),			_B,_P(222,223,104,24,109),								_B,_P(189,105,240,24));
STRING5(AUTOext,			_B,"AUTOext",			_B,_P(18,19,'(',203,')'),		_B,_P(18,19,'(',203,')'),		_B,_P(26,195,112,'2'),									_B,_P(217, 92,253,219));
STRING5(PRESETp,			_B,"PRESET\xa0",		_B,_P(22,23,160),				_B,_P(22,23,160),				_B,_P(188,194,128,113,105,186,160),						_B,_P(250,108,190,160));
STRING5(PUSHING,			_B,"PUSHING",			_B,"PUSHING",					_B,"PUSHING",					_W,_P(307,308),											_W,_P(293,294));
STRING5(MODE,				_B,"MODE",				_B,_P(109,140),					_B,_P(109,140),					_B,_P(184,195,116),										_B,_P(112,93));
STRING5(WEIGHT, 			_B,"WEIGHT",			_B,_P(102,33),					_B,_P(102,33),					_B,_P(192,125,191),										_B,_P(14,229,237));
STRING5(RGAIN,				_B,"R-GAIN",			_B,_P('R','-',224,202), 		_B,_P('R','-',224,202),			_B,_P(200,192,125,191),									_B,_P(220,186));
STRING5(BGAIN,				_B,"B-GAIN",			_B,_P('B','-',224,202), 		_B,_P('B','-',224,202),			_B,_P(201,192,125,191),									_B,_P(234,186));
STRING5(SHARPNESS,			_B,"SHARPNESS", 		_B,_P(191,106),					_B,_P(191,106),					_B,_P(105,185,195,128,119,106),							_B,_P(187,111,91));
STRING5(GAMMA,				_B,"GAMMA", 			_B,"GAMMA", 					_B,"GAMMA",						_B,_P(101,194,129),										_B,_P(16,109));
STRING5(AUTO_GAMMA,			_B,"AUTO GAMMA", 		_B,"\x12\x13 GAMMA", 			_B,"\x12\x13 GAMMA",			_B,_P(26,195,112,' ',101,194,129),						_B,_P(217,92,' ',16,109));
STRING1(GAMMA_045,			_B,"0.45");
STRING1(GAMMA_050,			_B,"0.5");
STRING1(GAMMA_055,			_B,"0.55");
STRING1(GAMMA_060,			_B,"0.6");
STRING1(GAMMA_065,			_B,"0.65");
STRING1(GAMMA_070,			_B,"0.7");
STRING1(GAMMA_075,			_B,"0.75");

STRING1(R_GAIN,				_B,"R-GAIN");	// 2017419 - WHL : CBB TEST
STRING1(G_GAIN,				_B,"G-GAIN");
STRING1(B_GAIN,				_B,"B-GAIN");

STRING1(NEW,				_B,"NEW");
STRING1(OLD,				_B,"OLD");

STRING1(YEL_REDdGRN,		_B,"YEL RED-GRN");
STRING1(YEL_GAIN,			_B,"YEL GAIN");
STRING1(RED_YELdBLU,		_B,"RED YEL-BLU");
STRING1(RED_GAIN,			_B,"RED GAIN");
STRING1(BLU_GRNdRED,		_B,"BLU GRN-RED");
STRING1(BLU_GAIN,			_B,"BLU GAIN");
STRING1(GRN_BLUdYEL,		_B,"GRN BLU-YEL");
STRING1(GRN_GAIN,			_B,"GRN GAIN");

STRING1(HUEnCHROMA,			_B,"HUE&CHROMA");
STRING1(REDnBLUE,			_B,"RED&BLUE");
STRING1(BLUEnGREEN,			_B,"BLUE&GREEN");
STRING1(GREENnYELLOW,		_B,"GREEN&YELLOW");
STRING1(YELLOWnRED,			_B,"YELLOW&RED");
STRING1(YELLOWdBLUE,		_B,"YELLOW-BLUE");
STRING1(GREENdRED,			_B,"GREEN-RED");
STRING1(BLUEdYELLOW,		_B,"BLUE-YELLOW");
STRING1(REDdGREEN,			_B,"RED-GREEN");
STRING1(GAIN_RED,			_B,"GAIN RED");
STRING1(GAIN_BLUE,			_B,"GAIN BLUE");
STRING1(GAIN_GREEN,			_B,"GAIN GREEN");
STRING1(GAIN_YELLOW,		_B,"GAIN YELLOW");

//STRING5(COLORGAIN,			_B,"COLOR GAIN",		_B,_P(228,125,36,99),			_B,_P(228,125,36,99),			_B,_P(233,234),											_B,_P(233,91));
STRING5(MIRROR, 			_B,"MIRROR",			_B,_P(16,123),					_B,_P(16,123),					_B,_P(130,189,195),										_B,_P(193,247,116,221));
STRING5(FLIP,				_B,"FLIP",				_B,_P(115,210), 				_B,_P(115,210),					_B,_P(122,190,113,128),									_B,_P(193,232,116,221));
STRING5(DZOOM,				_B,"D-ZOOM",			_B,_P(132,206,112,94),			_B,_P(132,206,112,94),			_B,_P(115,108,110,191,109,195,131),						_B,_P(94,231,242,228));
STRING5(DWDR,				_B,"ACE",				_B,_P(132,195,97,19,235),		_B,_P(132,195,97,19,235),		_B,_P(100,194,112,189,106,112,216,217),					_B,_P(31,141,138,223));
STRING5(DEFOG,				_B,"DEFOG",				_W,_P(255,27,256,257),			_W,_P(255,27,256,257),			_W,_P(279,216,217),										_B,_P(199,17,224,18));
//STRING5(SHADING,			_B,"SHADING",			_B,_P(199,190,96,248),			_B,_P(199,190,96,248),			_B,_P(105,29,195,115,27,194,102,216,217),				_B,_P(103,230,212,205,138,223));
STRING5(PRIVACY,			_B,"PRIVACY",			_B,_P(198,122),					_B,_P(198,122),					_B,_P(128,189,24,124,105,195),							_B,_P(250,97,213,121,195));
//STRING3(BOX,				_B,"BOX",				_B,"BOX",						_B,"BOX");
//STRING3(POLYGON,			_B,"POLYGON",			_B,"POLYGON",					_B,"POLYGON");
//STRING3(POS0X,				_B,"POS0-X",			_B,"POS0-X",					_B,"POS0-X");
//STRING3(POS0Y,				_B,"POS0-Y",			_B,"POS0-Y",					_B,"POS0-Y");
//STRING3(POS1X,				_B,"POS1-X",			_B,"POS1-X",					_B,"POS1-X");
//STRING3(POS1Y,				_B,"POS1-Y",			_B,"POS1-Y",					_B,"POS1-Y");
//STRING3(POS2X,				_B,"POS2-X",			_B,"POS2-X",					_B,"POS2-X");
//STRING3(POS2Y,				_B,"POS2-Y",			_B,"POS2-Y",					_B,"POS2-Y");
//STRING3(POS3X,				_B,"POS3-X",			_B,"POS3-X",					_B,"POS3-X");
//STRING3(POS3Y,				_B,"POS3-Y",			_B,"POS3-Y",					_B,"POS3-Y");
STRING5(BnWe,				_B,"B&W\xa6",			_B,_P(158,59,166),				_B,_P(158,59,166),				_W,_P(260,259),								_B,_P(254,119,166));

STRING5(EXTERNe, 			_B,"EXTERN\xa6",		_B,_P(193,120,113,31,166),		_B,_P(193,120,113,31,166),		_B,_P(230,231,166),							_B,_P(208,140,216,104,166));
//
//STRING5(IR_LED, 			_B,"IR LED",			_B,_P(247,193,103), 			_B,_P(247,193,103),				_W,_P(200,230,263,264,265),					_W,_P(220,208,187,21,275));
STRING5(ANTI_SAT,			_B,"ANTI-SAT.", 		_W,_P(268,24), 					_W,_P(268,24),					_W,_P(266,267,268,269),						_B,_P(248,252,118,231));
//STRING5(AGC_THRES,			_B,"AGC THRES", 		_B,_P(108,215,233), 			_B,_P(108,215,233),				_B,_P('A','G','C',18,15,14,235),			_W,_P('A','G','C',' ',257,258,188,223));
//STRING5(AGC_MARGIN, 		_B,"AGC MARGIN",		_B,_P(194,226), 				_B,_P(194,226),					_B,_P('A','G','C',129,195,108,194),			_W,_P('A','G','C',' ',257,258,271,265));
//STRING5(DELAY,				_B,"DELAY", 			_B,_P(189,192), 				_B,_P(189,192),					_B,_P(226,227,236,237,238,241),				_W,_P(221,287,195,15,188,223));
STRING5(ZONENUM,			_B,"ZONE NUM",			_B,_P(187,188,240,246), 		_B,_P(187,188,240,246),			_B,_P(25,190,23,117,194,124),				_B,_P(205,204,122,251));
STRING5(ZONEDISP,			_B,"ZONE DISP", 		_B,_P(187,188,243,138), 		_B,_P(187,188,243,138),			_B,_P(25,190,23,247,248),					_B,_P(205,204,249,195));

STRING5(PRV_MASKMOD,		_B,"MASK MODE",			_W,_P(270,291,109,140),			_W,_P(270,291,109,140),			_B,_P(129,106,31,184,195,116),				_B,_P(109,194,240,112,93));	// 2015112 - WHL
STRING5(PRV_FORMAT,			_B,"FORMAT",			_B,_P(101,140),					_B,_P(101,140),					_B,_P(122,28,129,113,112),					_W,_P(288,259));
STRING5(PRV_FILL,			_B,"FILL",				_W,_P(283,290),					_W,_P(283,290),					_W,_P(239,280,281),							_B,_P(233,210));

STRING5(YLEVEL, 			_B,"Y LEVEL",			_B,_P(187,188, 29, 99), 				_B,_P(187,188, 29, 99),					_B,_P('Y',192,125,191),			_B,_P('Y',' ',102,137));
STRING5(CBLEVEL,			_B,"CB LEVEL",			_B,_P(187,188,125,228,'(','Y',')'),		_B,_P(187,188,125,228,'(','Y',')'),		_B,_P('B',192,125,191),			_B,_P('C','B',' ',102,137));
STRING5(CRLEVEL,			_B,"CR LEVEL",			_B,_P(187,188,125,228,'(','U',')'),		_B,_P(187,188,125,228,'(','U',')'),		_B,_P('R',192,125,191),			_B,_P('C','R',' ',102,137));
STRING5(TRANS,				_B,"TRANS", 			_W,_P(256,261,99), 						_W,_P(256,261,99),						_W,_P(282,196,234),				_B,_P(245,111,91));
//STRING5(COM,				_B,"COM.",				_B,_P(238,141), 						_B,_P(238,141),							_B,_P(213,215),					_B,_P(244,196));
//STRING5(CAMID,				_B,"CAM ID",			_B,_P(127,123, 34,' ','I','D'), 		_B,_P(127,123, 34,' ','I','D'),			_B,_P(30,132,189,'I','D'),		_B,_P(238,110, 97,' ','I','D'));
//STRING5(BAUDRATE,			_B,"BAUDRATE",			_B,_P(128,129), 						_B,_P(128,129),							_B,_P(126,195,192,195,112),		_B,_P(192,91));
//STRING1(2400,				_B,"2400");
//STRING1(4800,				_B,"4800");
//STRING1(9600,				_B,"9600");
//STRING1(57600,				_B,"57600");
//STRING1(115200,				_B,"115200");
//STRING5(SET_DONE,			_B,"SET DONE",			_W,_P(126,23,280,274),					_W,_P(126,23,280,274),					_W,_P(286,229,299,252),			_W,_P(188,223,273,107));
//STRING5(IMAGERANGE, 		_B,"IMAGE RANGE",		_B,_P(139,121,116,196), 				_B,_P(139,121,116,196),					_B,"IMAGE RANGE",				_B,_P(205,185,136,211));
//STRING5(FULL,				_B,"FULL",				_B,"FULL",								_B,"FULL",								_B,_P(122,191),					_W,_P(221,290,205,204));
//STRING1(1080_25p,			_B,"1080_25p");
//STRING1(1080_50i,			_B,"1080_50i");
//STRING1(720_50p,			_B,"720_50p");
//STRING1(1080_50p,			_B,"1080_50p");
//STRING1(1080_30p,			_B,"1080_30p");
//STRING1(1080_60i,			_B,"1080_60i");
//STRING1(720_60p,			_B,"720_60p");
//STRING1(1080_60p,			_B,"1080_60p");
//STRING1(720_25p,			_B,"720_25p");
//STRING1(720_30p,			_B,"720_30p");
STRING5(FRAMERATE,			_B,"FRAME RATE",		_B,_P(139,121,216,129), 		_B,_P(139,121,216,129),			_B,_P(122,192,195,131,192,195,112),						_B,_P(250,102,215,102,213,246));
STRING5(CVBS,				_B,"CVBS",				_B,_P(139,121,218,236), 		_B,_P(139,121,218,236),			_B,"CVBS",												_B,_P(198, 25,105, 23,141, 94,206));
STRING5(FREQ,				_B,"FREQ",				_B,_P(121,129),					_B,_P(121,129),					_W,_P(292,293,294),										_W,_P(227,283,193));
STRING1(50HZ,				_B,"50HZ");
STRING1(60HZ,				_B,"60HZ");
STRING5(COLOR_BAR,			_B,"COLOR BAR", 		_W,_P(277,125,262), 			_W,_P(277,125,262),				_B,_P(30,189,195,124,195),								_B,_P(239,100,115));
STRING5(LANGUAGE,			_B,"LANGUAGE",			_B,_P(250,251), 				_B,_P(250,251),					_B,_P(253,254),											_B,_P(203,202));
STRING1(ENG,				_B,"ENG");
STRING1(CHN,				_B,"CHN");
STRING1(CHNs,				_B,"CHN(S)");
STRING1(JPN,				_B,"JPN");
STRING1(KOR,				_B,"KOR");
STRING5(RESET,				_B,"RESET", 			_B,_P(223,232), 				_B,_P(223,232),					_B,_P(190,107,113,112),									_B,_P(235, 24,252));
STRING3(SETUP,				_B,"\x0c SETUP", 		_B,_P(12,' ',111,249), 			_B,_P(12,' ',111,249));
//STRING3(SHADINGDET, 		_B,"SHADING_DET",		_B,"SHADING_DET",				_B,"SHADING_DET");
STRING3(CAM_VERSION, 		_B,"CAM VERSION",		_B,_P(239,119),					_B,_P(239,119));
STRING5(CAM_TITLE,			_B,"CAM TITLE", 		_B,"CAM TITLE", 				_B,"CAM TITLE",					_B,_P(30,132,189,110,24,112,191),						_W,_P(238,110,97,281,213,282));
STRING1(UPsDN_d_CHAR_SELECT,_B,"U"" , ""D"" - CHAR SELECT");
STRING1(LEsRI_d_POSITION, 	_B,"L"" , ""R"" - POSITION");
STRING1(TLx8,				_B,_P(163,163,163,163,163,163,163,163));

STRING5(DET_WINDOW,			_B,"DET WINDOW",		_W,_P(264,231,187,188),		_W,_P(264,231,187,188),					_W,_P(283,284,255,256,286,229),		_B,_P(16,231,205,204,188,223));
STRING5(SENSITIVITY,		_B,"SENSITIVITY",		_B,_P(252,99),				_B,_P(252,99),							_B,_P(232,234),						_B,_P(16,91));
STRING5(DET_H_POS,			_B,"DET H-POS", 		_B,_P(136,241,100,137), 	_B,_P(136,241,100,137),					_B,_P(220,221,224,225),				_B,_P(14,105,211,237));
STRING5(DET_V_POS,			_B,"DET V-POS", 		_B,_P(130,225,100,137), 	_B,_P(130,225,100,137),					_B,_P(222,223,224,225),				_B,_P(189,105,211,237));
STRING5(DET_H_SIZE,			_B,"DET H-SIZE",		_B,_P(136,241,28,99),		_B,_P(136,241,28,99),					_B,_P(220,221,104,24,109),			_B,_P(14,105,240,24));
STRING5(DET_V_SIZE,			_B,"DET V-SIZE",		_B,_P(130,225,97,99),		_B,_P(130,225,97,99),					_B,_P(222,223,104,24,109),			_B,_P(189,105,240,24));
STRING5(MOTION_OSD,			_B,"MOTION OSD",		_B,_P(207,117,'O','S','D'),	_B,_P(207,117,'O','S','D'),				_W,_P(287,288,'O','S','D'),			_W,_P(257,138,' ','O','S','D'));
STRING5(TEXT_ALARM,			_B,"TEXT ALARM",		_W,_P(267,206,284,138),		_W,_P(267,206,284,138),					_W,_P(289,290,23,189,131),			_W,_P(260,217,200,99));
STRING5(SIGNAL_OUT,			_B,"SIGNAL OUT",		_W,_P(276,246,275,289),		_W,_P(276,246,275,289),					_W,_P(215,291,249,250),				_B,_P(196,251,236,104));
STRING5(DETp_MOTION, 		_B,"DET. MOTION",		_B,_P(195,201,213,231),		_B,_P(195,201,213,231),					_B,_P(184,195,105,187,194),			_B,_P(210,232,215,16,231));
STRING1(CAMp_MOVING,		_B,"CAM. MOVING");
STRING1(BRIGHT_CHGp, 		_B,"BRIGHT CHG.");
STRING5(WINDOWUSE,			_B,"WINDOW USE",		_W,_P(287,260,294,295),				_W,_P(287,260,294,295),				_W,_P(206,15,270,271),				_B,_P(210,232,215,184,209));			//	14.07.14 lgh
STRING5(WINDOWZONE,			_B,"WINDOW ZONE",		_W,_P(287,260,187,188),				_W,_P(287,260,187,188),				_W,_P(206,15,283,284,255,256),		_B,_P(210,232,215,16,231,205,204));		//	14.07.14 lgh
STRING5(DET_TONE,			_B,"DET TONE",			_W,_P(264,231,187,188,125),			_W,_P(264,231,187,188,125),			_W,_P(283,284,255,256,239),			_B,_P(16,231,205,204,186,225));		//	15.03.24 lgh
STRING5(MDRECT_FILL,		_B,"MDRECT FILL",		_W,_P(264,231,187,188,283,290),		_W,_P(264,231,187,188,283,290),		_W,_P(283,284,255,256,280,281),		_B,_P(16,231,205,204,233,210));		//	15.03.24 lgh

#if model_WDR_ROI
STRING1(ROI_WDRe, 			_B,"ROI\xa6");
STRING1(ROI_WDR, 			_B,"ROI");
#endif
STRING5(nIRIS,				_B,"IRIS",				_B,_P(20,21),					_B,_P(20,21),					_B,_P(23,24,190,106),						_B,_P(198,213,108,194));
STRING5(IRIS,				_B,"\x04 IRIS",			_B,_P(4,' ',20,21),				_B,_P(4,' ',20,21),				_B,_P(4,' ',23,24,190,106),					_B,_P(4,' ',198,213,108,194));
//STRING1(IRISCAL,			_B,"IRIS CALB");

STRING1(nDEBUG,				_B,"DEBUG");
STRING1(FUNCTION,			_B,"FUNCTION");
STRING1(AE,					_B,"AE");
STRING1(AF,					_B,"AF");
STRING5(AWB,				_B,"AWB",				_W,_P(269,241,245),				_W,_P(269,241,245),				_B,_P(26,195,112,123,188,24,112,124,189,194,106),		_B,_P(217,92,186,207,91,138,223));
STRING1(DBG_4,				_B,"DBG4");
STRING1(DBG_5,				_B,"DBG5");
STRING1(IMD_DBG,			_B,"IMD");
STRING1(TDN_DBG,			_B,"TDN");
STRING1(STATUS,				_B,"STATUS");
STRING1(BYPASS,				_B,"BYPASS");
STRING1(PAR_TYPE,			_B,"PAR TYPE");
STRING1(DEC,				_B,"DEC");
STRING1(HEX,				_B,"HEX");
STRING1(PAR_NUM,			_B,"PAR NUM");
STRING1(PAR_VAL,			_B,"PAR VAL");

STRING5(C_TEMP,				_B,"C-TEMP",			_W,_P(125,279),		_W,_P(125,279),		_W,_P(239,278,234),		_B,_P(186,207,91));
STRING1(TEMP1,				_B,"3000K");
STRING1(TEMP2,				_B,"5000K");
STRING1(TEMP3,				_B,"8000K");

//STRING1(10p,				_B,"10%");
//STRING1(20p,				_B,"20%");
//STRING1(30p,				_B,"30%");

//STRING5(HALF,				_B,"HALF",				_B,"HALF",						_B,"HALF",						_W,_P(303,304),						_W,_P(289,268,205,204));

//STRING5(EXTERN_SW,			_B,"EXTERN SW",			_B,_P(193,120,113,31),			_B,_P(193,120,113,31),			_W,_P(230,231,274,250),				_B,_P(208,140,216,104));
//STRING5(DN_THRES,			_B,"D>N THRES",			_B,_P('D','>','N',108,215,233),	_B,_P('D','>','N',108,215,233),	_W,_P(275,'>',276,226,21,227,277),	_W,_P(227,'>',201,15,221,287,257,258));
//STRING5(ND_THRES,			_B,"N>D THRES",			_B,_P('N','>','D',108,215,233),	_B,_P('N','>','D',108,215,233),	_W,_P(276,'>',275,226,21,227,277),	_W,_P(201,'>',227,15,221,287,257,258));

STRING5(RIGHT_UPe,			_B,"RIGHT UP\xa6",		_W,_P(281,272,0xa6),			_W,_P(281,272,0xa6),			_W,_P(295,296),						_W,_P(274,185,262,0xa6));
STRING5(LEFT_DOWNe,			_B,"LEFT DOWN\xa6",		_W,_P(285,292,0xa6),			_W,_P(285,292,0xa6),			_W,_P(297,298),						_W,_P(278,285,262,0xa6));

STRING5(SAVEe,				_B,"SAVE\xa6",			_B,"SAVE\xa6",					_B,"SAVE\xa6",					_W,_P(107,195,311,0xa6),								_W,_P(297,219,0xa6));
STRING1(CANCELe,			_B,"CANCEL\xa6");

STRING1(STYLE,				_B,"STYLE");
STRING1(PREVIOUS,			_B,"PREVIOUS");
STRING1(FONT_TEST,			_B,"FONT TEST");

STRING1(SHTp_SPEED,			_B,"SHT. SPEED");
STRING1(AGC_SPEED,			_B,"AGC SPEED");
STRING1(IRIS_SPEED,			_B,"IRIS SPEED");
STRING1(SHTp_OPEN,			_B,"SHT. OPEN");
STRING1(SHTp_CLOSE,			_B,"SHT. CLOSE");
STRING1(AGC_OPEN,			_B,"AGC OPEN");
STRING1(AGC_CLOSE,			_B,"AGC CLOSE");

STRING1(ERR_CHT_MAX,		_B,"ERR CHT MAX");
STRING1(ERR_CHT_MIN,		_B,"ERR CHT MIN");
STRING5(CTRLp_SPEED,		_B,"CTRL. SPEED", 			_B,_P('C','T','R','L','.',' ',22,23,234,110),	_B,_P('C','T','R','L','.',' ',22,23,234,110),	_B,_P('C','T','R','L','.',' ',106,127,195,116),	_B,_P('C','T','R','L','.',' ',192,91));
STRING1(CTRLp_STEP,			_B,"CTRL. STEP");
STRING1(STABILIZING,		_B,"STABILIZING");
//STRING1(AGC_MIN_,			_B,"AGC OFFSET"/*"AGC MIN"*/);
//STRING1(AGC_MAX_,			_B,"AGC MAX");
//STRING1(BLACK_LEVEL,		_B,"BLACK LEVEL");
STRING1(SHUTTER_MAX,		_B,"SHUTTER MAX");
STRING1(SHUTTER_MIN,		_B,"SHUTTER MIN");
STRING1(DEBLUR_MIN,			_B,"DEBLUR MIN");

STRING1(DEVELOPER,			_B,"DEVELOPER");
//STRING1(WDR_ADJUST,			_B,"WDR ADJUST");
STRING1(Ap_BRIGHT_L,		_B,"A. BRIGHT L");
STRING1(Ap_BRIGHT_S,		_B,"A. BRIGHT S");
STRING1(Dp_BRIGHT,			_B,"D. BRIGHT");
STRING1(Dp_CONTRAST,		_B,"D. CONTRAST");
STRING1(DEFAULT,			_B,"DEFAULT");

STRING1(SMALL_EDGE,			_B,"SMALL EDGE");
STRING1(BIG_EDGE,			_B,"BIG EDGE");
STRING1(LOW_AGC,			_B,"LOW AGC");
STRING1(MID_AGC,			_B,"MID AGC");
STRING1(HIGH_AGC,			_B,"HIGH AGC");
STRING1(REDp_BY_AGC,		_B,"RED. BY AGC");
STRING1(REDUCE_BY_AGC,		_B,"REDUCE BY AGC");
//STRING1(INCRpBY_AGC,		_B,"INCR.BY AGC");
//STRING1(INTERPOLATE,		_B,"INTERPOLATE");
//STRING1(LPF,				_B,"LPF");

STRING1(AWB_LSUP,			_B,"COLOR SUP.");
STRING1(HSUP_ON,			_B,"HSUP ON");
STRING1(HSUP_TH,			_B,"HSUP TH");
STRING1(LSUP_ON,			_B,"LSUP ON");
STRING1(ESUP_W_NOR,			_B,"ESUP NOR");
STRING1(ESUP_W_WDR,			_B,"ESUP WDR");
STRING1(ESUP_G_NOR,			_B,"ESUP G NOR");
STRING1(ESUP_G_WDR,			_B,"ESUP G WDR");


STRING1(AWB_SPEED,			_B,"AWB SPEED");
STRING1(A_TYPE,				_B,"A-TYPE");
STRING1(B_TYPE,				_B,"B-TYPE");
STRING1(C_TYPE,				_B,"C-TYPE");
STRING1(D_TYPE,				_B,"D-TYPE");
STRING1(E_TYPE,				_B,"E-TYPE");
STRING1(F_TYPE,				_B,"F-TYPE");
STRING1(ALWAYS,				_B,"ALWAYS");
STRING1(DAY_ONLY,			_B,"DAY ONLY");
STRING1(CENTER,				_B,"CENTER");
STRING1(SPOT,				_B,"SPOT");

STRING5(nBACKLIGHT,			_B,"BACKLIGHT",			_B,_P(25,20,26,27),					_B,_P(25,20,26,27),					_B,_P(211,207,216,217),				_B,_P(204,21));
STRING5(AE_SPEED,			_B,"AE SPEED", 			_B,_P('A','E',' ',22,23,234,110),	_B,_P('A','E',' ',22,23,234,110),	_B,_P('A','E',' ',106,127,195,116),	_B,_P('A','E',' ',192,91));
STRING5(LOWe,				_B,"LOW\xa6",			_B,_P(208,0xa6), 					_B,_P(208,0xa6),					_B,_P(197,0xa6),					_B,_P(26,212,0xa6));
STRING5(MIDDLEe, 			_B,"MIDDLE\xa6",		_B,_P(222,0xa6), 					_B,_P(222,0xa6),					_B,_P(198,0xa6),					_B,_P(229,15,0xa6));
STRING5(HIGHe,				_B,"HIGH\xa6",			_B,_P(28,0xa6),						_B,_P(28,0xa6),						_B,_P(199,0xa6),					_B,_P(29,212,0xa6));

STRING1(DNR_3D,				_B,"3D-DNR");
STRING1(DNR_2D,				_B,"2D-DNR");


//#if 0 // Error Check
//-------------------------------------------------------------------------------------------------
// Focus Adj.
#if(model_Lens==0) // 150325 WHL
//STRING5(FOCUS_AREA,			_B,"FOCUS AREA",		_W,_P(258,212,187,188),		_W,_P(258,212,187,188),		_W,_P(122,28,195,30,255,256),	_B,_P(235,222,205,204));
//STRING5(NARROW, 			_B,"NARROW",			_W,_P(286),					_W,_P(286),					_W,_P(258,14),					_W,_P(277,212));
//STRING5(WIDE, 				_B,"WIDE",				_B,_P(97),					_B,_P(97),					_W,_P(257,14),					_W,_P(261,212));
//STRING5(DISP_TONE,			_B,"DISP TONE",			_W,_P(256,261,99), 			_W,_P(256,261,99),			_W,_P(282,196,234),				_B,_P(245,111,91));
//STRING5(DISP_H_POS, 		_B,"DISP H_POS",		_B,_P(136,241,100,137), 	_B,_P(136,241,100,137),		_B,_P(220,221,224,225),			_B,_P(14,105,211,237));
//STRING5(DISP_V_POS, 		_B,"DISP V_POS",		_B,_P(130,225,100,137), 	_B,_P(130,225,100,137),		_B,_P(222,223,224,225),			_B,_P(189,105,211,237));
//STRING5(DISP_H_SIZE,		_B,"DISP H_SIZE",		_B,_P(136,241,28,99),		_B,_P(136,241,28,99),		_B,_P(220,221,104,24,109),		_B,_P(14,105,240,24));
//STRING5(DISP_V_SIZE,		_B,"DISP V_SIZE",		_B,_P(130,225,97,99),		_B,_P(130,225,97,99),		_B,_P(222,223,104,24,109),		_B,_P(189,105,240,24));
#endif

//-------------------------------------------------------------------------------------------------
// Two Moter
//#if(model_Lens==1)
#if 1
STRING1(MOTORIZED,			_B,"\x01 MOTORIZED");
//STRING1(AF_MODE,			_B,"AF MODE");
//STRING1(ONESHOTp,			_B,"ONESHOT\xa0");
//STRING1(DnN_FILTER,			_B,"D&N FILTER");
//STRING1(SYNC_OFF,			_B,"SYNC OFF");
//STRING1(SYNC_ON,			_B,"SYNC ON");
//STRING1(INITIAL,			_B,"INITIAL");

//STRING1(MAF_AUTO_TRACE,		_B,"AUTO TRACE");
STRING1(MOTOR_SETUP,		_B,"\x01 MAF SETUP");
//STRING1(MAF_DEVMODE,		_B,"DEV MODE");
//STRING1(MAF_TESTMODE,		_B,"TEST MODE");
//STRING1(MAF_ZOOM_AREA,		_B,"ZOOM AREA");
//STRING1(MAF_FOCUS_AREA,		_B,"FOCUS AREA");

//STRING1(MAF_LENS_SEL,		_B,"LENS MODEL");
//STRING1(MAF_LENS_YT2812,	_B,"YT2812");
//STRING1(MAF_LENS_YT3013,	_B,"YT3013");
//STRING1(MAF_LENS_YT3017,	_B,"YT3017");
//STRING1(MAF_LENS_YT30022,	_B,"YT30022");
//STRING1(MAF_LENS_YT3021,	_B,"YT3021");
//STRING1(MAF_LENS_CSTM,		_B,"CUSTOM");

//STRING1(MAF_TOTAL_AREA,		_B,"TOTAL AREA");
//STRING1(MAF_START_POS,		_B,"START POS");
//STRING1(MAF_END_POS,		_B,"END POS");

//STRING1(MAF_DISABLE,		_B,"DISABLE");

#else
//STRING5(TWOMOTOR,			_B,"\x01 2-MOTOR",	_W,_P(0x01,' ',211,19,296,297),		_W,_P(0x01,' ',211,19,296,297),		_W,_P(0x01,' ',300,195,184,195,110,195),		_B,_P(0x01,' ',245,112,241));
//STRING5(AF_MODE,			_B,"AF MODE",		_B,_P('A','F',109,140),				_B,_P('A','F',109,140),				_B,_P('A','F',184,195,116),						_B,_P(217,92,235,222,112,93));
//STRING5(SCANNING,   		_B,"SCANNING",		_B,"SCANNING",						_B,"SCANNING",						_W,_P(209,210,301,302),							_B,_P(224,202,136,211));
//STRING5(INITIAL,			_B,"INITIAL",		_W,_P(298,137,248),					_W,_P(298,137,248),					_W,_P(305,243,306),								_B,_P(235,24,252));
//STRING5(ONEPUSHAF,			_B,"ONEPUSHAF",		_B,"ONEPUSHAF",						_B,"ONEPUSHAF",						_B,_P(188,194,128,113,105,186,195,'A','F'),		_W,_P(121,291,92,292,217,92,235,222));
//STRING5(SyncTDN,			_B,"SYNC TDN",		_B,"SYNC TDN",						_B,"SYNC TDN",						_B,_P(115,195,117,24,112,242,243),				_B,_P(227,201,15,92,24));
#endif
//#else
STRING5(FOCUS_ADJ,			_B,"\x01 FOCUS ADJ",	_W,_P(1,' ',258,212,263,131),	_W,_P(1,' ',258,212,263,131),	_B,_P(1,' ',122,28,195,30,106,209,210),		_B,_P(1,' ',235,222,225,223));
//#endif

#if (IMAGE_INPUT_MODE==1)
// 20151113 - WHL : PIP Menu Setup
//STRING5(PIP,				_B,"PIP",			_B,"PIP",			_B,"PIP",			_B,"PIP",			_W,_P(305,306,307));
//STRING5(SIZE,				_B,"SIZE",			_B,_P(99),			_B,_P(99),			_B,_P(104,24,109),	_B,_P(240,24));
#endif

//STRING1(WARNINGTEST1,		_B,"\x07\x08\x09\x0a\x0b");
//STRING1(WARNINGTEST2,		_B,_P(7,8,9,10,11));

STRING5(OUTPUTSET,			_B,"\x02 OUTPUT SET",	_B,_P(0x02,' ',139,121,229,230),	_B,_P(0x02,' ',139,121,229,230),	_B,_P(0x02,' ',249,250,184,195,116),	_B,_P(0x02,' ',236,104,188,223));

//STRING5(25P_NORMAL,			_B,"25P NORMAL",		_B,"25P NORMAL",					_B,"25P NORMAL",					_B,"25P NORMAL",						_B,_P('2','5','P'));
//STRING5(12P_WDR,			_B,"12P WDR",			_B,"12P WDR",						_B,"12P WDR",						_B,"12P WDR",							_B,_P('1','2','P',' ',204,21,138,223));
//STRING5(25P_DNR,			_B,"25P DNR",			_B,"25P DNR",						_B,"25P DNR",						_B,"25P DNR",							_W,_P('2','5','P',' ',276,212,16,268));
//STRING5(50P_NORMAL,			_B,"50P NORMAL",		_B,"50P NORMAL",					_B,"50P NORMAL",					_B,"50P NORMAL",						_B,_P('5','0','P'));
//STRING5(25P_WDR,			_B,"25P WDR",			_B,"25P WDR",						_B,"25P WDR",						_B,"25P WDR",							_B,_P('2','5','P',' ',204,21,138,223));
//STRING5(50P_DNR,			_B,"50P DNR",			_B,"50P DNR",						_B,"50P DNR",						_B,"50P DNR",							_W,_P('5','0','P',' ',276,212,16,268));
//STRING5(30P_NORMAL,			_B,"30P NORMAL",		_B,"30P NORMAL",					_B,"30P NORMAL",					_B,"30P NORMAL",						_B,_P('3','0','P'));
//STRING5(15P_WDR,			_B,"15P WDR",			_B,"15P WDR",						_B,"15P WDR",						_B,"15P WDR",							_B,_P('1','5','P',' ',204,21,138,223));
//STRING5(30P_DNR,			_B,"30P DNR",			_B,"30P DNR",						_B,"30P DNR",						_B,"30P DNR",							_W,_P('3','0','P',' ',276,212,16,268));
//STRING5(60P_NORMAL,			_B,"60P NORMAL",		_B,"60P NORMAL",					_B,"60P NORMAL",					_B,"60P NORMAL",						_B,_P('6','0','P'));
//STRING5(30P_WDR,			_B,"30P WDR",			_B,"30P WDR",						_B,"30P WDR",						_B,"30P WDR",							_B,_P('3','0','P',' ',204,21,138,223));
//STRING5(60P_DNR,			_B,"60P DNR",			_B,"60P DNR",						_B,"60P DNR",						_B,"60P DNR",							_W,_P('6','0','P',' ',276,212,16,268));

#if (DOOR_BELL_CAMERA==1)
//STRING5(DC_LP_MODE,			_B,"POWER SAVE",		_B,"POWER SAVE",		_B,"POWER SAVE",		_B,"POWER SAVE",		_W,_P(221,275,112,93));
//STRING5(TIME_10S,			_B,"10S",				_B,"10S",				_B,"10S",				_B,"10S",				_B,_P('1','0',235));
//STRING5(TIME_15S,			_B,"15S",				_B,"15S",				_B,"15S",				_B,"15S",				_B,_P('1','5',235));
//STRING5(TIME_20S,			_B,"20S",				_B,"20S",				_B,"20S",				_B,"20S",				_B,_P('2','0',235));
//STRING5(TIME_25S,			_B,"25S",				_B,"25S",				_B,"25S",				_B,"25S",				_B,_P('2','5',235));
//STRING5(TIME_30S,			_B,"30S",				_B,"30S",				_B,"30S",				_B,"30S",				_B,_P('3','0',235));
#endif




#define __ISP_MENU_STR__
#include "isp_user_parameter.h"
#undef __ISP_MENU_STR__


#endif /* __ISP_MENU_STR_H__ */
