#pragma once
#if defined(IS_DEBUG)
#include "gfx/color.h"

#ifdef __cplusplus
#define C_FUNC extern "C"
#else
#define C_FUNC
#endif

#include "core/vec2.h"

#define IMGUI_CONFIG_FLAGS_NONE                     (0)
#define IMGUI_CONFIG_FLAGS_NAV_ENABLE_KEYBOARD      (1 << 0)
#define IMGUI_CONFIG_FLAGS_NAV_ENABLE_GAMEPAD       (1 << 1)
#define IMGUI_CONFIG_FLAGS_NO_MOUSE                 (1 << 4)
#define IMGUI_CONFIG_FLAGS_NO_MOUSE_CURSOR_CHANGE   (1 << 5)
#define IMGUI_CONFIG_FLAGS_NO_KEYBOARD              (1 << 6)
#define IMGUI_CONFIG_FLAGS_DOCKING_ENABLE           (1 << 7)
#define IMGUI_CONFIG_FLAGS_VIEWPORTS_ENABLE         (1 << 10)
#define IMGUI_CONFIG_FLAGS_IS_SRGB                  (1 << 20)
#define IMGUI_CONFIG_FLAGS_IS_TOUCH_SCREEN          (1 << 21)

#define IMGUI_CHILD_FLAGS_NONE                      (0)
#define IMGUI_CHILD_FLAGS_BORDERS                   (1 << 0)
#define IMGUI_CHILD_FLAGS_ALWAYS_USE_WINDOW_PADDING (1 << 1)
#define IMGUI_CHILD_FLAGS_RESIZE_X                  (1 << 2)
#define IMGUI_CHILD_FLAGS_RESIZE_Y                  (1 << 3)
#define IMGUI_CHILD_FLAGS_AUTO_RESIZE_X             (1 << 4)
#define IMGUI_CHILD_FLAGS_AUTO_RESIZE_Y             (1 << 5)
#define IMGUI_CHILD_FLAGS_ALWAYS_AUTO_RESIZE        (1 << 6)
#define IMGUI_CHILD_FLAGS_FRAME_STYLE               (1 << 7)
#define IMGUI_CHILD_FLAGS_NAV_FLATTENED             (1 << 8)

#define IMGUI_WINDOW_FLAGS_NONE                             (0)
#define IMGUI_WINDOW_FLAGS_NO_TITLE_BAR                     (1 << 0)
#define IMGUI_WINDOW_FLAGS_NO_RESIZE                        (1 << 1)
#define IMGUI_WINDOW_FLAGS_NO_MOVE                          (1 << 2)
#define IMGUI_WINDOW_FLAGS_NO_SCROLLBAR                     (1 << 3)
#define IMGUI_WINDOW_FLAGS_NO_SCROLL_WITH_MOUSE             (1 << 4)
#define IMGUI_WINDOW_FLAGS_NO_COLLAPSE                      (1 << 5)
#define IMGUI_WINDOW_FLAGS_ALWAYS_AUTO_RESIZE               (1 << 6)
#define IMGUI_WINDOW_FLAGS_NO_BACKGROUND                    (1 << 7)
#define IMGUI_WINDOW_FLAGS_NO_SAVED_SETTINGS                (1 << 8)
#define IMGUI_WINDOW_FLAGS_NO_MOUSE_INPUTS                  (1 << 9)
#define IMGUI_WINDOW_FLAGS_MENU_BAR                         (1 << 10)
#define IMGUI_WINDOW_FLAGS_HORIZONTAL_SCROLL_BAR            (1 << 11)
#define IMGUI_WINDOW_FLAGS_NO_FOCUS_ON_APPEARING            (1 << 12)
#define IMGUI_WINDOW_FLAGS_NO_BRING_TO_FRONT_ON_FOCUS       (1 << 13)
#define IMGUI_WINDOW_FLAGS_ALWAYS_VERTICAL_SCROLLBAR        (1 << 14)
#define IMGUI_WINDOW_FLAGS_ALWAYS_HORIZONTAL_SCROLLBAR      (1 << 15)
#define IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS                    (1 << 16)
#define IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS                     (1 << 17)
#define IMGUI_WINDOW_FLAGS_UNSAVED_DOCUMENT                 (1 << 18)
#define IMGUI_WINDOW_FLAGS_NO_DOCKING                       (1 << 19)
#define IMGUI_WINDOW_FLAGS_NO_NAV                           (IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS | IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS)
#define IMGUI_WINDOW_FLAGS_NO_DECORATION                    (IMGUI_WINDOW_FLAGS_NO_TITLE_BAR | IMGUI_WINDOW_FLAGS_NO_RESIZE | IMGUI_WINDOW_FLAGS_NO_SCROLLBAR | IMGUI_WINDOW_FLAGS_NO_COLLAPSE)
#define IMGUI_WINDOW_FLAGS_NO_INPUTS                        (IMGUI_WINDOW_FLAGS_NO_MOUSE_INPUTS | IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS | IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS)
#define IMGUI_WINDOW_FLAGS_DOCK_NODE_HOST                   (1 << 23)
#define IMGUI_WINDOW_FLAGS_CHILD_WINDOW                     (1 << 24)
#define IMGUI_WINDOW_FLAGS_TOOLTIP                          (1 << 25)
#define IMGUI_WINDOW_FLAGS_POPUP                            (1 << 26)
#define IMGUI_WINDOW_FLAGS_MODAL                            (1 << 27)
#define IMGUI_WINDOW_FLAGS_CHILD_MENU                       (1 << 28)

