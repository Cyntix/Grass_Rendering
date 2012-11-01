FOR /F "tokens=*" %%G IN ('DIR /B /AD /S *debug*') DO RMDIR /S /Q %%G
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S *release*') DO RMDIR /S /Q %%G
FOR /F "tokens=*" %%G IN ('DIR /B /S *.ncb*') DO DEL %%G