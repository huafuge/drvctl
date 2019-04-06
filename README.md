
drvctl -- Command line utility for Driver Control

## Usage

    drvctl <action> args...
    action args...:

      create SERVICE_NAME DRIVER_PATH [DISPLAY_NAME]
      start  SERVICE_NAME
      stop   SERVICE_NAME
      delete SERVICE_NAME

## Example

```cmd
@set DRIVER_SRV_NAME=kernel
@set DRIVER_BUILD_PATH=Z:\Win64Driver\build\windows\x64\debug\kernel.sys
@set DRIVER_LOCAL_PATH=kernel.sys

drvctl stop %DRIVER_SRV_NAME%
drvctl delete %DRIVER_SRV_NAME%

drvctl create %DRIVER_SRV_NAME% %DRIVER_LOCAL_PATH% & drvctl start %DRIVER_SRV_NAME%
```
