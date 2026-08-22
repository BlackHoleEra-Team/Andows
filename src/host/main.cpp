// Andows 宿主壳 —— Win32 最小 demo
// 壳 = Win32 原生窗口 + 原生菜单;客户区是 Android 画面的贴图区(当前为占位画布)。

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {

constexpr wchar_t kWindowClass[] = L"AndowsHostWindow";
constexpr wchar_t kWindowTitle[] = L"Andows — Android Surface";
constexpr int kMenuFileExit = 1001;
constexpr int kMenuHelpAbout = 1002;

// RAII 封装 GDI brush,避免泄漏
class Brush {
public:
    explicit Brush(COLORREF color) : h_(CreateSolidBrush(color)) {}
    ~Brush() {
        if (h_) DeleteObject(h_);
    }
    Brush(const Brush&) = delete;
    Brush& operator=(const Brush&) = delete;
    HBRUSH get() const { return h_; }

private:
    HBRUSH h_;
};

// 绘制"Android 画面"占位画布(客户区)。
// 将来这里替换为 SurfaceFlinger → DXGI 的真实贴图。
void DrawAndroidCanvas(HDC hdc, int w, int h) {
    const int barH = 48;    // 顶部 app bar 高度
    const int pad = 16;     // 边距
    const int cardH = 64;   // 卡片高度
    const int cardGap = 12; // 卡片间距

    Brush bg(RGB(240, 240, 242));
    Brush appBar(RGB(38, 38, 42));
    Brush card(RGB(255, 255, 255));
    Brush accent(RGB(75, 63, 227));

    RECT all{0, 0, w, h};
    FillRect(hdc, &all, bg.get());

    // 顶部 app bar
    RECT bar{0, 0, w, barH};
    FillRect(hdc, &bar, appBar.get());

    SetBkMode(hdc, TRANSPARENT);
    HGDIOBJ oldFont = SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));

    RECT titleRect{pad, 0, w - pad, barH};
    SetTextColor(hdc, RGB(255, 255, 255));
    DrawTextW(hdc, L"Android Surface (placeholder)", -1, &titleRect,
              DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // 三张"笔记"卡片,模拟 Android 列表 UI
    const wchar_t* cards[] = {L"购物清单", L"读书笔记", L"待办事项"};
    for (int i = 0; i < 3; ++i) {
        int y = barH + pad + i * (cardH + cardGap);
        if (y + cardH > h - 40) break;
        RECT rc{pad, y, w - pad, y + cardH};
        FillRect(hdc, &rc, card.get());
        RECT accentBar{pad, y, pad + 4, y + cardH};
        FillRect(hdc, &accentBar, accent.get());
        RECT txt{pad + 20, y, w - pad, y + cardH};
        SetTextColor(hdc, RGB(23, 23, 23));
        DrawTextW(hdc, cards[i], -1, &txt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // 底部说明:壳是原生窗口,画面是整块贴图
    RECT note{pad, h - 28, w - pad, h - 8};
    SetTextColor(hdc, RGB(90, 90, 100));
    DrawTextW(hdc, L"Host: Win32 native window  |  Canvas: Android rendered (placeholder)",
              -1, &note, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    if (oldFont) SelectObject(hdc, oldFont);
}

// 创建窗口菜单(原生控件):文件 / 帮助
HMENU CreateAppMenu() {
    HMENU bar = CreateMenu();

    HMENU fileMenu = CreatePopupMenu();
    AppendMenuW(fileMenu, MF_STRING, kMenuFileExit, L"退出(&X)");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)fileMenu, L"文件(&F)");

    HMENU helpMenu = CreatePopupMenu();
    AppendMenuW(helpMenu, MF_STRING, kMenuHelpAbout, L"关于(&A)");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)helpMenu, L"帮助(&H)");

    return bar;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == kMenuFileExit) {
            DestroyWindow(hwnd);
        } else if (LOWORD(wParam) == kMenuHelpAbout) {
            MessageBoxW(hwnd, L"Andows — Android 界的 Wine(纯用户态兼容层)",
                        L"关于 Andows", MB_OK | MB_ICONINFORMATION);
        }
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        DrawAndroidCanvas(hdc, rc.right - rc.left, rc.bottom - rc.top);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

}  // namespace

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = kWindowClass;
    if (!RegisterClassW(&wc)) return 1;

    HWND hwnd = CreateWindowExW(
        0, kWindowClass, kWindowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 560, 420,
        nullptr, CreateAppMenu(), hInstance, nullptr);
    if (!hwnd) return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
