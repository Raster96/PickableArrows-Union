META
{
  Parser = Menu;
  After = zUnionMenu.d;
  Namespace = PickableArrows;
};

// Namespace = PickableArrows
// Important !!!
// Supporting Unicode symbols.
// File encoding: UTF-8 (without BOM).

// ------ Constants ------
const int Start_PY  = 1400;
const int Title_PY  = 450;
const int Menu_DY   = 550;
// Text
const int Text_PX   = 400;  // Position at left side
const int Text_SX   = 8000; // Size X
const int Text_SY   = 750;  // Size Y
const int Text_DY   = 120;  // Delta alignment
// Slider
const int Slider_PX = 6400; // Position at left side
const int Slider_SX = 1500; // Size X
const int Slider_SY = 600;  // Size Y
const int Slider_DY = 0;    // Delta alignment
// Choice
const int Choice_PX = 6400; // Position at left side
const int Choice_SX = 1500; // Size X
const int Choice_SY = 350;  // Size Y
const int Choice_DY = 120;  // Delta alignment

const string MenuBackPic   = "UnionMenu_BackPic.tga";
const string ItemBackPic   = "";
const string ChoiceBackPic = "MENU_CHOICE_BACK.TGA";
const string SliderBackPic = "MENU_SLIDER_BACK.TGA";
const string SliderPosPic  = "MENU_SLIDER_POS.TGA";
const string FontSmall     = "font_old_10_white.tga";
const string FontBig       = "font_old_20_white.tga";

var int CurrentMenuItem_PY;

// ------ Prototypes ------
func void InitializeBackPicturesAndFonts()
{
  MenuBackPic   = MENU_BACK_PIC;
  ItemBackPic   = MENU_ITEM_BACK_PIC;
  ChoiceBackPic = MENU_CHOICE_BACK_PIC;
  SliderBackPic = MENU_SLIDER_BACK_PIC;
  SliderPosPic  = MENU_SLIDER_POS_PIC;
  FontSmall     = MENU_FONT_SMALL;
  FontBig       = MENU_FONT_DEFAULT;
};

prototype C_EMPTY_MENU_DEF(C_MENU)
{
  InitializeBackPicturesAndFonts();
  C_MENU_DEF();
  backpic    = MenuBackPic;
  items[0]   = "";
  items[100] = "Union_menuitem_back";
  flags      = flags | MENU_SHOW_INFO;
};

instance C_MENU_ITEM_TEXT_BASE(C_MENU_ITEM_DEF)
{
  backpic        = ItemBackPic;
  posx           = Text_PX;
  posy           = Start_PY;
  dimx           = Text_SX;
  dimy           = Text_SY;
  flags          = flags | IT_EFFECTS_NEXT;
  onselaction[0] = SEL_ACTION_UNDEF;
};

instance C_MENUITEM_CHOICE_BASE(C_MENU_ITEM_DEF)
{
  backpic  = ChoiceBackPic;
  type     = MENU_ITEM_CHOICEBOX;
  fontname = FontSmall;
  posx     = Choice_PX;
  posy     = Start_PY + Choice_DY;
  dimx     = Choice_SX;
  dimy     = Choice_SY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};

instance C_MENUITEM_SLIDER_BASE(C_MENU_ITEM_DEF)
{
  backpic       = SliderBackPic;
  type          = MENU_ITEM_SLIDER;
  fontname      = FontSmall;
  posx          = Slider_PX;
  posy          = Start_PY + Slider_DY;
  dimx          = Slider_SX;
  dimy          = Slider_SY;
  flags         = flags & ~IT_SELECTABLE;
  flags         = flags | IT_TXT_CENTER;
  userstring[0] = SliderPosPic;
};

instance MenuItem_Opt_Headline(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = Title_PY;
  dimx    = 8100;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = Str_GetLocalizedString(
    "PickableArrows НАСТРОЙКИ",
    "PickableArrows SETTINGS",
    "PickableArrows EINSTELLUNGEN",
    "USTAWIENIA PickableArrows"
  );
};

