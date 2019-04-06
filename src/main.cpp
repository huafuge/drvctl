
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>
#include <direct.h>

#pragma comment(lib, "advapi32.lib")

using namespace std;

SC_HANDLE g_srvmgr;
SC_HANDLE g_service;
const char *g_srvname;

void open_service(const char *srv_name)
{
    g_service = OpenServiceA(g_srvmgr, srv_name, SERVICE_ALL_ACCESS);
    if (!g_service)
    {
        fprintf(stderr, "OpenService Failure\n");
        exit(-1);
    }
}

namespace path
{
    string abspath(string path)
    {
        if (path[0] == '/' || path[1] == ':')
        {
            return path;
        }
        char buf[260];
        return string(getcwd(buf, sizeof(buf))) + "\\" + path;
    }
}

int drv_create(int argc, char *argv[])
{
    if (argc < 3)
    {
        puts("Not specified DRIVER_PATH");
        return -1;
    }
    auto srv_name = argv[2];
    auto sys_path = path::abspath(argv[3]);

    g_service = CreateServiceA(g_srvmgr, srv_name, argc >= 4 ? argv[4] : srv_name,
                                SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
                                SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
                                (PCHAR)sys_path.c_str(), NULL, NULL, NULL, NULL, NULL);
    if (!g_service)
    {
        fprintf(stderr, "CreateServiceA Failure\n");
        auto m_dwLastError = GetLastError();
        if (ERROR_SERVICE_EXISTS == m_dwLastError)
        {
            puts("Service is existed.");
        }
    }
    return 0;
}

int drv_start(int argc, char *argv[])
{
    open_service(argv[2]);
    if (!StartServiceA(g_service, NULL, NULL))
    {
        /* m_dwLastError = GetLastError(); */
        printf("StartServiceA Failure\n");
        return -1;
    }
    return 0;
}

int drv_stop(int argc, char *argv[])
{
    open_service(argv[2]);
    SERVICE_STATUS ss;
    if (!ControlService(g_service, SERVICE_CONTROL_STOP, &ss))
    {
        /* m_dwLastError = GetLastError(); */
        printf("StopService Failure\n");
        return -1;
    }
    return 0;
}

int drv_delete(int argc, char *argv[])
{
    open_service(argv[2]);
    if (!DeleteService(g_service))
    {
        /* m_dwLastError = GetLastError(); */
        printf("DeleteService Failure\n");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        puts("drvctl <action> args...\n\n"
             "action args...:\n"
             "  create SERVICE_NAME DRIVER_PATH [DISPLAY_NAME]\n"
             "  start  SERVICE_NAME\n"
             "  stop   SERVICE_NAME\n"
             "  delete SERVICE_NAME\n"
        );
        return -1;
    }

    g_srvname = argv[2];
    g_srvmgr = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!g_srvmgr)
    {
        fprintf(stderr, "OpenSCManagerA Failure\n");
        return -1;
    }

    auto action = string(argv[1]);

    if (action == "create")
    {
        return drv_create(argc, argv);
    }
    if (action == "start")
    {
        return drv_start(argc, argv);
    }
    if (action == "stop")
    {
        return drv_stop(argc, argv);
    }
    if (action == "delete")
    {
        return drv_delete(argc, argv);
    }
    return 0;
}