#define IMGUI_COL_TEXT                                      0

typedef uint32_t imgui_id;
typedef int32_t imgui_config_flags;
typedef int32_t imgui_backend_flags;
typedef int32_t imgui_window_flags;
typedef int32_t imgui_child_flags;
typedef int32_t imgui_col;

typedef struct imgui_io
{
    imgui_config_flags ConfigFlags;
    imgui_backend_flags BackendFlags;
    vec2 DisplaySize;
    vec2 DisplayFramebufferScale;
    float DeltaTime;
    float IniSavingRate;
    const char* IniFilename;
    const char* LogFilename;
    void* UserData;
    void* Fonts;
    void* FontDefault;
    bool FontAllowUserScaling;
    bool ConfigNavSwapGamepadButtons;
    bool ConfigNavMoveSetMousePos;
    bool ConfigNavCaptureKeyboard;
    bool ConfigNavEscapeClearFocusItem;
    bool ConfigNavEscapeClearFocusWindow;
    bool ConfigNavCursorVisibleAuto;
    bool ConfigNavCursorVisibleAlways;
    bool ConfigDockingNoSplit;
    bool ConfigDockingNoDockingOver;
    bool ConfigDockingWithShift;
    bool ConfigDockingAlwaysTabBar;
    bool ConfigDockingTransparentPayload;
    bool ConfigViewportsNoAutoMerge;
    bool ConfigViewportsNoTaskBarIcon;
    bool ConfigViewportsNoDecoration;
    bool ConfigViewportsNoDefaultParent;
    bool ConfigViewportsPlatformFocusSetsImGuiFocus;
    bool ConfigDpiScaleFonts;
    bool ConfigDpiScaleViewports;
    bool MouseDrawCursor;
    bool ConfigMacOSXBehaviors;
    bool ConfigInputTrickleEventQueue;
    bool ConfigInputTextCursorBlink;
    bool ConfigInputTextEnterKeepActive;
    bool ConfigDragClickToInputText;
    bool ConfigWindowsResizeFromEdges;
    bool ConfigWindowsMoveFromTitleBarOnly;
    bool ConfigWindowsCopyContentsWithCtrlC;
    bool ConfigScrollbarScrollByPage;
    float ConfigMemoryCompactTimer;
    float MouseDoubleClickTime;
    float MouseDoubleClickMaxDist;
    float MouseDragThreshold;
    float KeyRepeatDelay;
    float KeyRepeatRate;
    bool ConfigErrorRecovery;
    bool ConfigErrorRecoveryEnableAssert;
    bool ConfigErrorRecoveryEnableDebugLog;
    bool ConfigErrorRecoveryEnableTooltip;
    bool ConfigDebugIsDebuggerPresent;
    bool ConfigDebugHighlightIdConflicts;
    bool ConfigDebugHighlightIdConflictsShowItemPicker;
    bool ConfigDebugBeginReturnValueOnce;
    bool ConfigDebugBeginReturnValueLoop;
    bool ConfigDebugIgnoreFocusLoss;
    bool ConfigDebugIniSettings;
    const char* BackendPlatformName;
    const char* BackendRendererName;
    void* BackendPlatformUserData;
    void* BackendRendererUserData;
    void* BackendLanguageUserData;
    bool WantCaptureMouse;
    bool WantCaptureKeyboard;
    bool WantTextInput;
    bool WantSetMousePos;
    bool WantSaveIniSettings;
    bool NavActive;
    bool NavVisible;
    float Framerate;
    int MetricsRenderVertices;
    int MetricsRenderIndices;
    int MetricsRenderWindows;
    int MetricsActiveWindows;
    vec2 MouseDelta;
    void* Ctx;
    vec2 MousePos;
    bool MouseDown[5];
    float MouseWheel;
    float MouseWheelH;
    uint32_t MouseSource;
    uint32_t MouseHoveredViewport;
    bool KeyCtrl;
    bool KeyShift;
    bool KeyAlt;
    bool KeySuper;
    int32_t KeyMods;
    struct { bool Down; float DownDuration; float DownDurationPrev; float AnalogValue;} KeysData[667 - 512];
    bool WantCaptureMouseUnlessPopupClose;
    vec2 MousePosPrev;
    vec2 MouseClickedPos[5];
    double MouseClickedTime[5];
    bool MouseClicked[5];
    bool MouseDoubleClicked[5];
    uint16_t MouseClickedCount[5];
    uint16_t MouseClickedLastCount[5];
    bool MouseReleased[5];
    double MouseReleasedTime[5];
    bool MouseDownOwned[5];
    bool MouseDownOwnedUnlessPopupClose[5];
    bool MouseWheelRequestAxisSwap;
    bool MouseCtrlLeftAsRightClick;
    float MouseDownDuration[5];
    float MouseDownDurationPrev[5];
    vec2 MouseDragMaxDistanceAbs[5];
    float MouseDragMaxDistanceSqr[5];
    float PenPressure;
    bool AppFocusLost;
    bool AppAcceptingEvents;
    uint16_t InputQueueSurrogate;
    struct ImVector_ImWchar {int Size; int Capacity; void* Data;} InputQueueCharacters;
} imgui_io;

