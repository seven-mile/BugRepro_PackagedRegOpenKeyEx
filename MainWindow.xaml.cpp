// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <wil/resource.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WinUI3Packaged::implementation
{

    inline void EnsureBackupRestorePrivilege() {

      wil::unique_handle hToken;
      winrt::check_bool(OpenProcessToken(GetCurrentProcess(),
                                         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                                         wil::out_param(hToken)));

      TOKEN_PRIVILEGES tp{};
      tp.PrivilegeCount = 1;
      tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

      winrt::check_bool(
          LookupPrivilegeValue(nullptr, SE_BACKUP_NAME, &tp.Privileges[0].Luid));

      winrt::check_bool(AdjustTokenPrivileges(hToken.get(), FALSE, &tp, sizeof(tp),
                                              nullptr, nullptr));

      winrt::check_bool(
          LookupPrivilegeValue(nullptr, SE_RESTORE_NAME, &tp.Privileges[0].Luid));

      winrt::check_bool(AdjustTokenPrivileges(hToken.get(), FALSE, &tp, sizeof(tp),
                                              nullptr, nullptr));
    }

    MainWindow::MainWindow()
    {
        InitializeComponent();

        EnsureBackupRestorePrivilege();

        while (!IsDebuggerPresent())
          Sleep(100);

        auto KEY_PATH = LR"(SOFTWARE)";

        wil::unique_hkey hkey;
        check_win32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_PATH, REG_OPTION_BACKUP_RESTORE, KEY_QUERY_VALUE, hkey.put()));

    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
