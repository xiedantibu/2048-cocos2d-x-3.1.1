//
//  Resource.h
//  2048
//
//  Created by agile on 14-6-23.
//
//

#ifndef _048_Resource_h
#define _048_Resource_h


static const int M_TAG_LAYER_BG             =0x1000;
static const int M_TAG_LAYER_BORDER_BG      =0x1001;
static const int M_TAG_SPRITES              =0x1002;
static const int M_TAG_LABEL                =0x1003;
static const int M_TAG_LAYERBG              =0x1004;
static const int M_TAG_Exit                 =0x1005;
static const int M_TAG_SCORE                =0x1006;
static const int M_TAG_HIGH_SCORE           =0x1007;
static const int M_TAG_TEMP                 =0x1008;
static const int M_TAG_SCORE_SPRITE         =0x1009;
static const int M_TAG_GAME_OVER            =0x1010;
static const int M_TAG_GAME_SUCCESS         =0x1011;

static const int M_ZORDER_spriteItem        =1000;
static const int M_ZORDER_middle            =1001;
static const int M_ZORDER_layerbg           =1002;
static const int M_ZORDER_label             =1003;




static const char M_IMG_BGB[]               ="bgb.png";
static const char M_IMG_BGPL[]              ="bgpl.png";
static const char M_IMG_MENU[]              ="menu.png";
static const char M_IMG_SCORE[]              ="score.png";

static const char M_FONT_CLEARSANS[]              ="ClearSans-Bold.ttf";
static const char M_FONT_CLEARSANS_2[]              ="ClearSans-Light.ttf";
static const char M_FONT_CLEARSANS_3[]              ="ClearSans-Regular.ttf";


static const char M_FILE_BASE_SETTING[]         ="base_setting.plist";
static const char M_FILE_KEY_VARIETY_VALUES[]   ="settings_variety_values";
static const char M_FILE_KEY_VARIETY[]          ="variety_entries";
static const char M_FILE_KEY_SIZEX[]          ="SizeX";
static const char M_FILE_KEY_SIZEY[]          ="SizeY";
static const char M_FILE_KEY_HIGHT_SCORE[]          ="hightscore";
static const char M_FILE_KEY_LAST_SCORE[]          ="lastscore";
static const char M_FILE_KEY_GAME_TYPE01[]          ="type-1";
static const char M_FILE_KEY_GAME_TYPE02[]          ="type-2";
static const char M_FILE_KEY_GAME_TYPE03[]          ="type-3";
#endif