C_FUNC imgui_io *imgui_get_io();
C_FUNC void *imgui_create_context(void *shared_font_atlas);
C_FUNC void imgui_destroy_context(void *ctx);

C_FUNC bool imgui_opengl3_init(const char *glsl_version);
C_FUNC void imgui_opengl3_new_frame();
C_FUNC void imgui_impl_opengl3_render_draw_data(void *draw_data);
C_FUNC void imgui_impl_opengl3_shutdown();

C_FUNC bool imgui_sdl3_init_for_opengl(void *window, void *context);
C_FUNC void imgui_sdl3_new_frame();
C_FUNC bool imgui_sdl3_process_event(void *event);
C_FUNC void imgui_sdl3_shutdown();

C_FUNC void *imgui_get_draw_data();
C_FUNC void imgui_new_frame();
C_FUNC void imgui_end_frame();
C_FUNC void imgui_render();

C_FUNC imgui_id imgui_dock_space_over_viewport();
C_FUNC void imgui_update_platform_windows();
C_FUNC void imgui_render_platform_windows_default(void *platform_render_arg, void *renderer_render_arg);

C_FUNC void imgui_show_demo_window(bool *p_open);

C_FUNC bool imgui_begin(const char *name, bool *p_open, imgui_window_flags flags);
C_FUNC void imgui_end();

C_FUNC void imgui_text(const char *fmt, ...);
C_FUNC void imgui_bullet_text(const char *fmt, ...);

C_FUNC void imgui_push_style_color(imgui_col col, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
C_FUNC void imgui_pop_style_color(int count);

C_FUNC bool imgui_begin_child(const char* str_id, vec2 size, imgui_child_flags child_flags, imgui_window_flags window_flags);
C_FUNC void imgui_end_child();

#endif