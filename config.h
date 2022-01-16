#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 6;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int minwsz    = 20;       /* Minimal heigt of a client for smfact */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "SourceCodePro:size=14" };
static const char dmenufont[]       = "SourceCodePro:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
    [SchemeHid]  = { col_cyan,  col_gray1, col_cyan  },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border     */
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    { "st-256color",      NULL,     NULL,           0,         0,          1,           1,        -1 },
    { "Wine",    NULL,     "微信",          1 << 8,    0,          0,           0,        -1 },
    { "Wine",    NULL,     "企业微信",      1 << 8,    0,          0,           0,        -1 },
    { NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
    { "Dragon-drag-and-drop", NULL, "dragon",       0,         1,          0,           1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const float smfact     = 0.00; /* factor of tiled clients [0.00..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static int attachbelow = 0;    /* 1 means attach after the currently active window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[T]",      tile },    /* first entry is default */
    { "[F]",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"/home/liuyan/bin/scripts/dmenu_run", NULL};
static const char *rofi_window[] = {"/home/liuyan/bin/scripts/rofi-run.sh", NULL};
static const char *termcmd[] = {"st", NULL};
static const char *sptermcmd[] = {"st", "-n", "spterm", NULL};
static const char *lockcmd[] = {"slock", NULL};
static const char *suspend[] = {"/home/liuyan/bin/scripts/suspend.sh", NULL};
static const char *ranger[] = {"st", "-e", "ranger", "-r","/home/liuyan/.config/ranger" , NULL};
static const char *flameshot[] = {"flameshot","gui", NULL};

static const char *upvol[] = {"/home/liuyan/bin/scripts/vol-up.sh", NULL};
static const char *downvol[] = {"/home/liuyan/bin/scripts/vol-down.sh", NULL};
static const char *mutevol[] = {"/home/liuyan/bin/scripts/vol-toggle.sh", NULL};
static const char *downbrightness[] = {"/home/liuyan/bin/scripts/brightness-down.sh", NULL};
static const char *upbrightness[] = {"/home/liuyan/bin/scripts/brightness-up.sh", NULL};

static const char *trayercmd[] = {"/home/liuyan/bin/scripts/toggle-t.sh", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34",  NULL }; //"-e", "tmux" -g 80x24
static const char *switch2wechat[] = {"wmctrl", "-x", "-a", "wechat", NULL};
static const char *switch2emacs[] = {"wmctrl", "-x", "-a", "emacs", NULL};
static const char *switch2chrome[] = {"wmctrl", "-x", "-a", "chrome", NULL};
static const char *switch2term[] = {"wmctrl", "-x", "-a", "spterm", NULL};
static const char *emacsanywhere[] = {"/home/liuyan/.emacs_anywhere/bin/run", NULL};
static const char *websearch[] = {"/home/liuyan/bin/scripts/web-search.sh", NULL};

static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_o,      spawn,          {.v = rofi_window } },
    { MODKEY,                       XK_u,      spawn,          {.v = ranger } },
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = sptermcmd } },
    { MODKEY,                       XK_t,      spawn,          {.v = trayercmd } },
    { MODKEY|ControlMask,           XK_p,      spawn,          {.v = suspend} },
    { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = lockcmd} },
    { MODKEY,                XK_bracketleft,   spawn,          {.v = emacsanywhere } },
    { Mod4Mask,                     XK_n,      spawn,          {.v = switch2chrome } },
    { Mod4Mask,                     XK_m,      spawn,          {.v = switch2emacs } },
    { Mod4Mask,                     XK_comma,  spawn,          {.v = switch2term } },
    { Mod4Mask,                     XK_period, spawn,          {.v = switch2wechat } },
    { Mod4Mask,                     XK_o,      spawn,          {.v = websearch } },
    { 0,                            XK_F4,     spawn,          {.v = flameshot} },
    { 0,                   XF86XK_AudioLowerVolume, spawn,     {.v = downvol } },
    { 0,                   XF86XK_AudioMute,        spawn,     {.v = mutevol } },
    { 0,                   XF86XK_AudioRaiseVolume, spawn,     {.v = upvol   } },
    { 0,                   XF86XK_MonBrightnessUp, spawn,      {.v = upbrightness   } },
    { 0,                   XF86XK_MonBrightnessDown, spawn,    {.v = downbrightness   } },
    { MODKEY,                       XK_s,      togglesticky,   {0} },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ControlMask,           XK_h,      setsmfact,      {.f = +0.05} },
    { MODKEY|ControlMask,           XK_l,      setsmfact,      {.f = -0.05} },
    { MODKEY|ControlMask,           XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_q,      killclient,     {0} },
    { MODKEY,                       XK_Left,   viewtoleft,     {0} },
    { MODKEY,                       XK_Right,  viewtoright,    {0} },
    { MODKEY|ControlMask,           XK_Left,   tagtoleft,      {0} },
    { MODKEY|ControlMask,           XK_Right,  tagtoright,     {0} },
    { MODKEY|ShiftMask,             XK_j,      viewtoleft,     {0} },
    { MODKEY|ShiftMask,             XK_k,      viewtoright,    {0} },
    { MODKEY|ShiftMask,             XK_h,      tagtoleft,      {0} },
    { MODKEY|ShiftMask,             XK_l,      tagtoright,     {0} },
    { MODKEY|ControlMask,           XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ControlMask,           XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY|ControlMask,           XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },
    { MODKEY|ShiftMask,             XK_m,      focusmaster,    {0} },
    // { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ControlMask,           XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ControlMask,           XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ControlMask,           XK_period, tagmon,         {.i = +1 } },
    { MODKEY,                       XK_f,      fullscreen,     {0} },
    { MODKEY,                       XK_semicolon,      hidewin,        {0} },
    { MODKEY|ControlMask,           XK_semicolon,      restorewin,     {0} },
    { MODKEY|ControlMask,           XK_j,      rotatestack,    {.i = +1 } },
    { MODKEY|ControlMask,           XK_k,      rotatestack,    {.i = -1 } },
    { MODKEY,                       XK_grave,  toggleAttachBelow,  {0} },
    { MODKEY,                       XK_space,  togglescratch,  {.v = scratchpadcmd } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ControlMask,           XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button1,        togglewin,      {0} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