func int Act_OpenWebLink()
{
  Open_Link("https://github.com/Raster96/PickableArrows-Union");
  return 0;
};

instance MenuItem_Opt_Open_Link(C_MENU_ITEM_DEF)
{
  C_MENU_ITEM_TEXT_BASE();
  posy += MENU_DY * 8;

  posx             = 64;
  onselaction[0]   = SEL_ACTION_UNDEF;
  oneventaction[1] = Act_OpenWebLink;
  flags            = flags | IT_TXT_CENTER;
  text[0]          = Str_GetLocalizedString(
    "Открыть страницу проекта",
    "Open project page",
    "Projektseite öffnen",
    "Otwórz stronę projektu"
  );

  text[1]          = "https://github.com/Raster96/PickableArrows-Union";
};

// ------ Menu ------
instance MenuItem_Union_Auto_PickableArrows(C_MENU_ITEM_UNION_DEF)
{
  text[0]          = "PickableArrows";
  text[1] = Str_GetLocalizedString(
    "Настройте параметры PickableArrows",
    "Configure PickableArrows settings",
    "PickableArrows-Einstellungen konfigurieren",
    "Zmień ustawienia PickableArrows"
  );
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "PickableArrows:Menu_Opt_PickableArrows";
};

instance Menu_Opt_PickableArrows(C_EMPTY_MENU_DEF)
{
  Menu_SearchItems("PickableArrows:MENUITEM_OPT_PickableArrows_*");
};

instance MenuItem_Opt_PickableArrows_Headline(C_MENU_ITEM)
{
  MenuItem_Opt_Headline();
};

// ------ Enabled ------
instance MenuItem_Opt_PickableArrows_Enabled(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 1;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Enabled";
  text[1] = Str_GetLocalizedString(
    "Включить или отключить весь плагин",
    "Enable or disable the entire plugin",
    "Das gesamte Plugin aktivieren oder deaktivieren",
    "Włącz lub wyłącz cały plugin"
  );
};

instance MenuItem_Opt_PickableArrows_Enabled_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "Enabled";
  onchgsetoptionsection = "PICKABLE_ARROWS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ GroundRecoveryChance ------
instance MenuItem_Opt_PickableArrows_GroundRecoveryChance(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 2;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "GroundRecoveryChance";
  text[1] = Str_GetLocalizedString(
    "Шанс восстановления стрелы с земли",
    "Chance to recover arrow from ground",
    "Chance, einen Pfeil vom Boden zu bergen",
    "Szansa na odzyskanie strzały z ziemi"
  );
};

instance MenuItem_Opt_PickableArrows_GroundRecoveryChance_Slider(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "GroundRecoveryChance";
  onchgsetoptionsection = "PICKABLE_ARROWS";
  text[0]               = "0%|1%|2%|3%|4%|5%|6%|7%|8%|9%|10%|11%|12%|13%|14%|15%|16%|17%|18%|19%|20%|21%|22%|23%|24%|25%|26%|27%|28%|29%|30%|31%|32%|33%|34%|35%|36%|37%|38%|39%|40%|41%|42%|43%|44%|45%|46%|47%|48%|49%|50%|51%|52%|53%|54%|55%|56%|57%|58%|59%|60%|61%|62%|63%|64%|65%|66%|67%|68%|69%|70%|71%|72%|73%|74%|75%|76%|77%|78%|79%|80%|81%|82%|83%|84%|85%|86%|87%|88%|89%|90%|91%|92%|93%|94%|95%|96%|97%|98%|99%|100%";
};

// ------ NpcRecoveryChance ------
instance MenuItem_Opt_PickableArrows_NpcRecoveryChance(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 3;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "NpcRecoveryChance";
  text[1] = Str_GetLocalizedString(
    "Шанс восстановления стрелы из NPC",
    "Chance to recover arrow from NPC",
    "Chance, einen Pfeil von NPC zu bergen",
    "Szansa na odzyskanie strzały z NPC"
  );
};

