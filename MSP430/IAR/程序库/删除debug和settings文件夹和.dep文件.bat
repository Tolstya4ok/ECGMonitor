@echo on

@echo ɾ�����ļ��к����ļ����µ�debug settings�ļ��к�*.dep�ļ�;Ctrl+C����ȡ��

@pause

@rem ɾ���������ļ����ļ���

@rem for /r . %%a in (.) do @if exist "%%a\debug" @echo "%%a\debug"
@for /r . %%a in (.) do @if exist "%%a\debug" rd /s /q "%%a\debug"

@rem for /r . %%a in (.) do @if exist "%%a\settings" @echo "%%a\settings"
@for /r . %%a in (.) do @if exist "%%a\settings" rd /s /q "%%a\settings"

@del /F /S /Q *.dep

@echo completed
@pause