instance MenuItem_Opt_PickableArrows_NpcRecoveryChance_Slider(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "NpcRecoveryChance";
  onchgsetoptionsection = "PICKABLE_ARROWS";
  text[0]               = "0%|1%|2%|3%|4%|5%|6%|7%|8%|9%|10%|11%|12%|13%|14%|15%|16%|17%|18%|19%|20%|21%|22%|23%|24%|25%|26%|27%|28%|29%|30%|31%|32%|33%|34%|35%|36%|37%|38%|39%|40%|41%|42%|43%|44%|45%|46%|47%|48%|49%|50%|51%|52%|53%|54%|55%|56%|57%|58%|59%|60%|61%|62%|63%|64%|65%|66%|67%|68%|69%|70%|71%|72%|73%|74%|75%|76%|77%|78%|79%|80%|81%|82%|83%|84%|85%|86%|87%|88%|89%|90%|91%|92%|93%|94%|95%|96%|97%|98%|99%|100%";
};

// ------ AllowNpcInventoryPickup ------
instance MenuItem_Opt_PickableArrows_AllowNpcInventoryPickup(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 4;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "AllowNpcInventoryPickup";
  text[1] = Str_GetLocalizedString(
    "Добавлять стрелы в инвентарь пораженных NPC",
    "Add arrows to hit NPC inventory",
    "Pfeile zum Inventar getroffener NPCs hinzufügen",
    "Dodawaj strzały do ekwipunku trafionych NPC"
  );
};

instance MenuItem_Opt_PickableArrows_AllowNpcInventoryPickup_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "AllowNpcInventoryPickup";
  onchgsetoptionsection = "PICKABLE_ARROWS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ AllowGroundPickup ------
instance MenuItem_Opt_PickableArrows_AllowGroundPickup(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 5;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "AllowGroundPickup";
  text[1] = Str_GetLocalizedString(
    "Позволить подбирать стрелы с земли",
    "Allow picking up arrows from ground",
    "Erlaube das Aufheben von Pfeilen vom Boden",
    "Pozwól na podnoszenie strzał z ziemi"
  );
};

instance MenuItem_Opt_PickableArrows_AllowGroundPickup_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "AllowGroundPickup";
  onchgsetoptionsection = "PICKABLE_ARROWS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ RequireDamageForNpcRecovery ------
instance MenuItem_Opt_PickableArrows_RequireDamageForNpcRecovery(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 6;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "RequireDamageForNpcRecovery";
  text[1] = Str_GetLocalizedString(
    "Требовать повреждения для восстановления из NPC",
    "Require damage dealt to recover from NPC",
    "Schaden erforderlich fuer Bergung von NPC",
    "Wymagaj zadania obrażeń dla odzyskania z NPC"
  );
};

instance MenuItem_Opt_PickableArrows_RequireDamageForNpcRecovery_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "RequireDamageForNpcRecovery";
  onchgsetoptionsection = "PICKABLE_ARROWS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ ChanceDependsOnSkills ------
instance MenuItem_Opt_PickableArrows_ChanceDependsOnSkills(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 7;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "ChanceDependsOnSkills";
  text[1] = Str_GetLocalizedString(
    "Шанс зависит от навыка лука/арбалета",
    "Chance depends on bow/crossbow skill",
    "Chance haengt von Bogen/Armbrust-Faehigkeit ab",
    "Szansa zależy od umiejętności łuk/kusza"
  );
};

instance MenuItem_Opt_PickableArrows_ChanceDependsOnSkills_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "ChanceDependsOnSkills";
  onchgsetoptionsection = "PICKABLE_ARROWS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ Open Project Page ------
instance MenuItem_Opt_PickableArrows_Open_Link(C_MENU_ITEM)
{
  MenuItem_Opt_Open_Link();
};